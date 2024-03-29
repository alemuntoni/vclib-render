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

#ifndef VCL_EXT_BGFX_TEXT_TEXT_VIEW_H
#define VCL_EXT_BGFX_TEXT_TEXT_VIEW_H

#include "../context.h"
#include "text_manager.h"

namespace vcl::bgf {

class TextView
{
    TextManager  textManager;
    bgfx::ViewId view = BGFX_INVALID_VIEW;
    float        textViewMatrix[16];
    float        textProjMatrix[16];
    uint         w = 0;
    uint         h = 0;

public:
    TextView();
    ~TextView();

    void enableText(bool b = true);
    bool isTextEnabled() const;

    void setTextFont(const std::string& fontName, uint fontSize);

    void clearText();

    void appendStaticText(
        const vcl::Point2f& pos,
        const std::string&  text,
        const vcl::Color&   color = vcl::Color::Black);

    void appendTransientText(
        const vcl::Point2f& pos,
        const std::string&  text,
        const vcl::Color&   color = vcl::Color::Black);

protected:
    void init(uint width, uint height);

    void frame(bgfx::FrameBufferHandle fbh);

    void resize(uint width, uint height);

private:
    void updateProjMatrix();
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_TEXT_TEXT_VIEW_H
