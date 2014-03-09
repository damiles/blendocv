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


static bNodeSocketTemplate cmp_node_cvDilate_in[] = {
    { SOCK_RGBA, 1, "cvImage", 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    { SOCK_INT, 1, "Iterations", 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 15.0f},
    { SOCK_FLOAT, 1, "Structuring element", 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 15.0f},
    { -1, 0, ""}
};
static bNodeSocketTemplate cmp_node_cvDilate_out[] = {
    { SOCK_RGBA, 0, "cvImage", 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
    { -1, 0, ""}
};

static void node_composit_exec_cvDilate(void *data, bNode *node, bNodeStack **in, bNodeStack **out) {
    int iterations;
    IplImage *image, *dst;
    IplConvKernel *kernel;
    CompBuf* dst_buf;
    int *valuesKernel, i;
    
    if (out[0]->hasoutput == 0) return;

    if (in[0]->data) {

        image = BOCV_IplImage_attach(in[0]->data);
        dst_buf = alloc_compbuf(image->width, image->height, image->nChannels, 1);
        dst = BOCV_IplImage_attach(dst_buf);
        iterations = (int) in[1]->vec[0];
        if (in[2]->data) {
                CompBuf* kernel_buf= in[2]->data;
                valuesKernel= malloc(kernel_buf->x*kernel_buf->y * sizeof(int));
                //Convert kernel to float array buf to send via socket
                for(i=0; i< kernel_buf->x*kernel_buf->y; i++){
                    valuesKernel[i]= (int)kernel_buf->rect[i];
                }
                kernel= cvCreateStructuringElementEx(
                        kernel_buf->x, 
                        kernel_buf->y, 
                        kernel_buf->x/2, 
                        kernel_buf->y/2, 
                        CV_SHAPE_CUSTOM, 
                        valuesKernel );
                cvDilate(image, dst, kernel, iterations);
        }else
            cvDilate(image, dst, 0, iterations);
        out[0]->data = dst_buf;
        
        BOCV_IplImage_detach(dst);
        BOCV_IplImage_detach(image);
    }

}

void register_node_type_cmp_cvdilate(ListBase *lb) {
    static bNodeType ntype;

    node_type_base(&ntype, CMP_NODE_CVDILATE, "OpenCV - Dilate", NODE_CLASS_OCV_IMAGEPROCESS, NODE_OPTIONS);
    node_type_socket_templates(&ntype, cmp_node_cvDilate_in, cmp_node_cvDilate_out);
    node_type_size(&ntype, 150, 80, 250);
    node_type_exec(&ntype, node_composit_exec_cvDilate);

    nodeRegisterType(lb, &ntype);
}


