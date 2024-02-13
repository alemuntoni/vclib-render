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

#ifndef VCL_EXT_BGFX_SHADER_PROGRAMS_PICKING_SHADER_PROGRAM_H
#define VCL_EXT_BGFX_SHADER_PROGRAMS_PICKING_SHADER_PROGRAM_H

#include <vclib/render/interfaces/picking_shader_program_i.h>

#include "load_program.h"

namespace vcl::bgf {

class PickingShaderProgram : public PickingShaderProgramI
{
    bgfx::ProgramHandle p = BGFX_INVALID_HANDLE;

public:
    PickingShaderProgram()
    {
        p = vcl::bgf::loadProgram(
            "vclib/ext/bgfx/pickable_object/vs_pickable_object",
            "vclib/ext/bgfx/pickable_object/fs_pickable_object");
    }

    PickingShaderProgram(const std::string& vs, const std::string& fs)
    {
        p = loadProgram(vs, fs);
    };

    ~PickingShaderProgram()
    {
        if (bgfx::isValid(p)) {
            bgfx::destroy(p);
        }
    };

    PickingShaderProgram(const PickingShaderProgram&) = delete;

    PickingShaderProgram& operator=(const PickingShaderProgram&) =
        delete;

    bgfx::ProgramHandle program() const { return p; };
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_SHADER_PROGRAMS_PICKING_SHADER_PROGRAM_H
