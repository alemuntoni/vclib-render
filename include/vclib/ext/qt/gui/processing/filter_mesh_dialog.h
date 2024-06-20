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

#ifndef VCL_EXT_QT_GUI_PROCESSING_FILTER_MESH_DIALOG_H
#define VCL_EXT_QT_GUI_PROCESSING_FILTER_MESH_DIALOG_H

#include <QDialog>

#include <vclib/processing/actions/interfaces/filter_mesh_action.h>

namespace vcl::qt {

namespace Ui {
class FilterMeshDialog;
} // namespace Ui

class FilterMeshDialog : public QDialog
{
    Q_OBJECT

    Ui::FilterMeshDialog* mUI;

public:
    explicit FilterMeshDialog(
        const std::shared_ptr<proc::FilterMeshAction>& action,
        QWidget*                                       parent = nullptr);
    ~FilterMeshDialog();

};

} // namespace vcl::qt

#endif // VCL_EXT_QT_GUI_PROCESSING_FILTER_MESH_DIALOG_H
