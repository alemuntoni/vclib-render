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

#ifndef VCL_EXT_QT_GUI_DRAWABLE_OBJECT_FRAME_H
#define VCL_EXT_QT_GUI_DRAWABLE_OBJECT_FRAME_H

#include <QFrame>

#include <vclib/render/interfaces/drawable_object_i.h>

namespace vcl::qt {

namespace Ui {
class DrawableObjectFrame;
} // namespace Ui

class DrawableObjectFrame : public QFrame
{
    Q_OBJECT

public:
    explicit DrawableObjectFrame(
        DrawableObjectI* obj,
        QWidget*         parent = nullptr);
    ~DrawableObjectFrame();

signals:
    void visibilityChanged();

private slots:
    void on_visibilityCheckBox_stateChanged(int arg1);

private:
    Ui::DrawableObjectFrame* ui;
    DrawableObjectI*         obj;
};

} // namespace vcl::qt

#endif // VCL_EXT_QT_GUI_DRAWABLE_OBJECT_FRAME_H
