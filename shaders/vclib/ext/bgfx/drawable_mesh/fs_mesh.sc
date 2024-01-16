$input v_pos
$input v_normal
$input v_color
$input v_bc

#include <drawable_mesh/uniforms.sh>
#include <vclib/render/mesh_render_settings_macros.h>

BUFFER_RO(triangleColors, uint, 1);

void main()
{
    uint drawModeInt = floatBitsToUint(drawMode.x);

    // if not drawing mesh, discard
    if (!bool(drawModeInt & VCL_MRS_DRAW_MESH)) {
        discard;
    }

    /***** compute light ******/
    // default values
    vec3 specular = vec3(0.0, 0.0, 0.0);
    vec4 light = vec4(1, 1, 1, 1);

    vec3 normal = v_normal;

    // if flat shading, compute normal of face
    if (bool(drawModeInt & (VCL_MRS_DRAW_SURF_SHADING_FLAT))) {
        vec3 X = dFdx(v_pos);
        vec3 Y = dFdy(v_pos);
        normal = normalize(cross(X,Y));
    }

    // if flat or smooth shading, compute light
    if (!bool(drawModeInt & (VCL_MRS_DRAW_SURF_SHADING_NONE))) {
        light = computeLight(u_lightDir, u_lightColor, normal);


        specular = computeSpecular(
            v_pos,
            u_cameraEyePos,
            u_lightDir,
            u_lightColor,
            normal);
    }

    /***** compute color ******/
    vec4 color = userSurfaceColor;

    if (bool(drawModeInt & (VCL_MRS_DRAW_SURF_COLOR_VERTEX))) {
        color = v_color;
    }
    if (bool(drawModeInt & (VCL_MRS_DRAW_SURF_COLOR_MESH))) {
        color = meshColor;
    }
    if (bool(drawModeInt & (VCL_MRS_DRAW_SURF_COLOR_FACE))) {
        color = uintToVec4Color(triangleColors[gl_PrimitiveID]);
    }

    /***** compute wireframe ******/
    if (bool(drawModeInt & (VCL_MRS_DRAW_WIREFRAME))) {
        float wfThickness = 1;
        vec3 wfColor = vec3(1.0, 1.0, 1.0);

       vec3 fw = abs(dFdx(v_bc)) + abs(dFdy(v_bc));
       vec3 val = smoothstep(vec3_splat(0.0), fw*wfThickness, v_bc);
       float edge = min(min(val.x, val.y), val.z);

       vec3 edgeCol = mix(color.xyz, wfColor, 1.0);
       color.xyz = mix(wfColor, color.xyz, edge);
    }

    gl_FragColor = light * color + vec4(specular, 0);
}
