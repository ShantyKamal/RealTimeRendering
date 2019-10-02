/*
 * RenderingEngine.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#include "RenderingEngine.h"
#include "Node.h"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

//cpp file purposely included here because it just contains some global functions
#include "ShaderTools.h"

RenderingEngine::RenderingEngine() {
	shaderProgram = ShaderTools::InitializeShaders();
	if (shaderProgram == 0) {
		std::cout << "Program could not initialize shaders, TERMINATING" << std::endl;
		return;
	}

}

RenderingEngine::~RenderingEngine() {}

void RenderingEngine::RenderScene(const std::vector<Node> celestialBodies,GLFWwindow* window, float theta, float phi, int r)
{
	//Clears the screen to a dark grey background
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	////////////////////////////////////////////////////////////////////////
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000000.0f);

	//float r = 400;
	float x, y, z, t;
	theta *= 3.14f / 180.f;
	phi *= 3.14f / 180.f;
	x = r * sin(theta);
	y = r * cos(theta);

/*	x = r*cos(theta)*sin(phi);
	y = r*sin(theta)*sin(phi);
	z = r*cos(phi);				*/

	t = r*cos(phi);   // distance to y-axis after being rotated up
	y = r*sin(phi);

	z = t*cos(theta);
	x = t*sin(theta);

	float up = 1;
	float phiAbs = fabs(phi);

	if (cos(phi) < 0)
		up = -up;

	glm::mat4 View = glm::lookAt(
		glm::vec3(x, y, z), // Camera is at (x,y,z), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, up, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	glm::mat4 idMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMx;
	std::vector<Node> bodies = celestialBodies;
	for (Node node : bodies)
	{
		//rotate each body slightly by rotating local matrix of each slightly
		rotationMx = glm::translate(idMatrix, glm::vec3(0.5f, 0.0f, 0.0f));
		node.updateLocalMatrix(rotationMx);
		node.localMatrix = rotationMx * node.localMatrix;
		glm::mat4 m = node.localMatrix;
/*std::cout<<"\n"<<m[0][0]<<"'"<<m[0][1]<<"'"<<m[0][2]<<"'"<<m[0][3]<<"\n"<<
	  "'"<<m[1][0]<<"'"<<m[1][1]<<"'"<<m[1][2]<<"'"<<m[1][3]<<"\n"<<
      "'"<<m[2][0]<<"'"<<m[2][1]<<"'"<<m[2][2]<<"'"<<m[2][3]<<"\n"<<
      "'"<<m[3][0]<<"'"<<m[3][1]<<"'"<<m[3][2]<<"'"<<m[3][3]<<"\n\n";*/
	}

	Node sun = bodies.at(0);
	//sun.updateWorldMatrix(rotationMx);


	////////////////////////////////////////////////////////////////////////

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shaderProgram);


	//Load texture uniform
	//Shaders need to be active to load uniforms
	glUseProgram(shaderProgram);
	//Set which texture unit the texture is bound to
	glActiveTexture(GL_TEXTURE0);

	//Get identifier for uniform
	GLuint uniformLocation = glGetUniformLocation(shaderProgram, "imageTexture");
	//Load texture unit number into uniform
	glUniform1i(uniformLocation, 0);

	////////////////////////////////////////////////////////////////////////
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//Bind the texture to GL_TEXTURE0


	////////////////////////////////////////////////////////////////////////
	Geometry object;
	glm::mat4 mvp;
	GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
	for (const Node& node : bodies) {
		object = node.body;
		mvp = Projection * View * node.worldMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
		glBindTexture(GL_TEXTURE_2D, object.texture.textureID);
		glBindVertexArray(object.vao);
		glDrawArrays(object.drawMode, 0, object.verts.size());

		glBindVertexArray(0);
	}

	glUseProgram(0);

	// check for and report any OpenGL errors
	CheckGLErrors();
}

void RenderingEngine::assignBuffers(Geometry& geometry) {
	//Generate vao for the object
	//Constant 1 means 1 vao is being generated
	glGenVertexArrays(1, &geometry.vao);
	glBindVertexArray(geometry.vao);

	//Generate vbos for the object
	//Constant 1 means 1 vbo is being generated
	glGenBuffers(1, &geometry.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	//Parameters in order: Index of vbo in the vao, number of primitives per element, primitive type, etc.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	/*glGenBuffers(1, &geometry.normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);*/

	/*glGenBuffers(1, &geometry.colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.colorBuffer);
	//Parameters in order: Index of vbo in the vao, number of primitives per element, primitive type, etc.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);*/

	glGenBuffers(1, &geometry.uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);


}

void RenderingEngine::setBufferData(Geometry& geometry) {
	//Send geometry to the GPU
	//Must be called whenever anything is updated about the object
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.verts.size(), geometry.verts.data(), GL_STATIC_DRAW);

	/*glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * geometry.normals.size(), geometry.normals.data(), GL_STATIC_DRAW);*/

	/*glBindBuffer(GL_ARRAY_BUFFER, geometry.colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.colors.size(), geometry.colors.data(), GL_STATIC_DRAW);*/

	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * geometry.uvs.size(), geometry.uvs.data(), GL_STATIC_DRAW);
}

void RenderingEngine::deleteBufferData(Geometry& geometry) {
	glDeleteBuffers(1, &geometry.vertexBuffer);
	glDeleteBuffers(1, &geometry.normalBuffer);
	glDeleteBuffers(1, &geometry.colorBuffer);
	glDeleteBuffers(1, &geometry.uvBuffer);
	glDeleteVertexArrays(1, &geometry.vao);
}

bool RenderingEngine::CheckGLErrors() {
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
	{
		std::cout << "OpenGL ERROR:  ";
		switch (flag) {
		case GL_INVALID_ENUM:
			std::cout << "GL_INVALID_ENUM" << std::endl; break;
		case GL_INVALID_VALUE:
			std::cout << "GL_INVALID_VALUE" << std::endl; break;
		case GL_INVALID_OPERATION:
			std::cout << "GL_INVALID_OPERATION" << std::endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl; break;
		case GL_OUT_OF_MEMORY:
			std::cout << "GL_OUT_OF_MEMORY" << std::endl; break;
		default:
			std::cout << "[unknown error code]" << std::endl;
		}
		error = true;
	}
	return error;
}
