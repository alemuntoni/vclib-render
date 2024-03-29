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
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#include <vclib/ext/glfw/bgfx/minimal_viewer_window.h>

#include <vclib/ext/glfw/gui/input.h>

namespace vcl::bglfwx {

MinimalViewerWindow::MinimalViewerWindow(
    std::shared_ptr<DrawableObjectVector> v,
    const std::string&                    windowTitle,
    uint                                  width,
    uint                                  height) :
        CanvasWindow(windowTitle, width, height),
        MinimalViewer(v, width, height)
{
}

MinimalViewerWindow::MinimalViewerWindow(
    const std::string& windowTitle,
    uint               width,
    uint               height) :
        MinimalViewerWindow(
            std::make_shared<DrawableObjectVector>(),
            windowTitle,
            width,
            height)
{
}

MinimalViewerWindow::MinimalViewerWindow(uint width, uint height) :
        MinimalViewerWindow(
            std::make_shared<DrawableObjectVector>(),
            "Minimal Viewer",
            width,
            height)
{
}

void MinimalViewerWindow::draw()
{
    MV::draw(viewId());
}

void MinimalViewerWindow::onResize(unsigned int width, unsigned int height)
{
    MV::resizeViewer(width, height);
}

void MinimalViewerWindow::onKeyPress(Key::Enum key)
{
    MV::setKeyModifiers(modifiers());

    if (modifiers()[KeyModifier::CONTROL] && modifiers()[KeyModifier::SHIFT]) {
        MV::setDirectionalLightVisibility(true);
    }

    switch (key) {
    case Key::C:
        std::cerr << "(" << MV::camera().eye() << ") "
                  << "(" << MV::camera().center() << ") "
                  << "(" << MV::camera().up() << ")\n";
        break;

    case Key::A: MV::toggleAxisVisibility(); break;

    default:
        MV::keyPress(key);
        bgfx::setViewTransform(
            viewId(), MV::viewMatrix().data(), MV::projectionMatrix().data());
        break;
    }

    MV::updateDirectionalLight();
}

void MinimalViewerWindow::onKeyRelease(Key::Enum key)
{
    if (isDirectionalLightVisible()) {
        if (!modifiers()[KeyModifier::CONTROL] ||
            !modifiers()[KeyModifier::SHIFT])
        {
            MV::setDirectionalLightVisibility(false);
        }
    }

    MV::setKeyModifiers(modifiers());
}

void MinimalViewerWindow::onMouseMove(double x, double y)
{
    MV::moveMouse(x, y);
    MV::updateDirectionalLight();

    bgfx::setViewTransform(
        viewId(), MV::viewMatrix().data(), MV::projectionMatrix().data());
}

void MinimalViewerWindow::onMousePress(MouseButton::Enum button)
{
    MV::pressMouse(button);
}

void MinimalViewerWindow::onMouseRelease(MouseButton::Enum button)
{
    MV::releaseMouse(button);
}

void MinimalViewerWindow::onMouseScroll(double dx, double dy)
{
    const int WHEEL_STEP = 120;
    float     notchY     = dy / float(WHEEL_STEP);

    MV::wheelMouse(notchY > 0);

    bgfx::setViewTransform(
        viewId(), MV::viewMatrix().data(), MV::projectionMatrix().data());
}

} // namespace vcl::bglfwx
