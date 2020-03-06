static const char* gScreenVertex = R"#(
#version 400

layout(location=0) in vec4 in_Position;
layout(location=1) in vec2 in_TexCoords;
out vec2 ex_TexCoords;

void main(void)
{
    gl_Position = in_Position;
    ex_TexCoords = in_TexCoords;
}
)#";

static const char* gScreenFragment = R"#(
#version 400

in vec2 ex_TexCoords;
out vec4 out_Color;

uniform sampler2D sceneTexture;
//uniform sampler2D bloomTexture;

void main(void)
{
    out_Color = texture(sceneTexture, ex_TexCoords);

    //out_Color = texture(bloomTexture, ex_TexCoords);

    //out_Color = texture(sceneTexture, ex_TexCoords)
    //            + texture(bloomTexture, ex_TexCoords);
}
)#";
