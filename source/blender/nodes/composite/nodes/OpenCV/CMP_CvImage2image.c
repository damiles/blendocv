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


static bNodeSocketTemplate cmp_node_cvImage2Image_in[]= {
	{	SOCK_OCV_IMAGE, 1, "cvImage",			1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{	-1, 0, ""	}
};
static bNodeSocketTemplate cmp_node_cvImage2Image_out[]= {
	{	SOCK_RGBA, 0, "Image",			0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
	{	-1, 0, ""	}
};

static void node_composit_exec_cvImage2Image(void *data, bNode *node, bNodeStack **in, bNodeStack **out)
{
    //TODO: Use atach buffers
	int x,y,w,h;
	uchar *pixelIn;
	float *pixelOut;
	IplImage *img;
	CompBuf *stackbuf;

	CV_FUNCNAME( "cvImage2image" ); 
	if(out[0]->hasoutput==0) return;
	cvSetErrMode(1); //Parent mode error
	__CV_BEGIN__;
	if(in[0]->data){
		
		if(!CV_IS_IMAGE(in[0]->data))
			CV_ERROR( CV_HeaderIsNull,"There's no input image" );
		
		img = in[0]->data;
		
		w=img->width;
		h=img->height;
		
		stackbuf= alloc_compbuf(w,h, CB_RGBA,1);	/* note, this returns zero'd image */
		
		
		for(y=0; y<h; y++){
				pixelIn = (uchar*)img->imageData + img->widthStep*(h-1-y);
				pixelOut = stackbuf->rect + 4*w*y;

			for (x=0; x<w; x++,pixelOut+=4, pixelIn+=img->nChannels) {
				if(img->nChannels==4){
					pixelOut[2] = (float)pixelIn[0]/255;
					pixelOut[1] = (float)pixelIn[1]/255;
					pixelOut[0] = (float)pixelIn[2]/255;
					pixelOut[3] = (float)pixelIn[3]/255;
				}else if(img->nChannels==1){
					pixelOut[2] = pixelOut[1] = pixelOut[0] = (float)pixelIn[0]/255;
					pixelOut[3] = 1.0f;
				}else if(img->nChannels==3){
					pixelOut[2] = (float)pixelIn[0]/255;
					pixelOut[1] = (float)pixelIn[1]/255;
					pixelOut[0] = (float)pixelIn[2]/255;
					pixelOut[3] = 1.0f;
				} 				
			}
		}
		
		out[0]->data= stackbuf;
	}
	__CV_END__;
}


void register_node_type_cmp_cvimage2image(ListBase *lb)
{
	static bNodeType ntype;
	
	node_type_base(&ntype, CMP_NODE_CVIMAGE2IMAGE, "OpenCV - CvImage to Image", NODE_CLASS_OCV_INPUT, NODE_OPTIONS);
	node_type_socket_templates(&ntype,cmp_node_cvImage2Image_in, cmp_node_cvImage2Image_out);
	node_type_size(&ntype, 150, 80, 250);
	node_type_exec(&ntype, node_composit_exec_cvImage2Image);
	
	nodeRegisterType(lb, &ntype);
}

