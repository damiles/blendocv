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


static bNodeSocketTemplate cmp_node_cvSobel_in[] = {
    { SOCK_OCV_IMAGE, 1, "cvImage", 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    { SOCK_INT, 1, "X order", 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f},
    { SOCK_INT, 1, "Y order", 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f},
    { -1, 0, ""}
};
static bNodeSocketTemplate cmp_node_cvSobel_out[] = {
    { SOCK_OCV_IMAGE, 0, "cvImage", 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
    { -1, 0, ""}
};

static void node_composit_exec_cvSobel(void *data, bNode *node, bNodeStack **in, bNodeStack **out) {
    //TODO: Use atach buffers
    int x, y, aperture;
    IplImage *image, *sobel_img, *img_grey;

    if (out[0]->hasoutput == 0) return;

    if (in[0]->data) {
        x = (int) in[1]->vec[0];
        y = (int) in[2]->vec[0];
        switch (node->custom1) {
            case 0:
                aperture = 3;
                break;
            case 1:
                aperture = 1;
                break;
            case 2:
                aperture = 5;
                break;
            case 3:
                aperture = 7;
                break;
           
        }

        image = BOCV_Socket_IplImage(in[0]);
        if (image == NULL)//Check if there are image input
            return;
        sobel_img= cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,image->nChannels);
        cvSobel(image, sobel_img, 1,1,3);
        out[0]->data = sobel_img;
        
        //generate_preview(data, node, sobel_img);
    }

}

void register_node_type_cmp_cvsobel(ListBase *lb) {
    static bNodeType ntype;

    node_type_base(&ntype, CMP_NODE_CVSOBEL, "OpenCV - Sobel", NODE_CLASS_OCV_IMAGEPROCESS, NODE_PREVIEW|NODE_OPTIONS);
    node_type_socket_templates(&ntype, cmp_node_cvSobel_in, cmp_node_cvSobel_out);
    node_type_size(&ntype, 150, 80, 250);
    node_type_exec(&ntype, node_composit_exec_cvSobel);

    nodeRegisterType(lb, &ntype);
}



