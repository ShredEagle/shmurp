#pragma once


namespace ad {

static const char* gVertexShader = R"#(
#version 400

layout(location=0) in vec2 in_Position;
layout(location=1) in vec2 in_InstancePosition;
uniform vec4 u_Color;
uniform mat4 u_WorldToDevice;
out vec4 ex_Color;

void main(void)
{
    gl_Position = vec4(in_Position+in_InstancePosition, 0, 1) * u_WorldToDevice;
    ex_Color = u_Color;
}
)#";

static const char* gFragmentShader = R"#(
#version 400

in vec4 ex_Color;
out vec4 out_Color;

void main(void)
{
    out_Color = ex_Color;
}
)#";

} // namespace ad
