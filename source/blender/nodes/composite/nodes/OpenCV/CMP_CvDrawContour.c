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



static bNodeSocketTemplate cmp_node_cvDrawContour_in[]= {
	{	SOCK_OCV_IMAGE, 1, "cvImage",			1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{	SOCK_OCV_SEQ, 1, "cvSeq",			1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{	-1, 0, ""	}
};
static bNodeSocketTemplate cmp_node_cvDrawContour_out[]= {
	{	SOCK_OCV_IMAGE, 0, "cvImage",			0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
	{	-1, 0, ""	}
};

static void node_composit_exec_cvDrawContour(void *data, bNode *node, bNodeStack **in, bNodeStack **out)
{
	IplImage *img, *dst, *img1, *img2, *img3,*imgRed, *umbral;
        CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* contour = 0;
//TODO: Use atach buffers
	if(out[0]->hasoutput==0) return;
	
	img=in[0]->data;
	dst = cvCreateImage( cvGetSize(img), 8, 3 );
	img1=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	img2=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	img3=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	imgRed=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	umbral=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);

	cvSplit(img, img1, img2, imgRed, img3);        
	cvThreshold( umbral,imgRed,210,255, CV_THRESH_BINARY );
        
        cvFindContours( img, storage, &contour, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE,cvPoint(0, 0) );
        cvZero( dst );

        for( ; contour != 0; contour = contour->h_next )
        {
            CvScalar color = CV_RGB( rand()&255, rand()&255, rand()&255 );
            /* replace CV_FILLED with 1 to see the outlines */
            cvDrawContours( dst, contour, color, color, -1, CV_FILLED, 8, cvPoint(0,0) );
        }
	    out[0]->data= dst;
	
	/*CvSeq* contour = in[1]->data;
	if(in[0]->data && in[1]->data){
        	IplImage* dst = cvCreateImage( cvGetSize(image), 8, 3 );
	  	cvZero(dst);
            
		//cvDrawContours( dst, contour, CV_RGB(255,0,0),CV_RGB(0,255,0), -1,3, CV_AA,cvPoint(0,0));
		CvSeq* c=contour;
		for( ; c != 0; c = c->h_next ) 
        	{ 
            		CvScalar color = CV_RGB( rand()&255, rand()&255, rand()&255 ); 
            		cvDrawContours( dst, c, color, color, -1, 1, 8 ,cvPoint(0,0)); 
        	}
	    	out[0]->data= dst;
	}*/
}

void register_node_type_cmp_cvdrawcontour(ListBase *lb)
{
	static bNodeType ntype;
	
	node_type_base(&ntype, CMP_NODE_CVDRAWCONTOUR, "Draw Contour", NODE_CLASS_OCV_DRAW, NODE_OPTIONS);
	node_type_socket_templates(&ntype,cmp_node_cvDrawContour_in, cmp_node_cvDrawContour_out);
	node_type_size(&ntype, 150, 80, 250);
	node_type_exec(&ntype, node_composit_exec_cvDrawContour);
	
	nodeRegisterType(lb, &ntype);
}
