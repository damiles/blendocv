/**
 * $Id: CMP_util.c 14352 2008-04-07 15:21:25Z blendix $
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

#include "BOCV_util.h"


CvArr* BOCV_CreateArrFrom(void* src)
{
	if(CV_IS_IMAGE(src)){
		IplImage* isrc=(IplImage*) src;
		return (CvArr*)cvCreateImage(cvSize(isrc->width,isrc->height), isrc->depth, isrc->nChannels);		 
	}
	//Here need code to mat and sequence...
	return NULL;
}

int BOCV_checkAreSameType(void* src1,void* src2 )
{
	if(CV_IS_IMAGE(src1)){
		if(CV_IS_IMAGE(src2)){
			IplImage* isrc1=(IplImage*) src1;
			IplImage* isrc2=(IplImage*) src2;
			if(isrc1->width != isrc2->width)
				return 0;
			if(isrc1->height != isrc2->height)
				return 0;	
			return 1;
		}else{
			return 0;
		}
		
	}
	//Here need code to mat and sequence...
	return 0;
}

IplImage* BOCV_IplImage_attach(CompBuf* cbuf)
{
    	IplImage *img = cvCreateImageHeader(cvSize(cbuf->x,cbuf->y),IPL_DEPTH_32F,cbuf->type);
	cvSetData(img,cbuf->rect,cbuf->x * cbuf->type * sizeof(float)); // always 4 byte align.
	return img;
}

void BOCV_IplImage_detach(IplImage *img)
{
	cvReleaseImageHeader(&img);
}

IplImage* BOCV_Socket_IplImage(bNodeStack *in){
    
    //Check for Socket type
    if(in->sockettype==7){ //IplImage
        if(!CV_IS_IMAGE(in->data)) //Check for image in
			return NULL;
        return in->data;
    }else if(in->sockettype==2){ //Compbuf RGBA image from blender
    
        int w,h,x,y;
        float *pixelIn;
        uchar *pixelOut;
        CompBuf *cbuf= typecheck_compbuf(in->data, CB_RGBA);
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
        
        return img;
    }
    return NULL;
}