#pragma once


namespace ad {

static const char* gVertexShader = R"#(
#version 400

layout(location=0) in vec2 in_Position;
layout(location=2) in vec2 in_InstancePosition;
uniform vec4 u_Color;
uniform mat4 u_WorldToDevice;
out vec4 ex_Color;

void main(void)
{
    gl_Position = vec4(in_Position+in_InstancePosition, 0, 1) * u_WorldToDevice;
    ex_Color = u_Color;
}
)#";

static const char* gVertexShader3D = R"#(
#version 400

layout(location=0) in vec4 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_InstancePosition;
layout(location=3) in mat4 in_InstanceOrientation;
uniform vec4 u_Color;
uniform mat4 u_WorldToDevice;
out vec4 ex_Color;
out vec4 ex_Normal;

void main(void)
{
    // IMPORTANT: in_InstanceOrientation matrix is not transposed, so we pre-multiply the vector
    // u_WorldToDevice is transposed, so we post-multiply the vector
    gl_Position = (in_InstanceOrientation * in_Position + vec4(in_InstancePosition, 0, 0))
        * u_WorldToDevice;
    ex_Normal = in_InstanceOrientation * vec4(in_Normal, 0.0);
    ex_Color = u_Color;
}
)#";

static const char* gFragmentShader = R"#(
#version 400

in vec4 ex_Color;
layout (location=0) out vec4 out_Scene;
layout (location=1) out vec4 out_Bloom;

void main(void)
{
    out_Scene = ex_Color;
    out_Bloom = ex_Color;
}
)#";

static const char* gFragmentShader3D = R"#(
#version 400

in vec4 ex_Color;
in vec4 ex_Normal;
layout (location=0) out vec4 out_Scene;
layout (location=1) out vec4 out_Bloom;

void main(void)
{
    float factor = dot(ex_Normal, vec4(0.0, 0.0, 1.0, 0.0));
    out_Scene = ex_Color + min(0.0, 0.65*factor);
    out_Bloom = ex_Color * factor;
}
)#";

} // namespace ad
