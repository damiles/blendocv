/**
 * $Id: CMP_hueSatVal.c 14749 2008-05-08 19:57:10Z blendix $
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
 * Contributor(s): none yet.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include "../../node_composite_util.h"
#include "../../BOCV_util.h"


static bNodeSocketTemplate cmp_node_cvResize_in[]= {
	{	SOCK_OCV_IMAGE, 1, "cvImage",		1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{	SOCK_FLOAT, 1, "Width",			640.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10000.0f},
	{	SOCK_FLOAT, 1, "Height",		480.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10000.0f},
	{	-1, 0, ""	}
};
static bNodeSocketTemplate cmp_node_cvResize_out[]= {
	{	SOCK_OCV_IMAGE, 0, "cvImage",			0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
	{	-1, 0, ""	}
};

static void node_composit_exec_cvResize(void *data, bNode *node, bNodeStack **in, bNodeStack **out)
{
    //TODO: Use atach buffers
	int w,h;	
	IplImage *image, *dst;
	CV_FUNCNAME( "cvResize" ); 
	if(out[0]->hasoutput==0) return;
	cvSetErrMode(1); //Parent mode error
	__CV_BEGIN__;
	CV_CALL(image = in[0]->data);
	CV_CALL(w=(int)in[1]->vec[0]);  
	CV_CALL(h=(int)in[2]->vec[0]);
	dst= cvCreateImage(cvSize(w,h),image->depth,image->nChannels);
		
	if(in[0]->data){
		cvResize(image,dst,CV_INTER_LINEAR);
		out[0]->data= dst;
	}
	__CV_END__;
}


void register_node_type_cmp_cvresize(ListBase *lb)
{
	static bNodeType ntype;
	
	node_type_base(&ntype, CMP_NODE_CVRESIZE, "OpenCV - Resize", NODE_CLASS_OCV_IMAGEPROCESS, NODE_OPTIONS);
	node_type_socket_templates(&ntype,cmp_node_cvResize_in, cmp_node_cvResize_out);
	node_type_size(&ntype, 150, 80, 250);
	node_type_exec(&ntype, node_composit_exec_cvResize);
	
	nodeRegisterType(lb, &ntype);
}


