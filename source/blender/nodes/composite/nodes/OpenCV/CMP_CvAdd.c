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
//#include <cxerror.h>



static bNodeSocketTemplate cmp_node_cvAdd_in[] = {
    { SOCK_RGBA, 1, "CvArr", 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    { SOCK_RGBA, 1, "CvArr", 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    { SOCK_RGBA, 1, "Mask", 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    { -1, 0, ""}
};
static bNodeSocketTemplate cmp_node_cvAdd_out[] = {
    { SOCK_RGBA, 0, "CvArr", 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
    { -1, 0, ""}
};

static void node_composit_exec_cvAdd(void *data, bNode *node, bNodeStack **in, bNodeStack **out) {
    CvArr* dst;
    CvArr* src1;
    CvArr* src2;
    CvArr* mask = NULL;
    CompBuf *dst_buf;

    if (out[0]->hasoutput == 0) return;
    if ((in[0]->data)) {
        src1 = BOCV_IplImage_attach(in[0]->data);
        
        if (in[2]->data){
            mask = BOCV_Mask_attach(in[2]->data);
            if (!BOCV_checkMask(src1, mask)){
                node->error= 1;
                return;
            }
        }
        
        //Create output
        dst_buf = dupalloc_compbuf(in[0]->data);
        dst = BOCV_IplImage_attach(dst_buf);

        
        //If there are second input
        if(in[1]->data){
            src2 = BOCV_IplImage_attach(in[1]->data);
            if (!BOCV_checkAreSameType(src1, src2)){
                node->error= 1;
                return;
            }
            if(!BOCV_checkSameNChannels(src1, src2)){
                node->error= 1;
                return;
            }
            cvAdd(src1, src2, dst, mask);
            BOCV_IplImage_detach(src2);
        }else{
            CvScalar s;
            s.val[0]= (in[1]->vec[0]);
            s.val[1]= (in[1]->vec[1]);
            s.val[2]= (in[1]->vec[2]);
            s.val[3]= 0;
            cvAddS(src1, s, dst, mask);
        }
        
        out[0]->data = dst_buf;
        BOCV_IplImage_detach(src1);
        BOCV_IplImage_detach(dst);
    }

}

void register_node_type_cmp_cvadd(ListBase *lb) {
    static bNodeType ntype;

    node_type_base(&ntype, CMP_NODE_CVADDS, "Add", NODE_CLASS_OCV_ARRAY, NODE_OPTIONS);
    node_type_socket_templates(&ntype, cmp_node_cvAdd_in, cmp_node_cvAdd_out);

    node_type_size(&ntype, 150, 80, 250);
    node_type_exec(&ntype, node_composit_exec_cvAdd);

    nodeRegisterType(lb, &ntype);
}

