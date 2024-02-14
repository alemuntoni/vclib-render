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

#ifndef VCL_RENDER_INTERFACES_DRAWABLE_MESH_I_H
#define VCL_RENDER_INTERFACES_DRAWABLE_MESH_I_H

#include "../mesh_render_settings.h"
#include "drawable_object_i.h"

namespace vcl {

class DrawableMeshI : public vcl::DrawableObjectI
{
protected:
    MeshRenderSettings mrs;

public:
    DrawableMeshI() = default;

    template<MeshConcept MeshType>
    DrawableMeshI(const MeshType& m) : mrs(m)
    {
    }

    const MeshRenderSettings& renderSettings() const { return mrs; }

    virtual void setRenderSettings(const MeshRenderSettings& rs) { mrs = rs; }

    // DrawableObject implementation

    inline bool isVisible() const { return mrs.isVisible(); }

    inline void setVisibility(bool vis) { mrs.setVisibility(vis); }
};

} // namespace vcl

#endif // VCL_RENDER_INTERFACES_DRAWABLE_MESH_I_H
