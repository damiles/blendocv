/**
 * $Id: CMP_GetStructuringElement.c 14749 2014-03-08 19:57:10Z damiles $
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
//#include <cxerror.h>



static bNodeSocketTemplate cmp_node_getStructuringElement_in[] = {
    { SOCK_INT, 0, "num Cols", 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 100.0f},
    { SOCK_INT, 0, "num Rows", 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 100.0f},
    { -1, 0, ""}
};
static bNodeSocketTemplate cmp_node_getStructuringElement_out[] = {
    { SOCK_FLOAT, 0, "Structuring kernel", 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
    { -1, 0, ""}
};

static void node_composit_exec_getStructuringElement(void *data, bNode *node, bNodeStack **in, bNodeStack **out) {
    CompBuf *dst_buf;
    IplConvKernel *kernel;
    int w,h,i;
    if (out[0]->hasoutput == 0) return;
    
    w= in[0]->vec[0];
    h= in[1]->vec[0];
    //Create dst_buf
    dst_buf= alloc_compbuf(w, h, CB_VAL, 1);
    
    //Create kernel
    kernel= cvCreateStructuringElementEx(w, h, w/2, h/2, node->custom1, NULL );
    
    //Convert kernel to float array buf to send via socket
    for(i=0; i< w*h; i++){
        dst_buf->rect[i]= (float)kernel->values[i];
    }
    cvReleaseStructuringElement(&kernel);
    //Send output
    out[0]->data = dst_buf;

}
static void node_composit_init_getStructuringElement(bNodeTree *UNUSED(ntree), bNode* node, bNodeTemplate *UNUSED(ntemp))
{
	node->custom1= 0;
}
void register_node_type_cmp_getStructuringElement(ListBase *lb) {
    static bNodeType ntype;

    node_type_base(&ntype, CMP_NODE_CVGETSTRUCTURINGELEMENT, "OpenCV - getStructuringElement", NODE_CLASS_OCV_INPUT, NODE_OPTIONS);
    node_type_socket_templates(&ntype, cmp_node_getStructuringElement_in, cmp_node_getStructuringElement_out);
    node_type_init(&ntype, node_composit_init_getStructuringElement);
    node_type_size(&ntype, 150, 80, 250);
    node_type_exec(&ntype, node_composit_exec_getStructuringElement);

    nodeRegisterType(lb, &ntype);
}

