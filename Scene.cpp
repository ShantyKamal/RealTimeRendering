/*
 * Scene.cpp
 *
 *  Created on: Sep 10, 2018
 *  Author: John Hall
 */

#include "Scene.h"
#include <stdio.h>
#include <iostream>
#include "Node.h"

#include "RenderingEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<string>
#include "texture.h"
int sceneNum = 0; //monkey scene (1)
int rotationIt = 0; //right side up

Scene::Scene(RenderingEngine* renderer, GLFWwindow* window) : renderer(renderer), window(window), theta(0.f), phi(0.f) {
	//  *** start scene generation here ***
	initScene();
	//randomScene();

}


void Scene::initScene()
{
	//initialize the texture uniform location
	//glUseProgram(renderer->shaderProgram);
	//textureUniformLoc = glGetUniformLocation(renderer->shaderProgram, "imageTexture");
	std::vector<glm::vec3> vertices;

	//radius in km from wikipedia
	//***RADII***
	float sunS = 696342;
	float earthS= 6378.1;
	float moonS= 1737.1;
	float marsS = 3397.00;
	//***DISTANCES FROM PARENT***
	float earthD = 149597890;
	float marsD = 227936640;
	float moonD = 384399;
	//initialize celestial bodies

	float x = 100;
	Node stars = Node("stars.jpg", x, glm::vec3(0,0,0));
			vertices = getVertices(x, glm::vec3(0,0,0));
			Geometry myBody = pushSphere(x, vertices);
			stars.assignBody(myBody);


	Node sun = Node("sunTexture.jpg", log(sunS/100)/log(5), glm::vec3(-40,0,0));
				sun.assignBody(createBody(sunS, sun.centre));


	Node earth = Node("earthTexture.jpg",log(earthS/100)/log(2) , glm::vec3((log(earthD)/log(2))*2 ,0,0));
				earth.assignBody(createBody(earthS, earth.centre));
	Node moon = Node("moonTexture.jpg", log(moonS/100)/log(2), glm::vec3((log(moonD)/log(2))*2, 0,0));
				moon.assignBody(createBody(moonS, moon.centre));
	Node mars = Node("marsTexture.jpg", log(marsS/100)/log(2), glm::vec3((log(marsD)/log(2))*2, 0,0));
				mars.assignBody(createBody(marsS, mars.centre));

	//***RELATIONSHIPS***
	earth.setName("earth"); sun.setName("sun"); moon.setName("moooooon"); mars.setName("mars");
	sun.addChildren(earth);
	sun.addChildren(mars);
	earth.addChildren(moon);


	//add bodies we just created to the current scene
	celestialBodies.push_back(sun); celestialBodies.push_back(earth);
	celestialBodies.push_back(mars); celestialBodies.push_back(moon); celestialBodies.push_back(stars);


}
Geometry Scene::createBody(float size, glm::vec3 centre)
{
	std::vector<glm::vec3> vertices;
	//call with scaled size to generate the sphere of the body you want to create
	vertices = getVertices(log(size/100)/log(5) , centre);
	Geometry myBody = pushSphere( log(size/100)/log(5), vertices);
	return myBody;

}

std::vector<glm::vec3> Scene::getVertices(float s,glm::vec3 origin)
{
	std::vector<glm::vec3> vertices;
	float v1, v2, v3;
	char prefix[2];
		FILE *fp;
		fp = fopen("vertices.txt","r");
		while(!(feof(fp)))
			{
			fscanf(fp, "%s %f %f %f", prefix, &v1, &v2, &v3);
			vertices.push_back(glm::vec3(s*v1+origin.x,s*v2+origin.y, (s*v3)+origin.z));
			}

		    fclose(fp);
	return vertices;
}

std::vector<glm::vec2> Scene::getTextureCoords()
{
	std::vector<glm::vec2> textureUVs;
	float u, v;
	char prefix[2];
		FILE *fp;
		fp = fopen("textCoords.txt","r");
		while(!(feof(fp)))
			{
			fscanf(fp, "%s %f %f", prefix, &u, &v);
			textureUVs.push_back(glm::vec2(u, v));
			}

		    fclose(fp);

	return textureUVs;
}

std::vector<glm::vec3> Scene::getNormalCoords()
{
	std::vector<glm::vec3> normals;
	float x, y, z;
	char prefix[2];
		FILE *fp;
		fp = fopen("vertNormals.txt","r");
		while(!(feof(fp)))
			{
			fscanf(fp, "%s %f %f &f", prefix, &x, &y, &z);
			normals.push_back(glm::vec3(x, y, z));
			}

		    fclose(fp);

	return normals;
}

Geometry Scene::pushSphere(float radius, std::vector<glm::vec3> vertices)
{
	Geometry triangle;
	std::vector<glm::vec2> textureCoord = getTextureCoords();
	//std::vector<glm::vec3> normals = getNormalCoords();

	glm::vec3 v1, v2, v3;
	char prefix[1]; //f
	//int t1, t2, t3;
		int t1V, t1Txtr, t1N; //vertex 1
		int t2V, t2Txtr, t2N; //vertex 2
		int t3V, t3Txtr, t3N; //vertex 3
		char slash;
		FILE *fp;
		fp = fopen("actualTriangles.txt","r");

		int i = 0;
		while(!(feof(fp)))
			{ i++;
			fscanf(fp, "%s %d%c%d%c%d %d%c%d%c%d %d%c%d%c%d", prefix,
					&t1V, &slash ,&t1Txtr,&slash, &t1N,
					&t2V, &slash, &t2Txtr, &slash, &t2N,
					&t3V, &slash,  &t3Txtr,&slash,  &t3N);
			//retrieve the vertices specified for current triangle
			v1 = vertices.at(t1V - 1);
			v2 = vertices.at(t2V - 1);
			v3 = vertices.at(t3V - 1);
			//push this triangle onto geometry objects
			triangle.verts.push_back(v1*radius);
			triangle.verts.push_back(v2*radius);
			triangle.verts.push_back(v3*radius);
			//push color for these three vertices
			triangle.uvs.push_back(textureCoord.at(t1Txtr - 1));
			triangle.uvs.push_back(textureCoord.at(t2Txtr - 1));
			triangle.uvs.push_back(textureCoord.at(t3Txtr - 1));
			//push normals
			/*triangle.normals.push_back(normals.at(t1N -1));
			triangle.normals.push_back(normals.at(t2N -1));
			triangle.normals.push_back(normals.at(t3N -1));*/
			}

		    fclose(fp);
		    triangle.drawMode = GL_TRIANGLES;



		    		//Construct vao and vbos for the triangle
		    		RenderingEngine::assignBuffers(triangle);


		    		//Send the triangle data to the GPU
		    		//Must be done every time the triangle is modified in any way, ex. verts, colors, normals, uvs, etc.
		    		RenderingEngine::setBufferData(triangle);


		    		//Add the triangle to the scene objects
		    		objects.push_back(triangle);
		    		return triangle;
}


void Scene::triangleScene()
{
	//Create a single triangle
		//Additional triangles can be created by pushing groups of three more vertices into the verts vector
		Geometry triangle;

		//base
		triangle.verts.push_back(glm::vec3(-1.0f, 0.0f, -1.0f));
		triangle.verts.push_back(glm::vec3(1.0f, 0.0f, -1.0f));
		triangle.verts.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

		//left
		triangle.verts.push_back(glm::vec3(-1.0f, 0.0f, -1.0f));
		triangle.verts.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		triangle.verts.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

		//front
		triangle.verts.push_back(glm::vec3(-1.0f, 0.0f, -1.0f));
		triangle.verts.push_back(glm::vec3(1.0f, 0.0f,  -1.0f));
		triangle.verts.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

		//right
		triangle.verts.push_back(glm::vec3(1.0f, 0.0f, -1.0f));
		triangle.verts.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
		triangle.verts.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

		//Colors are stored per vertex in the order of the vertices
		triangle.colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		triangle.colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		triangle.colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

		triangle.colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
		triangle.colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
		triangle.colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

		triangle.colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		triangle.colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		triangle.colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

		triangle.colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
		triangle.colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
		triangle.colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

		triangle.drawMode = GL_TRIANGLES;

		//Construct vao and vbos for the triangle
		RenderingEngine::assignBuffers(triangle);

		//Send the triangle data to the GPU
		//Must be done every time the triangle is modified in any way, ex. verts, colors, normals, uvs, etc.
		RenderingEngine::setBufferData(triangle);

		//Add the triangle to the scene objects
		objects.push_back(triangle);
}

void Scene::selectScene(){
	rotationIt=0;
	objects.clear();
	triangles.verts.clear();
	triangles.uvs.clear();
	++sceneNum;
	//if (sceneNum<=1){sceneNum=1;} //cap scene changes once start is reached
	//if (sceneNum>=6){sceneNum=6;} //cap scene changes once end is reached
	sceneNum = sceneNum%6;
	MyTexture texture;
	//select texture for current scene
	switch (sceneNum)
	{
	    case 1: InitializeTexture(&texture, "image1.png", GL_TEXTURE_RECTANGLE);
	        break;
	    case 2: InitializeTexture(&texture, "image2.png", GL_TEXTURE_RECTANGLE);
	        break;
	    case 3: InitializeTexture(&texture, "image3.jpg", GL_TEXTURE_RECTANGLE);
	    		break;
	    case 4:InitializeTexture(&texture, "image4.jpg", GL_TEXTURE_RECTANGLE);
	    		break;
	    case 5: InitializeTexture(&texture, "image5.png", GL_TEXTURE_RECTANGLE);
	    	    		break;
	    	case 0: InitializeTexture(&texture, "image6.png", GL_TEXTURE_RECTANGLE);
	    	    		break;
	    default: std::cout<<"\n no image selected.\n";
	}

		//Shaders need to be active to load uniforms
			glUseProgram(renderer->shaderProgram);

		//Get identifiers for uniforms
		GLuint uniformLocation = glGetUniformLocation(renderer->shaderProgram, "imageTexture");

			//Set which texture unit the texture is bound to
			glActiveTexture(GL_TEXTURE0);
			//Bind the texture to GL_TEXTURE0
			glBindTexture(GL_TEXTURE_RECTANGLE, texture.textureID);

			//Load texture unit number into uniform
			glUniform1i(uniformLocation, 0);


				glm::mat4 idMatrix = glm::mat4(1.0f);
				GLuint rotationLoc = glGetUniformLocation(renderer->shaderProgram, "rotationMatrix");
				glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, glm::value_ptr(idMatrix));
				GLuint translationLoc = glGetUniformLocation(renderer->shaderProgram, "translationMatrix");
				glUniformMatrix4fv(translationLoc, 1, GL_FALSE, glm::value_ptr(idMatrix));
				GLuint offsetLoc = glGetUniformLocation(renderer->shaderProgram, "translateToCenter");
				glUniformMatrix4fv(offsetLoc, 1, GL_FALSE, glm::value_ptr(idMatrix));
				GLuint scaleLoc = glGetUniformLocation(renderer->shaderProgram, "scaleMatrix");
				glUniformMatrix4fv(scaleLoc, 1, GL_FALSE, glm::value_ptr(idMatrix));

			//glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, glm::value_ptr(rotationMatrix));

			//rotate();
			//scale();
			//translate();

			if(renderer->CheckGLErrors()) {
				std::cout << "Texture creation faileeeed" << std::endl;
			}
			//maintain aspect ratios
			float longerSide = std::max(texture.width, texture.height);
			float shorterSide = std::min(texture.width, texture.height);
			float aspectRatio = shorterSide/longerSide;
			// three vertex positions and associated colours of a triangles
			if (longerSide == texture.height){
				triangles.verts.push_back(glm::vec3( -aspectRatio, -1.0f, 1.0f));
				triangles.verts.push_back(glm::vec3( aspectRatio,  -1.0f, 1.0f));
				triangles.verts.push_back(glm::vec3( aspectRatio, 1.0f, 1.0f));
				triangles.verts.push_back(glm::vec3( -aspectRatio, -1.0f, 1.0f));
				triangles.verts.push_back(glm::vec3( aspectRatio, 1.0f, 1.0f));
				triangles.verts.push_back(glm::vec3( -aspectRatio, 1.0f, -1.0f));
			}
			else {
				triangles.verts.push_back(glm::vec3( -1.0f, -aspectRatio, 1.0f));
				triangles.verts.push_back(glm::vec3( 1.0f,  -aspectRatio, 1.0f));
				triangles.verts.push_back(glm::vec3( 1.0f, aspectRatio, 1.0f));
				triangles.verts.push_back(glm::vec3( -1.0f, -aspectRatio, 1.0f));
				triangles.verts.push_back(glm::vec3( 1.0f, aspectRatio, 1.0f));
				triangles.verts.push_back(glm::vec3( -1.0f, aspectRatio, -1.0f));
			}
				triangles.drawMode = GL_TRIANGLES;

				triangles.uvs.push_back(glm::vec2( 0.0f, 0.0f));
				triangles.uvs.push_back(glm::vec2( float(texture.width), 0.f));
				triangles.uvs.push_back(glm::vec2( float(texture.width), float(texture.height)));
				triangles.uvs.push_back(glm::vec2( 0.0f, 0.0f));
				triangles.uvs.push_back(glm::vec2( float(texture.width), float(texture.height)));
				triangles.uvs.push_back(glm::vec2(0.0f, float(texture.height)));
		//Construct vao and vbos for the triangles
		RenderingEngine::assignBuffers(triangles);
		//Send the triangles data to the GPU
		//Must be done every time the triangles is modified in any way, ex. verts, colors, normals, uvs, etc.
		RenderingEngine::setBufferData(triangles);
		//Add the triangles to the scene objects
		objects.push_back(triangles);
}


void Scene::updateTranslationMatrix(double xpos, double ypos){
	//TODO: screen coords --> opengl vertex coords
	//activate shaders

	glUseProgram(renderer->shaderProgram);

	glm::mat4 idMatrix = glm::mat4(1.0f);
	glm::mat4 translationMatrix = glm::translate(idMatrix, glm::vec3(xpos, ypos, 1.0));
	//glm::mat4 undoTranslationMatrix = glm::translate(idMatrix, glm::vec3(ypos, xpos, 1.0));
	GLuint translationLoc = glGetUniformLocation(renderer->shaderProgram, "translationMatrix");
	//GLuint undoTranslationLoc = glGetUniformLocation(renderer->shaderProgram, "centreMatrix");
	glUniformMatrix4fv(translationLoc, 1, GL_FALSE, glm::value_ptr(translationMatrix));
	//glUniformMatrix4fv(undoTranslationLoc, 1, GL_FALSE, glm::value_ptr(undoTranslationMatrix));

}

void Scene::updateRotationMatrix(float theta){
	//Shaders need to be active to load uniforms
	glUseProgram(renderer->shaderProgram);
	glm::mat4 idMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::rotate(idMatrix, glm::radians(theta), glm::vec3(0.0, 0.0, 1.0));
	GLuint rotationLoc = glGetUniformLocation(renderer->shaderProgram, "rotationMatrix");
	glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
}

void Scene::rotateRight(){
	rotationIt--;
	std::cout<<"rotateRight() "<<rotationIt;
	float angle = 10.0f * rotationIt;
	updateRotationMatrix(angle);
}

void Scene::rotateLeft(){
	rotationIt++;
	std::cout<<"rotateLeft() "<<rotationIt;
	float angle = 10.0f * rotationIt;
	updateRotationMatrix(angle);
}

void Scene::updateScaleMatrix(double yoffset){
	//scroll down is pos , scroll moving upwards is negative

	glUseProgram(renderer->shaderProgram);
	glm::mat4 idMatrix = glm::mat4(1.0f);
	if (yoffset<0) //zoom in, scale up
	{yoffset = abs(yoffset);
	yoffset+=1.5;}
	else if(yoffset>0) //zoom out, scale down
	{yoffset = abs(yoffset);
	yoffset+=1.5;
	yoffset = 1/yoffset;
	}
	else //yoffset == 0
	{yoffset = 1;}
	glm::mat4 scaleMatrix = glm::scale(idMatrix, glm::vec3(abs(yoffset), abs(yoffset), 1.0));
	GLuint scaleLoc = glGetUniformLocation(renderer->shaderProgram, "scaleMatrix");
	glUniformMatrix4fv(scaleLoc, 1, GL_FALSE, glm::value_ptr(scaleMatrix));


}
Scene::~Scene() {}


//sends geometry objects to renderer
void Scene::displayScene() {
	//renderer->RenderScene(celestialBodies, window, theta, phi);
	renderer->RenderScene(celestialBodies, window, theta, phi, r);
	}

