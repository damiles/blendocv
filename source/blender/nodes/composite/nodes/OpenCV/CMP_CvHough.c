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
#include <math.h>
#include <stdio.h>



static bNodeSocketTemplate cmp_node_cvHough_in[]= {
	{	SOCK_OCV_IMAGE, 1, "cvImage",			1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{	-1, 0, ""	}
};
static bNodeSocketTemplate cmp_node_cvHough_out[]= {
	{	SOCK_OCV_IMAGE, 0, "cvImage",			0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
	{	-1, 0, ""	}
};

static void node_composit_exec_cvHough(void *data, bNode *node, bNodeStack **in, bNodeStack **out)
{
    //TODO: Use atach buffers
	int i,option;
	IplImage *src,*dst, *grey, *color_dst;
	CvMemStorage* storage;
	CvSeq* lines;
	
	CV_FUNCNAME( "cvHough" ); 
	if(out[0]->hasoutput==0) return;
	cvSetErrMode(1); //Parent mode error
	__CV_BEGIN__;
	if(in[0]->data){
	CV_CALL(src=in[0]->data);
	CV_CALL(option=(int)in[1]->vec[0]);
	dst = cvCreateImage( cvGetSize(src), 8, 1 );
	grey = cvCreateImage( cvGetSize(src), 8, 1 );
        color_dst = cvCreateImage( cvGetSize(src), 8, 4 );
        storage = cvCreateMemStorage(0);
        lines = 0;
	CV_CALL(cvCvtColor(src, grey, CV_BGR2GRAY ));
        CV_CALL(cvCanny( grey, dst, 50, 200, 3 ));
        CV_CALL(cvCvtColor( dst, color_dst, CV_GRAY2BGR ));
	switch(node->custom1){
		case 0:
		  CV_CALL(option=0);
		  break;
		case 1:
		  CV_CALL(option=1);
		  break;
	}
	switch(option){
	case 0:
	CV_CALL(lines = cvHoughLines2( dst,storage,CV_HOUGH_STANDARD,1,CV_PI/180,100,0,0));
        for( i = 0; i < MIN(lines->total,100); i++ ){
            float* line = (float*)cvGetSeqElem(lines,i);
	    CvPoint pt1, pt2;
            float rho = line[0];
            float theta = line[1];
            double a = cos(theta); 
	    double b = sin(theta);
            double x0 = a*rho;
	    double y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            CV_CALL(cvLine( color_dst, pt1, pt2, CV_RGB(255,0,0), 3,CV_AA,0 ));}
	    break;
	case 1:
	lines = cvHoughLines2( dst,storage,CV_HOUGH_PROBABILISTIC,1,CV_PI/180,50,50,10 );
        for( i = 0; i < lines->total; i++ ){
            CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
            cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, CV_AA,0 );}
	break;  
	}
        CV_CALL(out[0]->data= color_dst);
	}
	__CV_END__;
}

void register_node_type_cmp_cvhough(ListBase *lb)
{
	static bNodeType ntype;
	
	node_type_base(&ntype, CMP_NODE_CVHOUGH, "Hough", NODE_CLASS_OCV_IMAGEPROCESS, NODE_OPTIONS);
	node_type_socket_templates(&ntype,cmp_node_cvHough_in, cmp_node_cvHough_out);
	node_type_size(&ntype, 150, 80, 250);
	node_type_exec(&ntype, node_composit_exec_cvHough);
	
	nodeRegisterType(lb, &ntype);
}

