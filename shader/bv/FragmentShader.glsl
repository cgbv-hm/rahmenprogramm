#version 430 // change later


// Structures
// =============================================================================================================
struct Matrices
{
	mat4 mvp;
};


struct FragmentInput
{
	vec2 uv;
};


struct Textures
{
	sampler2DRect tex;
};


struct Parameter
{
    vec4 paramA;
    vec4 paramB;
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
uniform Textures textures;

subroutine uniform FragmentProgram fragmentprogram;


uniform Parameter parameter;


uniform vec2 offsets3x3[9] = vec2[]
    (
        vec2(-1,  -1), vec2(-1,  0), vec2(-1, 1),
		vec2( 0,  -1), vec2( 0,  0), vec2( 0, 1),
		vec2( 1,  -1), vec2( 1,  0), vec2( 1, 1)
	);


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
layout (index = 0) subroutine (FragmentProgram) void passthrough()
{
    out_color = texture(textures.tex, Input.uv);
}


layout (index = 1) subroutine (FragmentProgram) void mean_value()
{
    vec4 texel = vec4(0.f, 0.f, 0.f, 1.f);

    //   1  1  1
    //   1  1  1
    //   1  1  1

    for(int i = 0; i < 9; ++i)
        texel += texture(textures.tex, Input.uv + offsets3x3[i]);

    out_color = texel / 9.f;
}


layout (index = 2) subroutine (FragmentProgram) void brightness_contrast()
{
    out_color = texture(textures.tex, Input.uv);
}


layout (index = 3) subroutine (FragmentProgram) void sharpen()
{
    out_color = texture(textures.tex, Input.uv);
}


layout (index = 4) subroutine (FragmentProgram) void dilataion()
{
    out_color = texture(textures.tex, Input.uv);
}


layout (index = 5) subroutine (FragmentProgram) void erosion()
{
    out_color = texture(textures.tex, Input.uv);
}


layout (index = 6) subroutine (FragmentProgram) void gauss3x3()
{
    out_color = texture(textures.tex, Input.uv);
}


layout (index = 7) subroutine (FragmentProgram) void gauss5x5()
{
    out_color = texture(textures.tex, Input.uv);
}


layout (index = 8) subroutine (FragmentProgram) void gauss7x7()
{
    out_color = texture(textures.tex, Input.uv);
}


layout (index = 9) subroutine (FragmentProgram) void gauss7x7vertical()
{
    out_color = texture(textures.tex, Input.uv);
}


layout (index = 10) subroutine (FragmentProgram) void gauss7x7horizontal()
{
    out_color = texture(textures.tex, Input.uv);
}


layout (index = 11) subroutine (FragmentProgram) void sobel()
{
    out_color = texture(textures.tex, Input.uv);
}


layout (index = 12) subroutine (FragmentProgram) void laplace()
{
	out_color = texture(textures.tex, Input.uv);
}


layout (index = 13) subroutine (FragmentProgram) void median()
{
    out_color = texture(textures.tex, Input.uv);
}
// =============================================================================================================