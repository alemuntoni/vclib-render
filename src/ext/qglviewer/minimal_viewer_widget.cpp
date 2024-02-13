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

#include <vclib/ext/qglviewer/minimal_viewer_widget.h>

#include <vclib/math/min_max.h>
#include <vclib/space/box.h>

namespace vcl::qgl {

MinimalViewerWidget::MinimalViewerWidget(QWidget* parent) : QGLViewer(parent)
{
    drawList = std::make_shared<DrawableObjectVector>();
}

MinimalViewerWidget::MinimalViewerWidget(
    std::shared_ptr<DrawableObjectVector> v,
    QWidget*                              parent) :
        QGLViewer(parent),
        drawList(v)
{
}

void MinimalViewerWidget::init()
{
    for (DrawableObjectI* d : *drawList) {
        d->init();
    }
}

void MinimalViewerWidget::setDrawableObjectVector(
    std::shared_ptr<DrawableObjectVector> v)
{
    drawList = v;
}

std::shared_ptr<const DrawableObjectVector> MinimalViewerWidget::
    drawableObjectVector() const
{
    return drawList;
}

void MinimalViewerWidget::fitScene()
{
    Box3d   bb          = drawList->boundingBox();
    Point3d sceneCenter = bb.center();
    double  sceneRadius = bb.diagonal() / 2;

    setSceneCenter(
        qglviewer::Vec(sceneCenter.x(), sceneCenter.y(), sceneCenter.z()));
    setSceneRadius(sceneRadius);
    showEntireScene();
}

void MinimalViewerWidget::draw()
{
    for (DrawableObjectI* obj : *drawList)
        obj->draw(0);
}

} // namespace vcl::qgl
