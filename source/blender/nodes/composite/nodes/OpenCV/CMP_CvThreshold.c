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


static bNodeSocketTemplate cmp_node_cvThreshold_in[]= {
	{	SOCK_RGBA, 1, "cvImage",			1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{	SOCK_FLOAT, 1, "Threshold",			0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{	SOCK_FLOAT, 1, "Max Value",			1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 30.0f},
	{	-1, 0, ""	}
};
static bNodeSocketTemplate cmp_node_cvThreshold_out[]= {
	{	SOCK_RGBA, 0, "cvImage",			0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
        {	SOCK_FLOAT, 0, "Outsu value",			1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
	{	-1, 0, ""	}
};

static void node_composit_exec_cvThreshold(void *data, bNode *node, bNodeStack **in, bNodeStack **out)
{
	float thresh, max_value;	
	IplImage *image, *threshold_img;
	CompBuf *output;
        IplImage *s8boutput, *s8binput;
        double outputOtsu=0.0;
        
        if(out[0]->hasoutput==0) return;
        
        if (in[0]->data) {
            
            image=BOCV_IplImage_attach(in[0]->data);

            if(image==NULL)//Check if there are image input
                return;

            output= alloc_compbuf(image->width,image->height, image->nChannels, 1);
            threshold_img = BOCV_IplImage_attach(output);

            //threshold_img= cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,image->nChannels);
            thresh=in[1]->vec[0];
            max_value=in[2]->vec[0];
            
            if(node->custom2!=0){
                //Only accept 1 channel image
                if(image->nChannels!=1){
                    node->error= 1;
                    return;
                }
                //We need to convert to 8U for OTSU
                //Convert 8-bit
                s8binput = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, image->nChannels);
                cvConvertScale(image, s8binput,255,0);

                s8boutput = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, image->nChannels);

                //Make Threshold
                outputOtsu=cvThreshold(s8binput,s8boutput,thresh,max_value,node->custom1 | CV_THRESH_OTSU);
                
                //Convert 8bit output to float because blender only support float sockets
                cvConvertScale(s8boutput, threshold_img,1,0);
                
            }else
                cvThreshold(image,threshold_img,thresh,max_value,node->custom1);

            generate_preview(data, node, output);

            BOCV_IplImage_detach(image);
            BOCV_IplImage_detach(threshold_img);

            
            
            out[0]->data= output;
            out[1]->vec[0]= outputOtsu/255.0;
        }

}

static void node_composit_init_cvthreshold(bNodeTree *UNUSED(ntree), bNode* node, bNodeTemplate *UNUSED(ntemp))
{
	node->custom1= 0;
        //For otsu check
        node->custom2= 0;
}

void register_node_type_cmp_cvthreshold(ListBase *lb)
{
	static bNodeType ntype;
	
	node_type_base(&ntype, CMP_NODE_CVTHRESHOLD, "OpenCV - Threshold", NODE_CLASS_OCV_IMAGEPROCESS, NODE_PREVIEW|NODE_OPTIONS);
	node_type_socket_templates(&ntype,cmp_node_cvThreshold_in, cmp_node_cvThreshold_out);
	node_type_size(&ntype, 150, 80, 250);
        node_type_init(&ntype, node_composit_init_cvthreshold);
	node_type_exec(&ntype, node_composit_exec_cvThreshold);
	
	nodeRegisterType(lb, &ntype);
}

