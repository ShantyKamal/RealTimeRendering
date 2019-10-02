// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated colour received from vertex stage
in vec2 uv; //texture coordinates 
// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

///////////////////
//horizontal sobel
mat3 sx = mat3( 
    1.0, 2.0, 1.0, 
    0.0, 0.0, 0.0, 
   -1.0, -2.0, -1.0 
);
//vertical sobel
mat3 sy = mat3( 
    1.0, 0.0, -1.0, 
    2.0, 0.0, -2.0, 
    1.0, 0.0, -1.0 
);
/////////////////////




uniform sampler2DRect imageTexture;

void main(void)
{
    // write colour output without modification
	//FragmentColour = texture(imageTexture, uv[0]);
	
	
}
