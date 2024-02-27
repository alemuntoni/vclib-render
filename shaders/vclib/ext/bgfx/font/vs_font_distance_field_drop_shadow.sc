$input a_position, a_color0, a_color1, a_texcoord0, a_texcoord1, a_texcoord2
$output v_color0, v_color1, v_texcoord0, v_texcoord1, v_texcoord2

#include <bgfx_shader.sh>

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 0.0, 1.0) );
	v_texcoord0 = a_texcoord0;
	v_texcoord1 = a_texcoord1;
	v_texcoord2 = a_texcoord2;
	v_color0 = a_color0;
	v_color1 = a_color1;
}
