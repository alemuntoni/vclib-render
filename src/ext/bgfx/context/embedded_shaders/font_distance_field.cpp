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


#include <vclib/ext/bgfx/context/embedded_shaders/font_distance_field.h>

#include <shaders/vclib/ext/bgfx/font/fs_font_distance_field.sc.glsl.bin.h>
#include <shaders/vclib/ext/bgfx/font/vs_font_distance_field.sc.glsl.bin.h>

#include <shaders/vclib/ext/bgfx/font/fs_font_distance_field.sc.essl.bin.h>
#include <shaders/vclib/ext/bgfx/font/vs_font_distance_field.sc.essl.bin.h>

#include <shaders/vclib/ext/bgfx/font/fs_font_distance_field.sc.spv.bin.h>
#include <shaders/vclib/ext/bgfx/font/vs_font_distance_field.sc.spv.bin.h>
#ifdef _WIN32
#include <shaders/vclib/ext/bgfx/font/fs_font_distance_field.sc.dx11.bin.h>
#include <shaders/vclib/ext/bgfx/font/vs_font_distance_field.sc.dx11.bin.h>
#endif //  defined(_WIN32)
#ifdef __APPLE__
#include <shaders/vclib/ext/bgfx/font/fs_font_distance_field.sc.mtl.bin.h>
#include <shaders/vclib/ext/bgfx/font/vs_font_distance_field.sc.mtl.bin.h>
#endif // __APPLE__

namespace vcl::bgf {

bgfx::EmbeddedShader::Data vcl::bgf::EmbeddedShader<VclProgram::FONT_DISTANCE_FIELD>::
    vertexEmbeddedShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {type, vs_font_distance_field_essl, sizeof(vs_font_distance_field_essl)};
    case bgfx::RendererType::OpenGL:
        return {type, vs_font_distance_field_glsl, sizeof(vs_font_distance_field_glsl)};
    case bgfx::RendererType::Vulkan:
        return {type, vs_font_distance_field_spv, sizeof(vs_font_distance_field_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {type, vs_font_distance_field_dx11, sizeof(vs_font_distance_field_dx11)};
    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {type, vs_font_distance_field_mtl, sizeof(vs_font_distance_field_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

bgfx::EmbeddedShader::Data vcl::bgf::EmbeddedShader<VclProgram::FONT_DISTANCE_FIELD>::
    fragmentEmbeddedShader(bgfx::RendererType::Enum type)
{
    switch (type) {
    case bgfx::RendererType::OpenGLES:
        return {type, fs_font_distance_field_essl, sizeof(fs_font_distance_field_essl)};
    case bgfx::RendererType::OpenGL:
        return {type, fs_font_distance_field_glsl, sizeof(fs_font_distance_field_glsl)};
    case bgfx::RendererType::Vulkan:
        return {type, fs_font_distance_field_spv, sizeof(fs_font_distance_field_spv)};
#ifdef _WIN32
    case bgfx::RendererType::Direct3D11:
        return {
            type,
            fs_font_distance_field_dx11,
            sizeof(fs_font_distance_field_dx11)};
    case bgfx::RendererType::Direct3D12:
#endif
#ifdef __APPLE__
    case bgfx::RendererType::Metal:
        return {type, fs_font_distance_field_mtl, sizeof(fs_font_distance_field_mtl)};
#endif
    default: return {type, nullptr, 0};
    }
}

bgfx::EmbeddedShader::Data vcl::bgf::EmbeddedShader<VclProgram::FONT_DISTANCE_FIELD>::
    computeEmbeddedShader(bgfx::RendererType::Enum type)
{
    static_assert(true, "Invalid shader type for Font Basic");
    return {type, nullptr, 0};
}

} // namespace vcl::bgf
