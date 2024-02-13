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

#ifndef VCL_RENDER_INTERFACES_DRAWABLE_OBJECT_I_H
#define VCL_RENDER_INTERFACES_DRAWABLE_OBJECT_I_H

#include <vclib/space/point.h>

#include "shader_program_i.h"

namespace vcl {

/**
 * @brief The DrawableObject class is the base class for all the objects that
 * can be rendered in a viewer.
 *
 * The DrawableObject class stores only an attribute that represents the name
 * of the object, that will be used to identify it in the viewer.
 *
 * A class that inherits from DrawableObject must implement the following member
 * functions:
 * - draw(uint viewId);
 * - center();
 * - radius();
 * - clone();
 * - isVisibile();
 * - setVisibility(bool);
 *
 * There is also a member function that can be implemented, but it is not
 * mandatory:
 * - init();
 *
 * For more details about these member functions, check the documentation of
 * each one.
 */
class DrawableObjectI
{
    std::string n; /**< @brief Name of the object */

public:
    /**< @brief Empty constructor */
    DrawableObjectI() = default;

    virtual ~DrawableObjectI() {}

    /**
     * @brief This member function is called after the initialization of the
     * Context. It must initialize and bind data to the GPU like buffers and
     * textures.
     */
    virtual void init() {};

    virtual void setShaderProgram(const ShaderProgramI&) {}

    /**
     * @brief This member function must draw the object.
     * It will be called at every frame.
     *
     * @param viewId The ID of the view to draw. It may be used depending on the
     * rendering engine.
     */
    virtual void draw(uint viewId = 0) = 0;

    /**
     * @brief This member function is used to find a good camera position
     * to render object. It must return the position of the center of the
     * object.
     * @return The position of the center of the object.
     */
    virtual vcl::Point3d center() const = 0;

    /**
     * @brief This member function is used to find a good camera position
     * to render object. It should return the ray of the bounding sphere of the
     * object, but also half diagonal of the bounding box of the object is a
     * good approximation. Return -1 if the object shouldn't influence the
     * position of the camera.
     * @return The radius of the bounding sphere of the object.
     */
    virtual double radius() const = 0;

    /**
     * @brief This member function is used to create a new copy of the
     * DrawableObjectI. Each derived class must implement this member function,
     * that returns a new dynamically allocated DrawableObjectI that is a copy of
     * the current one. for more details about this paradigm, check polimorphism
     * clone in modern c++:
     * https://www.fluentcpp.com/2017/09/08/make-polymorphic-copy-modern-cpp/
     *
     * @return A new dynamically allocated DrawableObjectI that is a copy of the
     * current one.
     */
    [[nodiscard(
        "You should assign and manage the pointer to the newly created "
        "object")]] virtual DrawableObjectI*
    clone() const = 0;

    /**
     * @brief This member function is used to check if the object is visible.
     * @return `true` if the object is visible;
     */
    virtual bool isVisible() const = 0;

    /**
     * @brief This member function is used to set the visibility of the object.
     * @param[in] vis: `true` if the object should be visible;
     */
    virtual void setVisibility(bool vis) = 0;

    /**
     * @brief Returns the name of the object.
     * @return The name of the object.
     */
    const std::string& name() const { return n; }

    /**
     * @brief Returns a reference of the name of the object, that allows to
     * modify it.
     * @param[in] name: The name of the object.
     */
    std::string& name() { return n; }
};

} // namespace vcl

#endif // VCL_RENDER_INTERFACES_DRAWABLE_OBJECT_I_H
