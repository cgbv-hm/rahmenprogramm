#version 430 // change later



// Structures
// =============================================================================================================
struct Matrices
{
	mat4 mvp;
	mat4 mv;
	mat3 normal;
};


struct Light
{
    vec3 lightPos;
};


struct FragmentInput
{
	vec2 uv;
};

struct Normalized
{
    vec3 normal;
    vec3 lightDir;
    vec3 viewDir;
};

struct Textures
{
	sampler2D tex;
};
// =============================================================================================================




// Subroutines
// =============================================================================================================
subroutine void FragmentProgram();
// =============================================================================================================




// Variables
// =============================================================================================================
layout(location = 0) in FragmentInput Input;

uniform Matrices matrices;

uniform Light light;

uniform Textures textures;

uniform float animStage;

subroutine uniform FragmentProgram fragmentprogram;

layout(location = 0) out vec4 out_color;
// =============================================================================================================




// Methods
// =============================================================================================================
void main()
{
	fragmentprogram();
}
// =============================================================================================================




// Subroutine Implementation
// =============================================================================================================
layout (index = 0) subroutine (FragmentProgram) void textured()
{
	out_color = texture(textures.tex, Input.uv);
}
// =============================================================================================================