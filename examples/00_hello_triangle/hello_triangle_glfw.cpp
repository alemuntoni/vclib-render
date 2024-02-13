/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "hello_triangle_glfw.h"

#include "common.h"

HelloTriangleGLFW::HelloTriangleGLFW() :
        vcl::bglfwx::CanvasWindow("Hello Triangle GLFW")
{
    setUpBGFX(viewId(), vbh, program);
}

HelloTriangleGLFW::~HelloTriangleGLFW()
{
    bgfx::destroy(vbh);
    bgfx::destroy(program);
}

void HelloTriangleGLFW::draw()
{
    drawOnView(viewId(), vbh, program);
}
