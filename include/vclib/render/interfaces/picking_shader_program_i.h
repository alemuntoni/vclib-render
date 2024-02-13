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

#ifndef VCL_RENDER_INTERFACES_PICKING_SHADER_PROGRAM_I_H
#define VCL_RENDER_INTERFACES_PICKING_SHADER_PROGRAM_I_H

#include "shader_program_i.h"

namespace vcl {

class PickingShaderProgramI : public ShaderProgramI
{
public:
    virtual ~PickingShaderProgramI() = default;
};

} // namespace vcl

#endif // VCL_RENDER_INTERFACES_PICKING_SHADER_PROGRAM_I_H
