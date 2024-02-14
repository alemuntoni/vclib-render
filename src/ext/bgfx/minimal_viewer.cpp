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

#include <vclib/ext/bgfx/minimal_viewer.h>

#include <vclib/math/min_max.h>
#include <vclib/render/interfaces/drawable_mesh_i.h>
#include <vclib/render/interfaces/pickable_object_i.h>
#include <vclib/space/box.h>

#include <vclib/ext/bgfx/context.h>

namespace vcl::bgf {

MinimalViewer::MinimalViewer(uint width, uint height) : DTB(width, height)
{
    cameraUniforms.updateCamera(DTB::camera());
    directionalLightUniforms.updateLight(DTB::light());
    axis.setShaderProgram(axisProgram);

    // setup picking
    // Set up ID buffer, which has a color target and depth buffer
    pickingRT = bgfx::createTexture2D(
        8,
        8,
        false,
        1,
        bgfx::TextureFormat::RGBA8,
        0 | BGFX_TEXTURE_RT | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT |
            BGFX_SAMPLER_MIP_POINT | BGFX_SAMPLER_U_CLAMP |
            BGFX_SAMPLER_V_CLAMP);

    pickingRTDepth = bgfx::createTexture2D(
        8,
        8,
        false,
        1,
        bgfx::TextureFormat::D32F,
        0 | BGFX_TEXTURE_RT | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT |
            BGFX_SAMPLER_MIP_POINT | BGFX_SAMPLER_U_CLAMP |
            BGFX_SAMPLER_V_CLAMP);

    // CPU texture for blitting to and reading ID buffer so we can see what was
    // clicked on. Impossible to read directly from a render target, you *must*
    // blit to a CPU texture first. Algorithm Overview: Render on GPU -> Blit to
    // CPU texture -> Read from CPU texture.
    blitTex = bgfx::createTexture2D(
        8,
        8,
        false,
        1,
        bgfx::TextureFormat::RGBA8,
        0 | BGFX_TEXTURE_BLIT_DST | BGFX_TEXTURE_READ_BACK |
            BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT |
            BGFX_SAMPLER_MIP_POINT | BGFX_SAMPLER_U_CLAMP |
            BGFX_SAMPLER_V_CLAMP);

    bgfx::TextureHandle rt[2] = {pickingRT, pickingRTDepth};
    pickingFB = bgfx::createFrameBuffer(2, rt, true);
}

MinimalViewer::MinimalViewer(
    std::shared_ptr<DrawableObjectVector> v,
    uint                                  width,
    uint                                  height) :
        MinimalViewer(width, height)
{
    setDrawableObjectVector(v);
}


MinimalViewer::~MinimalViewer()
{
    bgfx::destroy(pickingFB);
    bgfx::destroy(pickingRT);
    bgfx::destroy(pickingRTDepth);
    bgfx::destroy(blitTex);
}

std::shared_ptr<const DrawableObjectVector> MinimalViewer::
    drawableObjectVector() const
{
    return drawList;
}

void MinimalViewer::setDrawableObjectVector(
    std::shared_ptr<DrawableObjectVector> v)
{
    drawList = v;

    for (DrawableObjectI* obj : *drawList) {
        obj->init();

        DrawableMeshI* mesh = dynamic_cast<DrawableMeshI*>(obj);
        if (mesh) {
            mesh->setShaderProgram(meshProgram);
        }

        PickableObjectI* pickable = dynamic_cast<PickableObjectI*>(obj);
        if (pickable) {
            pickable->setPickingShaderPorgram(pickingProgram);
        }
    }
}

void MinimalViewer::fitScene()
{
    Box3d   bb          = drawList->boundingBox();
    Point3f sceneCenter = bb.center().cast<float>();
    float   sceneRadius = bb.diagonal() / 2;

    DTB::setTrackBall(sceneCenter, sceneRadius);
}

void MinimalViewer::draw(uint viewId)
{
    bgfx::setViewTransform(
        viewId, viewMatrix().data(), projectionMatrix().data());

    cameraUniforms.updateCamera(DTB::camera());
    cameraUniforms.bind();

    directionalLightUniforms.bind();

    for (DrawableObjectI* obj : *drawList)
        obj->draw(viewId);

    if (axis.isVisible()) {
        axis.draw(viewId);
    }
}

void MinimalViewer::keyPress(Key::Enum key)
{
    if (key == Key::A) {
        toggleAxisVisibility();
    }
    else {
        DTB::keyPress(key);
    }
}

uint MinimalViewer::pick(int x, int y)
{
    const bgfx::ViewId RENDER_PASS_ID = Context::requestViewId();
    const bgfx::ViewId RENDER_PASS_BLIT = Context::requestViewId();

    uint pickedObject = UINT_NULL;

    DTB::moveMouse(x, y);

    // clear
    bgfx::setViewClear(RENDER_PASS_ID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xff0000ff, 1.0f, 0);

    bgfx::setViewFrameBuffer(RENDER_PASS_ID, pickingFB);

    bgfx::setViewRect(RENDER_PASS_ID, 0, 0, 8, 8);
    setPickViewTransform(RENDER_PASS_ID, x, y);

    for (uint i = 0; i < drawList->size(); i++) {
        DrawableObjectI& obj = drawList->at(i);
        PickableObjectI* pickable = dynamic_cast<PickableObjectI*>(&obj);
        if (dynamic_cast<PickableObjectI*>(pickable)) {
            pickObjectIdUniforms.updateObjectId(i);
            pickObjectIdUniforms.bind();
            pickable->drawWithNames(RENDER_PASS_ID);
        }
    }

    // blit and read back the ID buffer
    bgfx::blit(RENDER_PASS_BLIT, blitTex, 0, 0, pickingRT);

    Array2<uint32_t> data;
    data.resize(8, 8);
    uint frt = bgfx::readTexture(blitTex, data.data());

    while (bgfx::frame() != frt) {
    }

    for (uint32_t i = 0; i < data.rows(); i++) {
        for (uint32_t j = 0; j < data.cols(); j++) {
            uint32_t id = data(j, i);
            if (id != 0) {
                pickedObject = id;
                break;
            }
        }
    }

    Context::releaseViewId(RENDER_PASS_BLIT);
    Context::releaseViewId(RENDER_PASS_ID);

    return pickedObject;
}

void MinimalViewer::setPickViewTransform(bgfx::ViewId id, int x, int y)
{
    // Compute the pick view matrix
    Matrix44f view = DTB::viewMatrix();

    // Compute the pick projection matrix
    Matrix44f proj = DTB::projectionMatrix();

    // Compute the pick view matrix
    float width  = (float)DTB::width();
    float height = (float)DTB::height();

    Matrix44f viewProj = view * proj;
    Matrix44f invViewProj = viewProj.inverse();

    float mouseXNDC = (x / width) * 2 - 1;
    float mouseYNDC = ((height - y) / height) * 2 - 1;

    Point3f nearPoint = mulH(Point3f(mouseXNDC, mouseYNDC, 0), invViewProj);
    Point3f farPoint = mulH(Point3f(mouseXNDC, mouseYNDC, 1), invViewProj);

    // lookAt unprojected point
    view = lookAtMatrix<Matrix44f>(nearPoint, farPoint, Point3f(0, 1, 0));
    proj = ::vcl::projectionMatrix<Matrix44f>(
        3.f, DTB::aspectRatio(), DTB::nearPlane(), DTB::farPlane(), false);

    bgfx::setViewTransform(id, view.data(), proj.data());
}

} // namespace vcl::bgf
