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

#include "ext/qglviewer/ui_viewer_main_window.h"
#include <vclib/ext/qglviewer/viewer_main_window.h>

#include <vclib/render/interfaces/drawable_mesh_i.h>

namespace vcl::qgl {

/**
 * @brief ViewerMainWindow constructor.
 *
 * Creates a MainWindow having the given parent.
 * It will manage initialize an empty vector of DrawableObjects, which owns.
 *
 * @param parent
 */
ViewerMainWindow::ViewerMainWindow(QWidget* parent) :
        QMainWindow(parent), mUI(new Ui::ViewerMainWindow)
{
    mUI->setupUi(this);

    // create the vector of DrawableObjects
    mDrawVector = std::make_shared<vcl::DrawableObjectVector>();

    // give the vector pointer to the contained widgets
    mUI->glArea->setDrawableObjectVector(mDrawVector);
    mUI->drawVectorFrame->setDrawableObjectVector(mDrawVector);

    // each time that the RenderSettingsFrame updates its settings, we call the
    // renderSettingsUpdated() member function
    connect(
        mUI->renderSettingsFrame,
        SIGNAL(settingsUpdated()),
        this,
        SLOT(renderSettingsUpdated()));

    // each time that the drawVectorFrame changes the visibility of an object,
    // we update the current settings of the RenderSettingsFrame, and we update
    // the glArea
    connect(
        mUI->drawVectorFrame,
        SIGNAL(drawableObjectVisibilityChanged()),
        this,
        SLOT(visibilityDrawableObjectChanged()));

    // each time that the selected object is changed in the drawVectorFrame, we
    // update the RenderSettingsFrame, updating its settings to the object
    // render settings
    connect(
        mUI->drawVectorFrame,
        SIGNAL(drawableObjectSelectionChanged(uint)),
        this,
        SLOT(selectedDrawableObjectChanged(uint)));

    mUI->rightArea->setVisible(false);
}

ViewerMainWindow::~ViewerMainWindow()
{
    delete mUI;
}

/**
 * @brief Sets the current DrawableObjectVector, and updates the GUI
 * accordingly.
 * @param v
 */
void ViewerMainWindow::setDrawableObjectVector(
    const std::shared_ptr<vcl::DrawableObjectVector>& v)
{
    mDrawVector = v;

    // order here is important: drawVectorFrame must have the drawVector before
    // the renderSettingsFrame!
    mUI->glArea->setDrawableObjectVector(mDrawVector);
    mUI->drawVectorFrame->setDrawableObjectVector(mDrawVector);
    if (mDrawVector->size() > 0) {
        auto m = std::dynamic_pointer_cast<DrawableMeshI>(mDrawVector->at(0));
        if (m) {
            mUI->renderSettingsFrame->setMeshRenderSettings(
                m->renderSettings(), true);
            mUI->renderSettingsFrame->setVisible(true);
        }
        else {
            mUI->renderSettingsFrame->setVisible(false);
        }
        // right area is visible if there is at least one DrawableObject
        mUI->rightArea->setVisible(true);
    }
    else {
        mUI->rightArea->setVisible(false);
    }
    mUI->glArea->fitScene();
}

/**
 * @brief Slot called when the user changed the visibility of an object in the
 * DrawableObjectVectorFrame
 */
void ViewerMainWindow::visibilityDrawableObjectChanged()
{
    // get the selected drawable object
    uint i = mUI->drawVectorFrame->selectedDrawableObject();
    auto m = std::dynamic_pointer_cast<DrawableMeshI>(mDrawVector->at(i));
    // if it is a DrawableMeshI, we must be sure that its render
    // settings are updated accordingly.
    if (m) {
        mUI->renderSettingsFrame->setMeshRenderSettings(m->renderSettings());
    }
    mUI->glArea->update();
}

/**
 * @brief Slot called when the user selected a different DrawableObject in the
 * DrawableObjectVectorFrame
 * @param i
 */
void ViewerMainWindow::selectedDrawableObjectChanged(uint i)
{
    // take the newly selected DrawableObject and check whether it is a
    // DrawableMeshI
    auto m = std::dynamic_pointer_cast<DrawableMeshI>(mDrawVector->at(i));
    if (m) {
        // if it is a DrawableMeshI, update the RenderSettingsFrame, and
        // set it visible
        mUI->renderSettingsFrame->setMeshRenderSettings(m->renderSettings());
        mUI->renderSettingsFrame->setVisible(true);
    }
    else {
        // it is not a DrawableMeshI, RenderSettingsFrame must be hidden
        mUI->renderSettingsFrame->setVisible(false);
    }
}

/**
 * @brief Slot called every time that the MeshRenderSettingsFrame emits
 * 'settingsUpdated()', that is when the user changes render settings of a
 * GeneriDrawableMesh.
 *
 * We need to get the selected GeneriDrawableMesh first, and then update the
 * settings to it.
 */
void ViewerMainWindow::renderSettingsUpdated()
{
    // The user changed the RenderSettings of the ith object.
    uint i = mUI->drawVectorFrame->selectedDrawableObject();
    if (mDrawVector->size() > 0) {
        // The selected object must always be a DrawableMeshI, because the
        // RenderSettingsFrame (which called this member function) is visible
        // only when the selected Object is a DrawableMeshI
        auto m = std::dynamic_pointer_cast<DrawableMeshI>(mDrawVector->at(i));
        // get RenderSettings from the RenderSettingsFrame, and set it to the
        // DrawableMeshI
        m->setRenderSettings(mUI->renderSettingsFrame->meshRenderSettings());
        mUI->glArea->update();
    }
}

} // namespace vcl::qgl
