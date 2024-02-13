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

#ifdef USE_QT
#include "minimal_viewer_qt.h"
#include <QApplication>
#elif USE_GLFW
#include "minimal_viewer_glfw.h"
#endif

int main(int argc, char** argv)
{
#ifdef USE_QT
    QApplication app(argc, argv);

    MinimalViewerQt tw;

    tw.show();

    return app.exec();
#elif USE_GLFW
    MinimalViewerGLFW tw;

    tw.show();

    return 0;
#else
    return 0;
#endif
}
