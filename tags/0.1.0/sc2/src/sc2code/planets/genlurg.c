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

// JMS 2010: -Totally new file: Lurg home system
//			 -Also shofixti crash site

#include "build.h"
#include "encount.h"
#include "globdata.h"
#include "lander.h"
#include "nameref.h"
#include "resinst.h"
#include "setup.h"
#include "state.h"
#include "planets/genall.h"
#include "libs/mathlib.h"

void
GenerateLurg (BYTE control)
{
	switch (control)
	{
		case GENERATE_ORBITAL:
			if (pSolarSysState->pOrbitalDesc == &pSolarSysState->PlanetDesc[0])
			{
				if (ActivateStarShip (LURG_SHIP, SPHERE_TRACKING))
				{
					NotifyOthers (LURG_SHIP, (BYTE)~0);
					PutGroupInfo (GROUPS_RANDOM, GROUP_SAVE_IP);
					ReinitQueue (&GLOBAL (ip_group_q));
					assert (CountLinks (&GLOBAL (npc_built_ship_q)) == 0);

					CloneShipFragment (LURG_SHIP,
							&GLOBAL (npc_built_ship_q), INFINITE_FLEET);

					pSolarSysState->MenuState.Initialized += 2;
					GLOBAL (CurrentActivity) |= START_INTERPLANETARY;
					SET_GAME_STATE (GLOBAL_FLAGS_AND_DATA, 1 << 7);
					InitCommunication (LURG_CONVERSATION);
					pSolarSysState->MenuState.Initialized -= 2;

					if (!(GLOBAL (CurrentActivity) & (CHECK_ABORT | CHECK_LOAD)))
					{
						GLOBAL (CurrentActivity) &= ~START_INTERPLANETARY;
						ReinitQueue (&GLOBAL (npc_built_ship_q));
						GetGroupInfo (GROUPS_RANDOM, GROUP_LOAD_IP);
					}
					break;
				}
				else
				{
					LoadStdLanderFont (&pSolarSysState->SysInfo.PlanetInfo);
					pSolarSysState->PlanetSideFrame[1] =
							CaptureDrawable (
							LoadGraphic (RUINS_MASK_PMAP_ANIM)
							);
					pSolarSysState->SysInfo.PlanetInfo.DiscoveryString =
							CaptureStringTable (
									LoadStringTable (RUINS_STRTAB)
									);
					if (!GET_GAME_STATE (ULTRON_CONDITION))
						pSolarSysState->SysInfo.PlanetInfo.DiscoveryString =
								SetAbsStringTableIndex (
								pSolarSysState->SysInfo.PlanetInfo.DiscoveryString,
								1
								);
				}
			}
		default:
			GenerateRandomIP (control);
			break;
	}
}

void
GenerateShofixtiCrashSite (BYTE control)
{
	switch (control)
	{
		DWORD rand_val, old_rand;
			
		case GENERATE_ENERGY:
		{
			if (pSolarSysState->pOrbitalDesc == &pSolarSysState->PlanetDesc[2]
				&& (GET_GAME_STATE (WHICH_SHIP_PLAYER_HAS) != 2) )
			{
				old_rand = TFB_SeedRandom (pSolarSysState->SysInfo.PlanetInfo.ScanSeed[ENERGY_SCAN]);
				rand_val = TFB_Random ();
				
				pSolarSysState->SysInfo.PlanetInfo.CurPt.x = 187;
				pSolarSysState->SysInfo.PlanetInfo.CurPt.y = MAP_HEIGHT - 16;
				pSolarSysState->SysInfo.PlanetInfo.CurDensity = 0;
				pSolarSysState->SysInfo.PlanetInfo.CurType = 0; // JMS: Fake picking the blip up upon finding...
				
				pSolarSysState->CurNode = 1;
				
				// ... which allows us to use this retrieve switch to make things happen
				if (pSolarSysState->SysInfo.PlanetInfo.ScanRetrieveMask[ENERGY_SCAN]
					& (1L << 0))
				{
					pSolarSysState->SysInfo.PlanetInfo.ScanRetrieveMask[ENERGY_SCAN]
					&= ~(1L << 0); // Now we reverse the fake picking up so the blip stays on the planet.
					((PLANETSIDE_DESC*)pMenuState->ModuleFrame)->InTransit = TRUE;
					SET_GAME_STATE (BLACK_ORB_STATE, 1);
					SET_GAME_STATE (WHICH_SHIP_PLAYER_HAS, 2);
				}
				
				TFB_SeedRandom (old_rand);
				break;
			}
			
			pSolarSysState->CurNode = 0;
			break;
		}
		case GENERATE_ORBITAL:
		{
			if (pSolarSysState->pOrbitalDesc == &pSolarSysState->PlanetDesc[2]
					&& !GET_GAME_STATE (CRASH_SITE_UNPROTECTED)
					&& ActivateStarShip (LURG_SHIP, SPHERE_TRACKING))
			{
				COUNT i;
					
				PutGroupInfo (GROUPS_RANDOM, GROUP_SAVE_IP);
				ReinitQueue (&GLOBAL (ip_group_q));
				assert (CountLinks (&GLOBAL (npc_built_ship_q)) == 0);
					
				for (i = 0; i < 5; ++i)
					CloneShipFragment (LURG_SHIP, &GLOBAL (npc_built_ship_q), 0);
					
				pSolarSysState->MenuState.Initialized += 2;
				GLOBAL (CurrentActivity) |= START_INTERPLANETARY;
				SET_GAME_STATE (GLOBAL_FLAGS_AND_DATA, 1 << 6);
				InitCommunication (LURG_CONVERSATION);
				pSolarSysState->MenuState.Initialized -= 2;
					
				if (GLOBAL (CurrentActivity) & (CHECK_ABORT | CHECK_LOAD))
					break;
				else
				{
					BOOLEAN LurgSurvivors;
					LurgSurvivors = GetHeadLink ( &GLOBAL (npc_built_ship_q) ) != 0;
						
					GLOBAL (CurrentActivity) &= ~START_INTERPLANETARY;
					ReinitQueue (&GLOBAL (npc_built_ship_q));
					GetGroupInfo (GROUPS_RANDOM, GROUP_LOAD_IP);
						
					if (LurgSurvivors)
						break;
						
					LockMutex (GraphicsLock);
					RepairSISBorder ();
					UnlockMutex (GraphicsLock);
					SET_GAME_STATE (CRASH_SITE_UNPROTECTED, 1);
				}
			}
				
			LoadStdLanderFont (&pSolarSysState->SysInfo.PlanetInfo);
			pSolarSysState->PlanetSideFrame[1] = CaptureDrawable (LoadGraphic (PRECURSOR_BATTLESHIP_MASK_PMAP_ANIM) );
			pSolarSysState->SysInfo.PlanetInfo.DiscoveryString = CaptureStringTable (LoadStringTable (SHOFIXTI_CRASH_SITE_STRTAB));
	
			GenerateRandomIP (GENERATE_ORBITAL);
			if (pSolarSysState->pOrbitalDesc == &pSolarSysState->PlanetDesc[2])
			{
				pSolarSysState->SysInfo.PlanetInfo.Weather = 0;
				pSolarSysState->SysInfo.PlanetInfo.Tectonics = 1;
			}
			
			break;
		}
		case GENERATE_MOONS:
			GenerateRandomIP (GENERATE_MOONS);
			if (pSolarSysState->pBaseDesc == &pSolarSysState->PlanetDesc[2])
			{
				COUNT angle;
				
				pSolarSysState->MoonDesc[0].data_index = SELENIC_WORLD;
				pSolarSysState->MoonDesc[0].radius = MIN_MOON_RADIUS
				+ (MAX_MOONS - 1) * MOON_DELTA;
				rand_val = TFB_Random ();
				angle = NORMALIZE_ANGLE (LOWORD (rand_val));
				pSolarSysState->MoonDesc[0].location.x =
				COSINE (angle, pSolarSysState->MoonDesc[0].radius);
				pSolarSysState->MoonDesc[0].location.y =
				SINE (angle, pSolarSysState->MoonDesc[0].radius);
			}
			break;
		case GENERATE_PLANETS:
		{
			COUNT angle;
			
			GenerateRandomIP (GENERATE_PLANETS);
			
			pSolarSysState->PlanetDesc[2].data_index = MAROON_WORLD;
			pSolarSysState->PlanetDesc[2].NumPlanets = 1;
			angle = ARCTAN (
							pSolarSysState->PlanetDesc[2].location.x,
							pSolarSysState->PlanetDesc[2].location.y
							);
			pSolarSysState->PlanetDesc[2].location.x =
			COSINE (angle, pSolarSysState->PlanetDesc[2].radius);
			pSolarSysState->PlanetDesc[2].location.y =
			SINE (angle, pSolarSysState->PlanetDesc[2].radius);
			break;
		}
		default:
			GenerateRandomIP (control);
			break;
	}
}