#version 450

//! [0]
uniform mat4 mvpMatrix;

in vec4 vertex;
in vec4 color;
in vec4 offset;

out vec4 varyingColor;

void main(void)
{
//    vec4 offset = vec4(1.0, 0.0, 0.0, 0.0);
    varyingColor = color;
    gl_Position = mvpMatrix * vertex + offset;
}
//! [0]
