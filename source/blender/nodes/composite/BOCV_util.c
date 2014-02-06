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
int BOCV_checkSameNChannels(void* src1, void* src2){
    if(CV_IS_IMAGE(src1) && CV_IS_IMAGE(src2)){
        if(((IplImage*)src1)->nChannels != ((IplImage*)src2)->nChannels ){
                BKE_report(NULL, 1<<4,"Inputs have different num channels");
                return 0;	
        }
        return 1;
    }else{
        return 0;
    }
}
int BOCV_checkMask(void* src1, void* mask){
    if(!BOCV_checkAreSameType(src1, mask)){
        return 0; 
    }
    if(CV_IS_IMAGE(mask)){
            if(((IplImage*)mask)->nChannels!=1){
                BKE_report(NULL, 1<<4,"Mask must be 1 channel, please convert to gray");
                return 0;
            }
    }
    return 1;
}
int BOCV_checkAreSameType(void* src1,void* src2 )
{
	if(CV_IS_IMAGE(src1)){
		if(CV_IS_IMAGE(src2)){
			IplImage* isrc1=(IplImage*) src1;
			IplImage* isrc2=(IplImage*) src2;
			if(isrc1->width != isrc2->width){
                                BKE_report(NULL, 1<<4,"Inputs have different sizes");
                		return 0;
                        }
			if(isrc1->height != isrc2->height){
                                BKE_report(NULL, 1<<4,"Inputs have different sizes");
                                return 0;	
                        }
                        return 1;
		}else{
			return 0;
                        BKE_report(NULL, 1<<4,"Src2 is not an image");
		}
		
	}
        BKE_report(NULL, 1<<4,"Src1 is not an image");
	//Here need code to mat and sequence...
	return 0;
}

/**
 * Create IplImage from Comp Buf
 * @param cbuf
 * @return IplImage pointer
 */
IplImage* BOCV_IplImage_attach(CompBuf* cbuf)
{
    if(cbuf->x>0 && cbuf->y>0){
    	IplImage *img = cvCreateImageHeader(cvSize(cbuf->x,cbuf->y),IPL_DEPTH_32F,cbuf->type);
	cvSetData(img,cbuf->rect,cbuf->x * cbuf->type * sizeof(float)); // always 4 byte align.
        return img;
    }else{
        return NULL;
    }
	
}

/**
 * Create Mask from comp buf node
 * @param cbuf
 * @return IplImage of Mask
 */
IplImage* BOCV_Mask_attach(CompBuf* cbuf)
{
    IplImage *mask;
    IplImage *img;
    if(cbuf == NULL)
        return NULL;
    if(cbuf->x>0 && cbuf->y>0 ){
        //Create image from comp buf
    	img = cvCreateImageHeader(cvSize(cbuf->x,cbuf->y),IPL_DEPTH_32F,cbuf->type);
	cvSetData(img,cbuf->rect,cbuf->x * cbuf->type * sizeof(float)); // always 4 byte align.
        
        mask= cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, cbuf->type);
        //Convert to 8 bit unsigned
        cvConvertScale(img, mask,1,0);
            
        return mask;
    }else{
        return NULL;
    }
	
}

void BOCV_IplImage_detach(IplImage *img)
{
	cvReleaseImageHeader(&img);
}

