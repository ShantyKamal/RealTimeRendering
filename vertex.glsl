// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 VertexPosition; //vertex position
layout(location = 1) in vec2 VertexUV; //texture coords


uniform mat3x3 rotationMatrix; 
uniform mat3x3 scalingMatrix;
uniform vec3 translationMatrix; 


// output to be interpolated between vertices and passed to the fragment stage
out vec2 uv;

void main()
{
	
	vec3 newPoint = rotationMatrix * VertexPosition;
	//vec3 newPoint = scalingMatrix * rotationMatrixFromRenderingEngine * VertexPosition; 
	
    // assign vertex position WITH modification
    gl_Position = vec4(newPoint.xy, 0.0, 1.0);

    // assign output colour to be interpolated
    uv = VertexUV;
}
