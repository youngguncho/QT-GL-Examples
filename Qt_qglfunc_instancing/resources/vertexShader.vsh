#version 450

//! [0]
uniform mat4 mvpMatrix;

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec4 instance_position;

out VS_OUT
{
    vec4 color;

} vs_out;

void main(void)
{
    vs_out.color = color;
//    offset_vec = vec4(offset, offset, offset, 1);
//    gl_Position = mvpMatrix * vertex + vec4(offset, offset, offset, 1);
    gl_Position = (position + instance_position) * vec4(0.25, 0.25, 1.0, 1.0);
}
//! [0]
