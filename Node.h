/*
 * Node.h
 * Parent class for nodes in a scene graph
 *  Created on: Dec 6, 2018
 *      Author: Shanty1
 */

#ifndef NODE_H_
#define NODE_H_
#include <glm/glm.hpp>
#include <vector>
#include "Geometry.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


class Node {
public:
	//set to identity matrix at first
	const char* name;
	glm::mat4 idMatrix = glm::mat4(1.0f);
	glm::mat4 localMatrix = idMatrix;
	glm::mat4 worldMatrix = idMatrix;
	std::vector<Node> children;
	//Node* parent;
	Geometry body;
	float radius;
	glm::vec3 centre;
	//set texture name to empty string at first
	const char* textureImg;

	//constructors
		Node(const char* textureImg, float radius, glm::vec3 centre);
		virtual ~Node();
		//void setParent(Node* myParent);
		void addChildren(Node child);
		void updateWorldMatrix(glm::mat4 parentWorldMatrix);
		void updateLocalMatrix(glm::mat4 translationMx);
		void assignBody(Geometry primitiveShape);


		void setName(const char* name);
};







#endif /* NODE_H_ */
