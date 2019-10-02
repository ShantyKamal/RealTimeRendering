/*
 * Program.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#include "Program.h"

#include <iostream>
#include <string>
#include <algorithm>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RenderingEngine.h"
#include "Scene.h"

double lastPosWy, lastPosWx = 0;


Program::Program() {
	setupWindow();
}

Program::~Program() {
	//Must be cleaned up in the destructor because these are allocated to the heap
	delete renderingEngine;
	delete scene;
}

void Program::start() {
	renderingEngine = new RenderingEngine();
	scene = new Scene(renderingEngine, window);

	//Main render loop
	while(!glfwWindowShouldClose(window)) {
		scene->displayScene();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}

void Program::setupWindow() {
	//Initialize the GLFW windowing system
	if (!glfwInit()) {
		std::cout << "ERROR: GLFW failed to initialize, TERMINATING" << std::endl;
		return;
	}

	//Set the custom error callback function
	//Errors will be printed to the console
	glfwSetErrorCallback(ErrorCallback);

	//Attempt to create a window with an OpenGL 4.1 core profile context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int width = 1200;
	int height = 800;
	window = glfwCreateWindow(width, height, "CPSC 453 OpenGL A5", 0, 0);
	if (!window) {
		std::cout << "Program failed to create GLFW window, TERMINATING" << std::endl;
		glfwTerminate();
		return;
	}
	//you get null pointer error if you ommit this line:
	glfwSetWindowUserPointer(window, this);
	//Set the custom function that tracks key presses AND MOUSE INPUT
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetScrollCallback(window, scroll_callback);


	//Bring the new window to the foreground (not strictly necessary but convenient)
	glfwMakeContextCurrent(window);

	//Intialize GLAD (finds appropriate OpenGL configuration for your system)
	if (!gladLoadGL()) {
		std::cout << "GLAD init failed" << std::endl;
		return;
	}

	//Query and print out information about our OpenGL environment
	QueryGLVersion();
}



void Program::QueryGLVersion() {
	// query opengl version and renderer information
	std::string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	std::string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	std::string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

	std::cout << "OpenGL [ " << version << " ] "
		<< "with GLSL [ " << glslver << " ] "
		<< "on renderer [ " << renderer << " ]" << std::endl;
}

void ErrorCallback(int error, const char* description) {
	std::cout << "GLFW ERROR " << error << ":" << std::endl;
	std::cout << description << std::endl;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//Key codes are often prefixed with GLFW_KEY_ and can be found on the GLFW website
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	Program* program = (Program*)glfwGetWindowUserPointer(window);
	Scene* s = program->getScene();

	if ((key == GLFW_KEY_LEFT) && ((action == (GLFW_REPEAT)) || (action == GLFW_PRESS))) {
		s->theta +=5;
		//program->getScene()->rotateLeft();
	}
	if ((key == GLFW_KEY_RIGHT) &&  ( (action == (GLFW_REPEAT)) || (action == GLFW_PRESS)) ) {
		s->theta -= 5;
		//program->getScene()->rotateRight();
	}

	if ((key == GLFW_KEY_DOWN) &&  ((action == (GLFW_REPEAT)) || (action == GLFW_PRESS))) {
			s->phi -= 5;
			//program->getScene()->rotateRight();
		}

	if ((key == GLFW_KEY_UP) &&  ((action == (GLFW_REPEAT)) || (action == GLFW_PRESS))) {
				s->phi +=5;
				//program->getScene()->rotateRight();
			}


	//zoom in/out with camera
	if ((key == GLFW_KEY_O) &&  ((action == (GLFW_REPEAT)) || (action == GLFW_PRESS))) {
		s->r = std::min((s->r)+10,1000);
	}
	if ((key == GLFW_KEY_I) &&  ((action == (GLFW_REPEAT)) || (action == GLFW_PRESS))) {
			s->r = std::fmax((s->r)-10,10);
			std::cout<<s->r<<"\n";
		}

}



void cursor_position_callback(GLFWwindow* window, double xWindowPos, double yWindowPos){

	Program* program = (Program*)glfwGetWindowUserPointer(window);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	  {
		lastPosWx = xWindowPos;
		lastPosWy = yWindowPos;
	     return;
	  }
	//offset?
	//xWindowPos = abs(lastPosWx-xWindowPos);
	//yWindowPos = abs(lastPosWy-yWindowPos);
	//lastPosWx = xWindowPos;
	//lastPosWy = yWindowPos;
	//window pos --> opengl coords
	double xcoord;
	if (xWindowPos >= 300) { xcoord = (xWindowPos-300)/300; }
	else if (xWindowPos<300) { xcoord = (xWindowPos/300)-1; }
	double ycoord;
	if (yWindowPos<300) { ycoord = 1 - (yWindowPos/300); }
	else if (yWindowPos >= 300) { ycoord = -(yWindowPos-300)/300; }
	std::cout<<"window-xpos: "<<xWindowPos<<"  xCoord: "<<xcoord<<"  window-ypos: "<<yWindowPos<<"  yCoord: "<<ycoord<<"\n";
	//get distance moved
	program->getScene()->updateTranslationMatrix( xcoord, ycoord);

}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	Program* program = (Program*)glfwGetWindowUserPointer(window);
	std::cout<<yoffset<<"\n";
	program->getScene()->updateScaleMatrix(yoffset);

}
