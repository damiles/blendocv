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

static bNodeSocketTemplate cmp_node_cvCvtColor_in[]= {
	{	SOCK_RGBA, 1, "cvImage",			1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{	-1, 0, ""	}
};
static bNodeSocketTemplate cmp_node_cvCvtColor_out[]= {
	{	SOCK_RGBA, 0, "cvImage",			0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
	{	-1, 0, ""	}
};

static void node_composit_exec_cvCvtColor(void *data, bNode *node, bNodeStack **in, bNodeStack **out)
{
	int w,h;
	CompBuf *output;
        
	if(in[0]->data){
		IplImage *img, *cvt;	
		img=BOCV_IplImage_attach(in[0]->data);
		if(img==NULL)//Check if there are image input
                    return;
                
		w=img->width;
		h=img->height;
		
		if( node->custom1!=8 && 
                    node->custom1 != 46 &&
                    node->custom1 != 47 &&
                    node->custom1 != 48 &&
                    node->custom1 != 49 &&
                    img->nChannels < 3){
                    
                    node->error= 1;
                    BOCV_report("Input image must be 3 or 4 channels");
                    return;
                }
                if( (node->custom1 ==8 || 
                    node->custom1  == 46 ||
                    node->custom1  == 47 ||
                    node->custom1  == 48 ||
                    node->custom1  == 49) && 
                    img->nChannels!=1){
                    node->error= 1;
                    BOCV_report("Input image must be 1 channel for GRAY -> RGB");
                    return;
                }
                if(node->custom1==7)
                    output= alloc_compbuf(w,h, 1, 1);
                else
                    output= alloc_compbuf(w,h, 3, 1);
                
                cvt = BOCV_IplImage_attach(output);
                
                if(node->custom1>=46) //Bayer nodes
                {
                    IplImage *s8boutput, *s8binput;
                    //Convert 8-bit
                    s8binput = cvCreateImage(cvSize(w,h), IPL_DEPTH_8U, 1);
                    cvConvertScale(img, s8binput,255,0);

                    s8boutput = cvCreateImage(cvSize(w,h), IPL_DEPTH_8U, 3);
                    
                    cvCvtColor(s8binput, s8boutput, node->custom1);
                    
                    //Convert 8bit output to float because blender only support float sockets
                    cvConvertScale(s8boutput, cvt,1.0/255.0,0);
                    
                }else{
                        cvCvtColor(img, cvt, node->custom1);
                }
		generate_preview(data, node, output);
                
                if(out[0]->hasoutput==0) return;
		out[0]->data= output;
	}
}

static void node_composit_init_cvCvtColor(bNodeTree *UNUSED(ntree), bNode* node, bNodeTemplate *UNUSED(ntemp))
{
	node->custom1= 7;
}

void register_node_type_cmp_cvcvtcolor(ListBase *lb)
{
	static bNodeType ntype;
	
	node_type_base(&ntype, CMP_NODE_CVCVTCOLOR, "OpenCV - Convert color", NODE_CLASS_OCV_IMAGEPROCESS, NODE_PREVIEW|NODE_OPTIONS);
	node_type_socket_templates(&ntype,cmp_node_cvCvtColor_in, cmp_node_cvCvtColor_out);
	node_type_size(&ntype, 150, 80, 250);
        node_type_init(&ntype, node_composit_init_cvCvtColor);
	node_type_exec(&ntype, node_composit_exec_cvCvtColor);
	
	nodeRegisterType(lb, &ntype);
}

