/*
 * $Id: GHOST_NDOFManagerWin32.h 39007 2011-08-04 03:14:00Z merwin $
 *
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Contributor(s):
 *   Mike Erwin
 *
 * ***** END GPL LICENSE BLOCK *****
 */


#ifndef _GHOST_NDOFMANAGERWIN32_H_
#define _GHOST_NDOFMANAGERWIN32_H_

#ifdef WITH_INPUT_NDOF

#include "GHOST_NDOFManager.h"


class GHOST_NDOFManagerWin32 : public GHOST_NDOFManager
{
public:
	GHOST_NDOFManagerWin32(GHOST_System&);
	bool available();
};


#endif // WITH_INPUT_NDOF
#endif // #include guard
