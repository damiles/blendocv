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

#define min(X,Y)  ((X) < (Y) ? (X) : (Y))
#define max(X,Y)  ((X) > (Y) ? (X) : (Y))


static bNodeSocketTemplate cmp_node_cvGravityCenter_in[]= {
	{	SOCK_OCV_IMAGE, 1, "cvImage",			1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{	-1, 0, ""	}
};
static bNodeSocketTemplate cmp_node_cvGravityCenter_out[]= {
	{	SOCK_OCV_IMAGE, 0, "cvImage",			0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
	{	-1, 0, ""	}
};

static void node_composit_exec_cvGravityCenter(void *data, bNode *node, bNodeStack **in, bNodeStack **out)
{
	IplImage* image,*dst;
//TODO: Use atach buffers
	CV_FUNCNAME( "cvGravityCenter" ); 
	if(out[0]->hasoutput==0) return;
	cvSetErrMode(1); //Parent mode error
	__CV_BEGIN__;
	CV_CALL(image = in[0]->data);
	if(image->nChannels>1)
		CV_ERROR(CV_BadNumChannels,"The number of channels of the source image isn't 1" );

	if(in[0]->data){
		int suma_x = 0, suma_y = 0, n = 0, i,j,cdg_x,cdg_y;
		CvScalar s;
		dst= cvCreateImage(cvSize(image->width,image->height),image->depth,4);
		CV_CALL(cvCvtColor( image, dst, CV_GRAY2BGR));
		for(i=1; i<image->height-1; i++)
			for(j=1; j<image->width-1; j++){
				s=cvGet2D(image,i,j);
				if(s.val[0]==255){
					suma_x+=i;
					suma_y+=j;
					n++;}
			}

		if(n!=0){
			cdg_x = suma_x / n; 
			cdg_y = suma_y / n;
		}else{
			cdg_x = 0; 
			cdg_y = 0;
		}

		s.val[0]=0;
		s.val[1]=0;
		s.val[2]=255;
		//i=max(cdg_x-10,0);
		//j=max(cdg_y-10,0);
		i=cdg_x-10;
		j=cdg_y-10;


		for(i; i<=(cdg_x+10); i++)
			//for(i; i<=min((cdg_x+10),(dst->width)); i++){
			CV_CALL(cvSet2D(dst,i,cdg_y,s));


		for(j; j<=(cdg_y+10); j++)
			//    for(j; j<=min((cdg_y+10),(dst->height)); j++){
			CV_CALL(cvSet2D(dst,cdg_x,j,s));

		CV_CALL(out[0]->data= dst);
	}
	__CV_END__;
	}

	void register_node_type_cmp_cvgravitycenter(ListBase *lb)
	{
		static bNodeType ntype;

		node_type_base(&ntype, CMP_NODE_CVGRAVITYCENTER, "Gravity Center", NODE_CLASS_OCV_DRAW, NODE_OPTIONS);
		node_type_socket_templates(&ntype,cmp_node_cvGravityCenter_in, cmp_node_cvGravityCenter_out);
		node_type_size(&ntype, 150, 80, 250);
		node_type_exec(&ntype, node_composit_exec_cvGravityCenter);

		nodeRegisterType(lb, &ntype);
	}
