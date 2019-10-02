/*
 * Scene.h
 *	Class for storing objects in a scene
 *  Created on: Sep 10, 2018
 *      Author: cb-ha
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include "texture.h"
#include "Geometry.h"
#include "Node.h"


//Forward declaration of classes
//(note this is necessary because these are pointers and it allows the #include to appear in the .cpp file)
class RenderingEngine;

class Scene {
public:
	Scene(RenderingEngine* renderer, GLFWwindow* window);
	virtual ~Scene();

	//Send geometry to the renderer
	void displayScene();

	float theta;
	float phi;
	int r=400;


	//Scene change functions
	void initScene();
	void selectScene();
	void triangleScene();
	Geometry createBody(float size, glm::vec3 centre);
	Geometry pushSphere(float scale,std::vector<glm::vec3> vertices);

	std::vector<glm::vec3> getVertices( float s, glm::vec3 origin);

	std::vector<glm::vec2> getTextureCoords();
	std::vector<glm::vec3> getNormalCoords();


	void rotateRight();
	void rotateLeft();
	void updateRotationMatrix(float angle);
	void updateTranslationMatrix(double xpos, double ypos);
	void updateScaleMatrix(double yoffset);


	//list of objects in the scene
	std::vector<Geometry> objects; ///MADE IT PUBLIC
	std::vector<Node> celestialBodies;
private:
	GLFWwindow* window;
	RenderingEngine* renderer;

	Geometry triangles;


};

#endif /* SCENE_H_ */
