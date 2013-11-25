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




static bNodeSocketTemplate cmp_node_cvSub_in[]= {
	{	SOCK_OCV_ARR, 1, "CvArr",			1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{	SOCK_OCV_ARR, 1, "CvArr",			1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{	SOCK_OCV_ARR, 1, "Mask",			1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{	-1, 0, ""	}
};
static bNodeSocketTemplate cmp_node_cvSub_out[]= {
	{	SOCK_OCV_ARR, 0, "CvArr",			0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
	{	-1, 0, ""	}
};

static void node_composit_exec_cvSub(void *data, bNode *node, bNodeStack **in, bNodeStack **out)
{
    //TODO: Use atach buffers
	CvArr* dst;
	CvArr* src1;
	CvArr* src2;
	CvArr* mask;
	CV_FUNCNAME( "cvSub" ); 
	if(out[0]->hasoutput==0) return;
	cvSetErrMode(1); //Parent mode error
	__CV_BEGIN__;
	if((in[0]->data)&&(in[1]->data)){
		CV_CALL(src1 = in[0]->data);
		CV_CALL(src2 = in[1]->data);
		if(!BOCV_checkAreSameType(src1, src2))
		         CV_ERROR( CV_StsBadArg,"The source inputs are differents" );
			
		CV_CALL(mask = in[2]->data);
		CV_CALL(dst=BOCV_CreateArrFrom(src1));
		if(dst)		
		{
			CV_CALL(cvSub(src1, src2, dst, mask));
		 	CV_CALL(out[0]->data= dst);
		}
	}
	__CV_END__;
}



void register_node_type_cmp_cvsub(ListBase *lb)
{
	static bNodeType ntype;
	
	node_type_base(&ntype, CMP_NODE_CVSUB, "OpenCV - Sub", NODE_CLASS_OCV_ARRAY, NODE_OPTIONS);
	node_type_socket_templates(&ntype,cmp_node_cvSub_in, cmp_node_cvSub_out);
	node_type_size(&ntype, 150, 80, 250);
	node_type_exec(&ntype, node_composit_exec_cvSub);
	
	nodeRegisterType(lb, &ntype);
}

