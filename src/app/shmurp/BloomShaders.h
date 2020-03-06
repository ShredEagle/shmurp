//
// COMPOSING RESULT
//
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


//
// BLURRING
//
static const char* gHBlurFragment = R"#(
#version 400

in vec2 ex_TexCoords;
out vec4 out_Color;

uniform sampler2D inputTexture;
vec2 textureSize = textureSize(inputTexture, 0);

float weights[] = float[](0.4, 0.2, 0.1, 0.08, 0.05, 0.05, 0.05, 0.03, 0.02, 0.01);

void main(void)
{
    out_Color = weights[0] * texture(inputTexture, ex_TexCoords);
    vec2 texOffset = vec2(1.0/textureSize.x, 0.);

    for(int i=1; i<weights.length(); ++i)
    {
        out_Color += weights[i] * texture(inputTexture, ex_TexCoords - (i*texOffset));
        out_Color += weights[i] * texture(inputTexture, ex_TexCoords + (i*texOffset));
    }
}
)#";

static const char* gVBlurFragment = R"#(
#version 400

in vec2 ex_TexCoords;
out vec4 out_Color;

uniform sampler2D inputTexture;
vec2 textureSize = textureSize(inputTexture, 0);

//float weights[4] = float[](0.3, 0.2, 0.1, 0.05);
float weights[] = float[](0.4, 0.2, 0.1, 0.08, 0.05, 0.05, 0.05, 0.03, 0.02, 0.01);

void main(void)
{
    out_Color = weights[0] * texture(inputTexture, ex_TexCoords);
    vec2 texOffset = vec2(0., 1.0/textureSize.x);

    for(int i=1; i<weights.length(); ++i)
    {
        out_Color += weights[i] * texture(inputTexture, ex_TexCoords - (i*texOffset));
        out_Color += weights[i] * texture(inputTexture, ex_TexCoords + (i*texOffset));
    }
}
)#";

