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

#include "oscill.h"

// XXX: we should not refer to units.h here because we should not be
//   using RADAR_WIDTH constants!
#include "units.h"
#include "setup.h"
		// for OffScreenContext
#include "libs/graphics/gfx_common.h"
#include "libs/graphics/drawable.h"
#include "libs/sound/sound.h"
#include "libs/sound/trackplayer.h"


static FRAME scope_frame;
static int scope_init = 0;
static FRAME scopeWork;
static Color scopeColor;
BOOLEAN oscillDisabled = FALSE;

void
InitOscilloscope (DWORD x, DWORD y, DWORD width, DWORD height, FRAME f)
{
	scope_frame = f;
	if (!scope_init)
	{
		EXTENT size = GetFrameBounds (scope_frame);
		POINT midPt = {size.width / 2, size.height / 2};

		// mid-image pixel defines the color of scope lines
		scopeColor = GetFramePixel (scope_frame, midPt);
		
		scopeWork = CaptureDrawable (CreateDrawable (
				WANT_PIXMAP | MAPPED_TO_DISPLAY,
				size.width, size.height, 1));

		scope_init = 1;
	}
	/* remove compiler warnings */
	(void) x;
	(void) y;
}

void
UninitOscilloscope (void)
{
	// XXX: Is never called (BUG?)
	DestroyDrawable (ReleaseDrawable (scopeWork));
	scopeWork = NULL;
	scope_init = 0;
}

// draws the oscilloscope
void
DrawOscilloscope (void)
{
	STAMP s;
	BYTE scope_data[RADAR_WIDTH - 2];

	if (oscillDisabled)
		return;

	if (GraphForegroundStream (scope_data, RADAR_WIDTH - 2, RADAR_HEIGHT - 2)) 
	{
		int i;
		CONTEXT oldContext;

		oldContext = SetContext (OffScreenContext);
		SetContextFGFrame (scopeWork);
		SetContextClipRect (NULL);
		
		// draw the background image
		s.origin.x = 0;
		s.origin.y = 0;
		s.frame = scope_frame;
		DrawStamp (&s);

		// draw the scope lines
		SetContextForeGroundColor (scopeColor);
		for (i = 0; i < RADAR_WIDTH - 3; ++i)
		{
			LINE line;

			line.first.x = i + 1;
			line.first.y = scope_data[i] + 1;
			line.second.x = i + 2;
			line.second.y = scope_data[i + 1] + 1;
			DrawLine (&line);
		}

		SetContext (oldContext);

		s.frame = scopeWork;
	}
	else
	{	// no data -- draw blank scope background
		s.frame = scope_frame;
	}

	// draw the final scope image to screen
	s.origin.x = 0;
	s.origin.y = 0;
	DrawStamp (&s);
}


static STAMP sliderStamp;
static STAMP buttonStamp;
static BOOLEAN sliderChanged = FALSE;
int sliderSpace;  // slider width - button width
BOOLEAN sliderDisabled = FALSE;

/*
 * Initialise the communication progress bar
 * x - x location of slider
 * y - y location of slider
 * width - width of slider
 * height - height of slider
 * bwidth - width of button indicating current progress
 * bheight - height of button indicating progress
 * f - image for the slider
 */                        

void
InitSlider (int x, int y, int width, int height,
		int bwidth, int bheight, FRAME f)
{
	sliderStamp.origin.x = x;
	sliderStamp.origin.y = y;
	sliderStamp.frame = f;
	buttonStamp.origin.x = x;
	buttonStamp.origin.y = y - ((bheight - height) >> 1);
	sliderSpace = width - bwidth;
}

void
SetSliderImage (FRAME f)
{
	sliderChanged = TRUE;
	buttonStamp.frame = f;
}

void
DrawSlider (void)
{
	int offs;
	static int last_offs = -1;

	if (sliderDisabled)
		return;
	
	offs = GetTrackPosition (sliderSpace);
	if (offs != last_offs || sliderChanged)
	{
		sliderChanged = FALSE;
		last_offs = offs;
		buttonStamp.origin.x = sliderStamp.origin.x + offs;
		BatchGraphics ();
		DrawStamp (&sliderStamp);
		DrawStamp (&buttonStamp);
		UnbatchGraphics ();
	}
}

