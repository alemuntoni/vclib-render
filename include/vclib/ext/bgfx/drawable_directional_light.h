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

#ifndef VCL_EXT_BGFX_DRAWABLE_DIRECTIONAL_LIGHT_H
#define VCL_EXT_BGFX_DRAWABLE_DIRECTIONAL_LIGHT_H

#include <bgfx/bgfx.h>

#include <vclib/render/interfaces/drawable_object_i.h>
#include <vclib/render/lights/directional_light.h>
#include <vclib/space/matrix.h>

#include "uniforms/drawable_directional_light_uniforms.h"

namespace vcl::bgf {

class DrawableDirectionalLight : public DrawableObjectI
{
    bool visible;

    DirectionalLight<float> light;
    vcl::Matrix44f transform = vcl::Matrix44f::Identity();
    std::vector<float> vertices;
    vcl::Color lColor = vcl::Color::Yellow;

    bgfx::VertexBufferHandle vertexCoordBH   = BGFX_INVALID_HANDLE;

    DrawableDirectionalLightUniforms dlUniforms;

    bgfx::ProgramHandle program = BGFX_INVALID_HANDLE;

public:
    DrawableDirectionalLight();

    void update(const DirectionalLight<float>& l);

    const vcl::Color& linesColor() const { return lColor; }

    void setLinesColor(const vcl::Color& c);

    // DrawableObjectI interface

    void             setShaderProgram(const ShaderProgramI&);

    void             draw(uint viewId);

    Point3d          center() const;

    double           radius() const;

    DrawableObjectI* clone() const;

    bool             isVisible() const { return visible;}

    void             setVisibility(bool vis) { visible = vis;}

private:
    void updateVertexBuffer();
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_DRAWABLE_DIRECTIONAL_LIGHT_H
