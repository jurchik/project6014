//Copyright Paul Reiche, Fred Ford. 1992-2002

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

// JMS 2010 - Totally new file: Lurg ship

#include "ships/ship.h"
#include "ships/lurg/resinst.h"

#include "libs/mathlib.h"


#define MAX_CREW 20
#define MAX_ENERGY 16
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST 3
#define SPECIAL_ENERGY_COST 1
#define ENERGY_WAIT 4
#define MAX_THRUST 40
#define THRUST_INCREMENT 8
#define TURN_WAIT 1
#define THRUST_WAIT 0
#define WEAPON_WAIT 10
#define SPECIAL_WAIT 0

#define SHIP_MASS 4
#define MISSILE_SPEED DISPLAY_TO_WORLD (30)
#define MISSILE_LIFE 10

static RACE_DESC lurg_desc =
{
	{ /* SHIP_INFO */
		FIRES_FORE,
		20, /* Super Melee cost */
		MAX_CREW, MAX_CREW,
		MAX_ENERGY, MAX_ENERGY,
		LURG_RACE_STRINGS,
		LURG_ICON_MASK_PMAP_ANIM,
		LURG_MICON_MASK_PMAP_ANIM,
		NULL, NULL, NULL, SHIP_IS_NOT_DAMAGED
	},
	{ /* FLEET_STUFF */
		0, //1525 / SPHERE_RADIUS_INCREMENT * 2, /* Initial SoI radius */
		{ /* Known location (center of SoI) */
			3710, 4389,
		},
	},
	{
		MAX_THRUST,
		THRUST_INCREMENT,
		ENERGY_REGENERATION,
		WEAPON_ENERGY_COST,
		SPECIAL_ENERGY_COST,
		ENERGY_WAIT,
		TURN_WAIT,
		THRUST_WAIT,
		WEAPON_WAIT,
		SPECIAL_WAIT,
		SHIP_MASS,
	},
	{
		{
			LURG_BIG_MASK_PMAP_ANIM,
			LURG_MED_MASK_PMAP_ANIM,
			LURG_SML_MASK_PMAP_ANIM,
		},
		{
			LURGGOB_BIG_MASK_PMAP_ANIM,
			LURGGOB_MED_MASK_PMAP_ANIM,
			LURGGOB_SML_MASK_PMAP_ANIM,
		},
		{
			NULL_RESOURCE,
			NULL_RESOURCE,
			NULL_RESOURCE,
		},
		{
			LURG_CAPTAIN_MASK_PMAP_ANIM,
			NULL, NULL, NULL, NULL, NULL
		},
		LURG_VICTORY_SONG,
		LURG_SHIP_SOUNDS,
		{ NULL, NULL, NULL },
		{ NULL, NULL, NULL },
		{ NULL, NULL, NULL },
		NULL, NULL
	},
	{
		0,
		(MISSILE_SPEED * MISSILE_LIFE) >> 1,
		NULL,
	},
	(UNINIT_FUNC *) NULL,
	(PREPROCESS_FUNC *) NULL,
	(POSTPROCESS_FUNC *) NULL,
	(INIT_WEAPON_FUNC *) NULL,
	0,
};

static void
lurg_intelligence (ELEMENT *ShipPtr, EVALUATE_DESC *ObjectsOfConcern,
		COUNT ConcernCounter)
{
	STARSHIP *StarShipPtr;
	EVALUATE_DESC *lpEvalDesc;

	GetElementStarShip (ShipPtr, &StarShipPtr);

	lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
	if (StarShipPtr->special_counter || lpEvalDesc->ObjectPtr == 0)
		StarShipPtr->ship_input_state &= ~SPECIAL;
	else
	{
		BOOLEAN LinedUp;
		COUNT direction_angle;
		SIZE delta_x, delta_y;

		delta_x = lpEvalDesc->ObjectPtr->next.location.x
				- ShipPtr->next.location.x;
		delta_y = lpEvalDesc->ObjectPtr->next.location.y
				- ShipPtr->next.location.y;
		direction_angle = ARCTAN (delta_x, delta_y);

		LinedUp = (BOOLEAN)(NORMALIZE_ANGLE (NORMALIZE_ANGLE (direction_angle
				- FACING_TO_ANGLE (StarShipPtr->ShipFacing))
				+ QUADRANT) <= HALF_CIRCLE);

		if (!LinedUp
				|| lpEvalDesc->which_turn > 20
				|| NORMALIZE_ANGLE (
				lpEvalDesc->facing
				- (FACING_TO_ANGLE (StarShipPtr->ShipFacing)
				+ HALF_CIRCLE) + OCTANT
				) > QUADRANT)
			StarShipPtr->ship_input_state &= ~SPECIAL;
		else if (LinedUp && lpEvalDesc->which_turn <= 12)
			StarShipPtr->ship_input_state |= SPECIAL;

		if (StarShipPtr->ship_input_state & SPECIAL)
			lpEvalDesc->MoveState = PURSUE;
	}

	ship_intelligence (ShipPtr,
			ObjectsOfConcern, ConcernCounter);

	if (StarShipPtr->ship_input_state & SPECIAL)
		StarShipPtr->ship_input_state |= THRUST | WEAPON;

	lpEvalDesc = &ObjectsOfConcern[ENEMY_WEAPON_INDEX];
	if (StarShipPtr->special_counter == 0
			&& lpEvalDesc->ObjectPtr
			&& lpEvalDesc->MoveState == AVOID
			&& ShipPtr->turn_wait == 0)
	{
		StarShipPtr->ship_input_state &= ~THRUST;
		StarShipPtr->ship_input_state |= SPECIAL;
		if (!(StarShipPtr->cur_status_flags & (LEFT | RIGHT)))
			StarShipPtr->ship_input_state |= 1 << ((BYTE)TFB_Random () & 1);
		else
			StarShipPtr->ship_input_state |=
					StarShipPtr->cur_status_flags & (LEFT | RIGHT);
	}
}

static COUNT
initialize_horn (ELEMENT *ShipPtr, HELEMENT HornArray[])
{
#define MISSILE_HITS 6
#define MISSILE_DAMAGE 2
#define MISSILE_OFFSET 2
#define LURG_OFFSET 23
	STARSHIP *StarShipPtr;
	MISSILE_BLOCK MissileBlock;

	GetElementStarShip (ShipPtr, &StarShipPtr);
	MissileBlock.cx = ShipPtr->next.location.x;
	MissileBlock.cy = ShipPtr->next.location.y;
	MissileBlock.farray = StarShipPtr->RaceDescPtr->ship_data.weapon;
	MissileBlock.face = MissileBlock.index = StarShipPtr->ShipFacing;
	MissileBlock.sender = (ShipPtr->state_flags & (GOOD_GUY | BAD_GUY))
			| IGNORE_SIMILAR;
	MissileBlock.pixoffs = LURG_OFFSET;
	MissileBlock.speed = MISSILE_SPEED;
	MissileBlock.hit_points = MISSILE_HITS;
	MissileBlock.damage = MISSILE_DAMAGE;
	MissileBlock.life = MISSILE_LIFE;
	MissileBlock.preprocess_func = NULL;
	MissileBlock.blast_offs = MISSILE_OFFSET;
	HornArray[0] = initialize_missile (&MissileBlock);
	return (1);
}

static void
lurg_preprocess (ELEMENT *ElementPtr)
{
	STARSHIP *StarShipPtr;

	GetElementStarShip (ElementPtr, &StarShipPtr);
	if ((StarShipPtr->cur_status_flags & SPECIAL)
/*
			&& DeltaEnergy (ElementPtr, -SPECIAL_ENERGY_COST)
*/
			)
	{
		SIZE add_facing;

		add_facing = 0;
		if (StarShipPtr->cur_status_flags & THRUST)
		{
			if (ElementPtr->thrust_wait == 0)
				++ElementPtr->thrust_wait;

			add_facing = ANGLE_TO_FACING (HALF_CIRCLE);
		}
		if (StarShipPtr->cur_status_flags & LEFT)
		{
			if (ElementPtr->turn_wait == 0)
				++ElementPtr->turn_wait;

			if (add_facing)
				add_facing += ANGLE_TO_FACING (OCTANT);
			else
				add_facing = -ANGLE_TO_FACING (QUADRANT);
		}
		else if (StarShipPtr->cur_status_flags & RIGHT)
		{
			if (ElementPtr->turn_wait == 0)
				++ElementPtr->turn_wait;

			if (add_facing)
				add_facing -= ANGLE_TO_FACING (OCTANT);
			else
				add_facing = ANGLE_TO_FACING (QUADRANT);
		}

		if (add_facing)
		{
			COUNT facing;
			STATUS_FLAGS thrust_status;

			facing = StarShipPtr->ShipFacing;
			StarShipPtr->ShipFacing = NORMALIZE_FACING (
					facing + add_facing
					);
			thrust_status = inertial_thrust (ElementPtr);
			StarShipPtr->cur_status_flags &=
					~(SHIP_AT_MAX_SPEED
					| SHIP_BEYOND_MAX_SPEED
					| SHIP_IN_GRAVITY_WELL);
			StarShipPtr->cur_status_flags |= thrust_status;
			StarShipPtr->ShipFacing = facing;
		}
	}
}

RACE_DESC*
init_lurg (void)
{
	RACE_DESC *RaceDescPtr;

	lurg_desc.preprocess_func = lurg_preprocess;
	lurg_desc.init_weapon_func = initialize_horn;
	lurg_desc.cyborg_control.intelligence_func = lurg_intelligence;

	RaceDescPtr = &lurg_desc;

	return (RaceDescPtr);
}