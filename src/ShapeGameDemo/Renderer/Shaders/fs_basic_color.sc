
// File: fs_basic_color.sc
// Basic Fragment Shader. Just outputs the interpolated color.

$input v_color0
$output o_color

#include <bgfx_shader.sh>

void main()
{
    // Output the color received from the vertex shader
    o_color = v_color0;
}