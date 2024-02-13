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

#include <vclib/ext/bgfx/canvas.h>

#include <vclib/gui/native_window_handle.h>

namespace vcl::bgf {

Canvas::Canvas()
{
}

Canvas::Canvas(void* winId, uint width, uint height, void* displayId)
{
    init(winId, width, height, displayId);
}

Canvas::Canvas(void* winID, uint width, uint height)
{
    init(winID, width, height, nullptr);
}

Canvas::~Canvas()
{
    if (bgfx::isValid(fbh))
        bgfx::destroy(fbh);
    Context::releaseViewId(view);
}

void Canvas::init(void* winId, uint width, uint height, void* displayId)
{
    this->winID = winId;

    view = Context::requestViewId();

    fbh = bgfx::createFrameBuffer(winId, width, height);

    bgfx::setViewFrameBuffer(view, fbh);
    bgfx::setViewClear(
        view, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xffffffff, 1.0f, 0);
    bgfx::setViewRect(view, 0, 0, width, height);
    bgfx::touch(view);
}

void Canvas::init(void* winId, uint width, uint height)
{
    init(winId, width, height, nullptr);
}

void Canvas::screenShot(const std::string& filename, uint width, uint height)
{
    if (width == 0 || height == 0) {
        draw();
        bgfx::requestScreenShot(fbh, filename.c_str());
        bgfx::frame();
    }
    else {
        void* d;
        void* w = vcl::createWindow("", width, height, d, true);

        // setup view and frame buffer
        bgfx::ViewId            v   = Context::requestViewId();
        bgfx::FrameBufferHandle fbh = bgfx::createFrameBuffer(w, width, height);
        bgfx::setViewFrameBuffer(v, fbh);
        bgfx::setViewClear(
            v, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xffffffff, 1.0f, 0);
        bgfx::setViewRect(v, 0, 0, width, height);
        bgfx::touch(v);

        // replace the current view with the new one
        bgfx::ViewId tmpView = view;
        view                 = v;
        draw();
        bgfx::requestScreenShot(fbh, filename.c_str());
        bgfx::frame();

        // restore the previous view and release the resources
        view = tmpView;
        bgfx::destroy(fbh);
        Context::releaseViewId(v);
        vcl::closeWindow(w, d);
    }
}

void Canvas::frame()
{
    bgfx::setViewFrameBuffer(view, fbh);
    // bgfx::touch(view);
    draw();
    bgfx::frame();
}

void Canvas::resize(uint width, uint height)
{
    if (bgfx::isValid(fbh))
        bgfx::destroy(fbh);

    fbh = bgfx::createFrameBuffer(winID, width, height);
    bgfx::setViewFrameBuffer(view, fbh);
    bgfx::setViewRect(view, 0, 0, width, height);
    bgfx::touch(view);
}

} // namespace vcl::bgf
