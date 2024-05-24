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

#ifndef VCL_BGFX_RENDER_DRAWABLE_DRAWABLE_MESH_H
#define VCL_BGFX_RENDER_DRAWABLE_DRAWABLE_MESH_H

#include <bgfx/bgfx.h>

#include <vclib/render/interfaces/drawable_mesh_i.h>

#include <vclib_bgfx/render/context.h>
#include <vclib_bgfx/render/drawable/mesh/mesh_render_buffers.h>
#include <vclib_bgfx/render/drawable/uniforms/drawable_mesh_uniforms.h>
#include <vclib_bgfx/render/drawable/uniforms/mesh_render_settings_uniforms.h>

namespace vcl {

template<MeshConcept MeshType>
class DrawableMesh : public DrawableMeshI
{
    using Base = DrawableMeshI;

    MeshRenderBuffers<MeshType> mMRB;

    bgfx::ProgramHandle mProgram =
        Context::programManager().getProgram(VclProgram::DRAWABLE_MESH);

    DrawableMeshUniforms       mMeshUniforms;
    MeshRenderSettingsUniforms mMeshRenderSettingsUniforms;

public:
    DrawableMesh() = default;

    DrawableMesh(const MeshType& mesh) : Base(mesh)
    {
        updateBuffers(mesh);
        mMRS.setDefaultSettingsFromCapability();
    }

    ~DrawableMesh() = default;

    void updateBuffers(const MeshType& m)
    {
        if constexpr (HasName<MeshType>) {
            name() = m.name();
        }

        mMRB = MeshRenderBuffers<MeshType>(m);
        mMRS.setRenderCapabilityFrom(m);
        mMeshRenderSettingsUniforms.updateSettings(mMRS);
        mMeshUniforms.update(mMRB);
    }

    // DrawableObject implementation

    void init() {}

    void draw(uint viewId)
    {
        if (bgfx::isValid(mProgram)) {
            uint64_t state =
                0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL |
                BGFX_STATE_BLEND_FUNC(
                    BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);

            if (mMRS.isSurfaceVisible()) {
                mMRB.bindVertexBuffers();
                mMRB.bindIndexBuffers();
                bindUniforms(VCL_MRS_DRAWING_SURFACE);

                bgfx::setState(state);

                bgfx::submit(viewId, mProgram);
            }

            if (mMRS.isWireframeVisible()) {
                mMRB.bindVertexBuffers();
                mMRB.bindIndexBuffers(mMRB.WIREFRAME);
                bindUniforms(VCL_MRS_DRAWING_WIREFRAME);

                bgfx::setState(state | BGFX_STATE_PT_LINES);

                bgfx::submit(viewId, mProgram);
            }

            if (mMRS.isPointCloudVisible()) {
                mMRB.bindVertexBuffers();
                bindUniforms(VCL_MRS_DRAWING_POINTS);

                bgfx::setState(state | BGFX_STATE_PT_POINTS);

                bgfx::submit(viewId, mProgram);
            }

            if (mMRS.isEdgesVisible()) {
                mMRB.bindVertexBuffers();
                mMRB.bindIndexBuffers(mMRB.EDGES);
                bindUniforms(VCL_MRS_DRAWING_EDGES);

                bgfx::setState(state | BGFX_STATE_PT_LINES);

                bgfx::submit(viewId, mProgram);
            }
        }
    }

    vcl::Point3d center() const { return (mMRB.bbMin() + mMRB.bbMax()) / 2; }

    double radius() const { return (mMRB.bbMax() - mMRB.bbMin()).norm() / 2; }

    DrawableMesh* clone() const { return new DrawableMesh(*this); }

    void setVisibility(bool vis)
    {
        DrawableMeshI::setVisibility(vis);
        mMeshRenderSettingsUniforms.updateSettings(mMRS);
    }

    void setRenderSettings(const MeshRenderSettings& rs)
    {
        DrawableMeshI::setRenderSettings(rs);
        mMeshRenderSettingsUniforms.updateSettings(rs);
    }

private:
    void bindUniforms(uint primitive)
    {
        mMeshRenderSettingsUniforms.updatePrimitive(primitive);
        mMeshRenderSettingsUniforms.bind();
        mMeshUniforms.bind();
    }
};

} // namespace vcl

#endif // VCL_BGFX_RENDER_DRAWABLE_DRAWABLE_MESH_H
