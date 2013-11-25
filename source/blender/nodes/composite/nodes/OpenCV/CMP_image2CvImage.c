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

/* **************** image2CvImage ******************** */
static bNodeSocketTemplate cmp_node_image2cvImage_in[]= {
	{	SOCK_RGBA, 1, "image",			1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{	-1, 0, ""	}
};
static bNodeSocketTemplate cmp_node_image2cvImage_out[]= {
	{	SOCK_OCV_IMAGE, 0, "cvImage",			0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
	{	-1, 0, ""	}
};

static void node_composit_exec_image2cvImage(void *data, bNode *node, bNodeStack **in, bNodeStack **out)
{
    //TODO: Use atach buffers
	int w,h,x,y;
	float *pixelIn;
	uchar *pixelOut;
	
	CV_FUNCNAME( "image2CvImage" ); 
	if(out[0]->hasoutput==0) return;
	cvSetErrMode(1); //Parent mode error
	__CV_BEGIN__;
	if(in[0]->data){
		CompBuf *cbuf= typecheck_compbuf(in[0]->data, CB_RGBA);
		IplImage *img;
		w=cbuf->x;
		h=cbuf->y;

		img= cvCreateImage(cvSize(w,h),IPL_DEPTH_8U,4);
		
			
		for(y=0; y<h; y++){
			pixelIn = cbuf->rect + 4*w*y;
			pixelOut = (uchar*)(img->imageData + img->widthStep*(h-1-y));
			for (x=0; x<w; x++, pixelIn+=4, pixelOut+=4) {
				pixelOut[2] = (uchar)(pixelIn[0]*255);
				pixelOut[1] = (uchar)(pixelIn[1]*255);
				pixelOut[0] = (uchar)(pixelIn[2]*255);
				pixelOut[3] = (uchar)(pixelIn[3]*255);
			}
		}
		
		//img->imageData= cbuf->rect;

		/*Only for test*/
		// Create a window
		//cvNamedWindow("test", 1);
		//cvShowImage("test", img);
		//cvWaitKey(0);
		/*End test*/
		

		CV_CALL(out[0]->data= img);
	}
	__CV_END__;
}


void register_node_type_cmp_image2cvimage(ListBase *lb)
{
	static bNodeType ntype;
	
	node_type_base(&ntype, CMP_NODE_IMAGE2CVIMAGE, "OpenCV - Image to CvImage", NODE_CLASS_OCV_OUTPUT, NODE_OPTIONS);
	node_type_socket_templates(&ntype,cmp_node_image2cvImage_in, cmp_node_image2cvImage_out);
	node_type_size(&ntype, 150, 80, 250);
	node_type_exec(&ntype, node_composit_exec_image2cvImage);
	
	nodeRegisterType(lb, &ntype);
}


