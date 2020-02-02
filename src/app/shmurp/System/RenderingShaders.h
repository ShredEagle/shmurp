#pragma once


namespace ad {

static const char* gVertexShader = R"#(
#version 400

layout(location=0) in vec4 in_Position;
uniform mat4 u_WorldToScreen;
out vec4 ex_Color;

void main(void)
{
    gl_Position = in_Position * u_WorldToScreen;
    ex_Color = vec4(0.0, 0.0, 0.8, 1.0);
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
