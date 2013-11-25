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


static bNodeSocketTemplate cmp_node_cvSmooth_in[]= {
	{	SOCK_OCV_IMAGE, 1, "cvImage",			1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{	SOCK_FLOAT, 1, "Param1",			3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 100.0f},
	{	SOCK_FLOAT, 1, "Param2",			3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 100.0f},
	{	SOCK_FLOAT, 1, "Param3",			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 100.0f},
	{	SOCK_FLOAT, 1, "Param4",			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 100.0f},
	{	-1, 0, ""	}
};
static bNodeSocketTemplate cmp_node_cvSmooth_out[]= {
	{	SOCK_OCV_IMAGE, 0, "cvImage",			0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
	{	-1, 0, ""	}
};

static void node_composit_exec_cvSmooth(void *data, bNode *node, bNodeStack **in, bNodeStack **out)
{
    //TODO: Use atach buffers
	int w,h;
	int p1, p2;
	float p3,p4;
	int type;
	CV_FUNCNAME( "cvSmooth" ); 
	if(out[0]->hasoutput==0) return;
	cvSetErrMode(1); //Parent mode error
	__CV_BEGIN__;
	if(in[0]->data){
		IplImage *img, *smth;
		CV_CALL(img = in[0]->data);
		w=img->width;
		h=img->height;
		CV_CALL(p1= (int)in[1]->vec[0]);
		if((p1%2)==0){
			p1--;
		}
		CV_CALL(p2= (int)in[2]->vec[0]);
		if((p2%2)==0)
			p2--;
		CV_CALL(p3= in[3]->vec[0]);
		CV_CALL(p4= in[4]->vec[0]);
		
		smth = cvCreateImage(cvSize(w,h), img->depth, img->nChannels);
		
	
		switch(node->custom1) {
			case 0:
				type=CV_BLUR_NO_SCALE;
				break;
			case 1:
				type=CV_BLUR;
				break;
			case 2:
				type=CV_GAUSSIAN;
				break;
			case 3://Must change image to 8U with 1 3 or 4 channels
				type=CV_MEDIAN;
				break;
			case 4://Must change image to 8U with 1 or 3 channels
				type=CV_BILATERAL;
				break;
		}
	
		CV_CALL(cvSmooth(img, smth, type, p1, p2, p3, p4 ));
		
		CV_CALL(out[0]->data= smth);
	}
	__CV_END__;
}


void register_node_type_cmp_cvsmooth(ListBase *lb)
{
	static bNodeType ntype;
	
	node_type_base(&ntype, CMP_NODE_CVSMOOTH, "OpenCV - Smooth", NODE_CLASS_OCV_IMAGEPROCESS, NODE_OPTIONS);
	node_type_socket_templates(&ntype,cmp_node_cvSmooth_in, cmp_node_cvSmooth_out);
	node_type_size(&ntype, 150, 80, 250);
	node_type_exec(&ntype, node_composit_exec_cvSmooth);
	
	nodeRegisterType(lb, &ntype);
}

