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

// JMS_GFX 2011: Merged resolution Factor stuff from UQM-HD.

#ifndef PURE_H
#define PURE_H

#include "libs/graphics/sdl/sdl_common.h"

int TFB_Pure_InitGraphics (int driver, int flags, int width, int height, unsigned int resolutionFactor);  // JMS_GFX: Added resolutionFactor
int TFB_Pure_ConfigureVideo (int driver, int flags, int width, int height, int togglefullscreen, unsigned int resolutionFactor);  // JMS_GFX: Added resolutionFactor
void Scale_PerfTest (void);

#endif
