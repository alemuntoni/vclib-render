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

#ifndef VCL_EXT_QT_GUI_MESH_RENDER_SETTINGS_FRAME_H
#define VCL_EXT_QT_GUI_MESH_RENDER_SETTINGS_FRAME_H

#include <QFrame>

#include <vclib/render/mesh_render_settings.h>

class QPushButton;

namespace vcl::qt {

namespace Ui {
class MeshRenderSettingsFrame;
} // namespace Ui

class MeshRenderSettingsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit MeshRenderSettingsFrame(QWidget* parent = nullptr);
    ~MeshRenderSettingsFrame();

    const MeshRenderSettings& meshRenderSettings() const;
    void setMeshRenderSettings(const MeshRenderSettings& settings);

signals:
    void settingsUpdated();

private slots:
    void on_pointVisibilityCheckBox_stateChanged(int arg1);
    void on_pointShapeCircleRadioButton_toggled(bool checked);
    void on_pointShapePixelRadioButton_toggled(bool checked);
    void on_pointShadingVertexRadioButton_toggled(bool checked);
    void on_pointShadingNoneRadioButton_toggled(bool checked);
    void on_pointColorComboBox_currentIndexChanged(int index);
    void on_pointColorDialogPushButton_clicked();
    void on_pointSizeSlider_valueChanged(int value);

    void on_surfaceVisibilityCheckBox_stateChanged(int arg1);
    void on_surfaceShadingNoneRadioButton_toggled(bool checked);
    void on_surfaceShadingSmoothRadioButton_toggled(bool checked);
    void on_surfaceShadingFlatRadioButton_toggled(bool checked);
    void on_surfaceColorComboBox_currentIndexChanged(int index);
    void on_surfaceColorDialogPushButton_clicked();

    void on_wireframeVisibilityCheckBox_stateChanged(int arg1);
    void on_wireframeShadingNoneRadioButton_toggled(bool checked);
    void on_wireframeShadingVertexRadioButton_toggled(bool checked);
    void on_wireframeColorComboBox_currentIndexChanged(int index);
    void on_wireframeColorDialogPushButton_clicked();
    void on_wireframeSizelSlider_valueChanged(int value);

private:
    enum POINT_COLOR { P_VERT = 0, P_MESH, P_USER };

    enum SURF_COLOR {
        SC_VERT = 0,
        SC_FACE,
        SC_MESH,
        SC_VERT_TEX,
        SC_WEDG_TEX,
        SC_USER
    };

    enum WIRE_COLOR { W_VERTEX = 0, W_MESH, W_USER };

    Ui::MeshRenderSettingsFrame* ui;
    MeshRenderSettings           mrs;

    void updateGuiFromSettings();
    void updatePointsTabFromSettings();
    void updatePointsColorComboBoxFromSettings();
    void updateSurfaceTabFromSettings();
    void uptateSurfaceShadingRadioButtonsFromSettings();
    void updateSurfaceColorComboBoxFromSettings();
    void updateWireframeTabFromSettings();
    void updateWireframeComboBoxFromSettings();

    void   setButtonBackGround(QPushButton* b, const QColor& c);
    QColor getButtonBackGround(QPushButton* b);
};

} // namespace vcl::qt

#endif // VCL_EXT_QT_GUI_MESH_RENDER_SETTINGS_FRAME_H
