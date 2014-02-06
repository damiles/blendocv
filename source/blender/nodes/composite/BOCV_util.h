/**
 * $Id: CMP_util.h 14352 2008-04-07 15:21:25Z blendix $
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
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The Original Code is Copyright (C) 2006 Blender Foundation.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): Damiles - David Millan Escriva (david@artresnet.com).
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#ifndef BOCV_NODE_UTILS_H_
#define BOCV_NODE_UTILS_H_

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "MEM_guardedalloc.h"

#include "DNA_camera_types.h" /* qdn: defocus node, need camera info */
#include "DNA_color_types.h"
#include "DNA_ID.h"
#include "DNA_image_types.h"
#include "DNA_material_types.h"
#include "DNA_node_types.h"
#include "DNA_object_types.h"
#include "DNA_scene_types.h"
#include "DNA_texture_types.h"

#include "BLI_math.h"
#include "BLI_blenlib.h"
#include "BLI_rand.h"
#include "BLI_threads.h"
#include "BLI_utildefines.h"
#include "BLI_utildefines.h"

#include "BKE_blender.h"
#include "BKE_colortools.h"
#include "BKE_global.h"
#include "BKE_image.h"
#include "BKE_main.h"
#include "BKE_material.h"
#include "BKE_node.h"
#include "BKE_texture.h"

#include "BKE_library.h"
#include "BKE_object.h"

#include "../NOD_composite.h"
#include "../intern/node_util.h"

#include "IMB_imbuf_types.h"
#include "IMB_imbuf.h"

#include "RE_pipeline.h"
#include "RE_shader_ext.h"
#include "RE_render_ext.h"

#include "node_composite_util.h"

#include <cv.h>
//#include <cxcore.h>
//#include <cxerror.h>

/* *************************** operations support *************************** */

CvArr* BOCV_CreateArrFrom(void* src);
int BOCV_checkAreSameType(void* src1,void* src2 );
int BOCV_checkSameNChannels(void* src1, void* src2);
int BOCV_checkMask(void* src1, void* mask);
IplImage *BOCV_IplImage_attach(CompBuf *cbuf);
IplImage *BOCV_Mask_attach(CompBuf *cbuf);
void BOCV_IplImage_detach(IplImage *img);
/*IplImage *BOCV_Socket_IplImage(bNodeStack *in);
IplImage *BOCV_Alloc_IplImage(int x, int y, int type, CompBuf* cbuf);*/
#endif
