$input v_color

#include <bgfx_shader.sh>

uniform vec4 meshColor;

void main()
{
    gl_FragColor = v_color;
}
