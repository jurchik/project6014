// JMS 2010: Explorer has smaller thrust increment in interplanetary
// JMS 2011: Added to BYTE res_factor to SUMMARY_DESC to enable saving/loading from different res modes.

// JMS_GFX 2011: Merged the resolution Factor stuff from UQM-HD.

#ifndef _SIS_H
#define _SIS_H

#include "libs/compiler.h"
#include "libs/gfxlib.h"
#include "planets/elemdata.h"
		// for NUM_ELEMENT_CATEGORIES

#define CLEAR_SIS_RADAR (1 << 2)
#define DRAW_SIS_DISPLAY (1 << 3)

#define UNDEFINED_DELTA 0x7FFF

#define NUM_DRIVE_SLOTS 11
#define NUM_JET_SLOTS 8
#define NUM_MODULE_SLOTS 16

#define CREW_POD_CAPACITY 50
#define STORAGE_BAY_CAPACITY 500 /* km cubed */
#define FUEL_TANK_SCALE 100
#define FUEL_TANK_CAPACITY (50 * FUEL_TANK_SCALE)
#define HEFUEL_TANK_CAPACITY (100 * FUEL_TANK_SCALE)
#define MODULE_COST_SCALE 50

/** Capacities for Explorer **/
#define EXPLORER_CREW_CAPACITY 50
#define EXPLORER_FUEL_CAPACITY (200 * FUEL_TANK_SCALE)

#define CREW_EXPENSE_THRESHOLD 1000
#define CREW_PER_ROW 5
#define SBAY_MASS_PER_ROW 50

#define EXPLORER_NUM_MODULE_SLOTS 0

#define MAX_FUEL_BARS 10
#define FUEL_VOLUME_PER_ROW (1 ? (HEFUEL_TANK_CAPACITY*2 / 20) : (HEFUEL_TANK_CAPACITY / MAX_FUEL_BARS))
#define FUEL_RESERVE FUEL_VOLUME_PER_ROW

/** Capacities for Explorer end **/

#define IP_SHIP_THRUST_INCREMENT 8
#define IP_SHIP_TURN_WAIT 17
#define IP_SHIP_TURN_DECREMENT 2

// JMS
#define EXPLORER_IP_SHIP_THRUST_INCREMENT 8 // JMS: WAS 5

#define BIO_CREDIT_VALUE 2

enum
{
	PLANET_LANDER = 0,
		/* thruster types */
	FUSION_THRUSTER,
		/* jet types */
	TURNING_JETS,
		/* module types */
	CREW_POD,
	STORAGE_BAY,
	FUEL_TANK,
	HIGHEFF_FUELSYS,
	DYNAMO_UNIT,
	SHIVA_FURNACE,
	GUN_WEAPON,
	BLASTER_WEAPON,
	CANNON_WEAPON,
	TRACKING_SYSTEM,
	ANTIMISSILE_DEFENSE,
	
	NUM_PURCHASE_MODULES,

	BOMB_MODULE_0 = NUM_PURCHASE_MODULES,
	BOMB_MODULE_1,
	BOMB_MODULE_2,
	BOMB_MODULE_3,
	BOMB_MODULE_4,
	BOMB_MODULE_5,

	NUM_MODULES /* must be last entry */
};

#define EMPTY_SLOT NUM_MODULES
#define NUM_BOMB_MODULES 10

// JMS_GFX: Module drawing follows resolution factor
#define DRIVE_SIDE_X (31 << RESOLUTION_FACTOR)
#define DRIVE_SIDE_Y (56 << RESOLUTION_FACTOR)
#define DRIVE_TOP_X (33 << RESOLUTION_FACTOR)
#define DRIVE_TOP_Y ((65 + 21) << RESOLUTION_FACTOR)

#define JET_SIDE_X (71 << RESOLUTION_FACTOR)
#define JET_SIDE_Y (48 << RESOLUTION_FACTOR)
#define JET_TOP_X (70 << RESOLUTION_FACTOR)
#define JET_TOP_Y ((73 + 21) << RESOLUTION_FACTOR)

#define MODULE_SIDE_X (17 << RESOLUTION_FACTOR)
#define MODULE_SIDE_Y (14 << RESOLUTION_FACTOR)
#define MODULE_TOP_X (17 << RESOLUTION_FACTOR)
#define MODULE_TOP_Y ((96 + 21) << RESOLUTION_FACTOR)

#define SHIP_PIECE_OFFSET (12 << RESOLUTION_FACTOR)

#define MAX_BUILT_SHIPS 6 // No more than 6 companion ships for the Explorer
		/* Maximum number of ships escorting the SIS */
#define MAX_LANDERS 3 // No more than 3 landers

#define SUPPORT_SHIP_PTS \
	{3 +  0, 30 + (1 * 16)}, \
	{3 + 42, 30 + (1 * 16)}, \
	{3 +  0, 30 + (0 * 16)}, \
	{3 + 42, 30 + (0 * 16)}, \
	{3 +  0, 30 + (5 * 16)}, \
	{3 + 42, 30 + (5 * 16)},
// Only authorized positions according to graphics (chflagstat.0.png)
// BW: now these need to be constant because they initialize an array
// in roster.c. Resolution Factor will be appied when effectively drawing.

#define SIS_NAME_SIZE 16

typedef struct
{
	SDWORD log_x, log_y;

	DWORD ResUnits;

	DWORD FuelOnBoard;
	COUNT CrewEnlisted;
			// Number of crew on board, not counting the captain.
			// Set to (COUNT) ~0 to indicate game over.
	COUNT TotalElementMass, TotalBioMass;

	BYTE ModuleSlots[NUM_MODULE_SLOTS+1];  /* FIXME: NUM_MODULE_SLOTS is zero, but that won't compile. */
	BYTE DriveSlots[NUM_DRIVE_SLOTS];
	BYTE JetSlots[NUM_JET_SLOTS];

	BYTE NumLanders;

	COUNT ElementAmounts[NUM_ELEMENT_CATEGORIES];

	UNICODE ShipName[SIS_NAME_SIZE];
	UNICODE CommanderName[SIS_NAME_SIZE];
	UNICODE PlanetName[SIS_NAME_SIZE];
} SIS_STATE;

// XXX: Theoretically, a player can have 17 devices on board without
//   cheating. We only provide
//   room for 16 below, which is not really a problem since this
//   is only used for displaying savegame summaries. There is also
//   room for only 16 devices on screen.
#define MAX_EXCLUSIVE_DEVICES 16

typedef struct
{
	SIS_STATE SS;
	BYTE Activity;
	BYTE Flags;
	BYTE day_index, month_index;
	COUNT year_index;
	BYTE MCreditLo, MCreditHi;
	BYTE NumShips, NumDevices;
	BYTE ShipList[MAX_BUILT_SHIPS];
	BYTE DeviceList[MAX_EXCLUSIVE_DEVICES];
	BYTE res_factor;	// JMS: Stores resolution factor to enable saving/loading from different res modes.
} SUMMARY_DESC;

#define OVERRIDE_LANDER_FLAGS (1 << 7)
#define AFTER_BOMB_INSTALLED (1 << 7)

extern void RepairSISBorder (void);
extern void InitSISContexts (void);
extern void DrawSISFrame (void);
extern void ClearSISRect (BYTE ClearFlags);
extern void SetFlashRect (RECT *pRect);
#define SFR_MENU_3DO ((RECT*)~0L)
#define SFR_MENU_ANY ((RECT*)~1L)
extern void DrawHyperCoords (POINT puniverse);
extern void DrawSISTitle (UNICODE *pStr);
extern BOOLEAN DrawSISMessageEx (const UNICODE *pStr, SIZE CurPos,
		SIZE ExPos, COUNT flags);
#define DSME_NONE     0
#define DSME_SETFR    (1 << 0)
#define DSME_CLEARFR  (1 << 1)
#define DSME_BLOCKCUR (1 << 2)
#define DSME_MYCOLOR  (1 << 3)
extern void DrawSISMessage (const UNICODE *pStr);
extern void DateToString (unsigned char *buf, size_t bufLen,
		BYTE month_index, BYTE day_index, COUNT year_index);

extern void DrawStatusMessage (const UNICODE *pStr);
typedef enum
{
	SMM_UNDEFINED = 0,
	SMM_DATE,
	SMM_RES_UNITS,
	SMM_CREDITS,

	SMM_DEFAULT = SMM_DATE,
} StatMsgMode;
// Sets the new mode and return the previous
extern StatMsgMode SetStatusMessageMode (StatMsgMode);

extern void DrawLanders (void);
extern void DrawStorageBays (BOOLEAN Refresh);
extern void GetGaugeRect (RECT *pRect, BOOLEAN IsCrewRect);
extern void DrawFlagshipStats (void);
extern void SaveFlagshipState (void);
void DrawAutoPilotMessage (BOOLEAN Reset);

extern void DeltaSISGauges (SIZE crew_delta, SIZE fuel_delta, int
		resunit_delta);
extern COUNT GetCrewCount (void);
extern COUNT GetCPodCapacity (POINT *ppt);
extern COUNT GetLBayCapacity (POINT *ppt);
extern COUNT GetSBayCapacity (POINT *ppt);
extern DWORD GetFTankCapacity (POINT *ppt);
extern COUNT CountSISPieces (BYTE piece_type);

extern void DrawFlagshipName (BOOLEAN InStatusArea);
extern void DrawCaptainsName (void);

#endif /* _SIS_H */

