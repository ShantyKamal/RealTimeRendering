/*
 * Node.cpp
 *
 *  Created on: Dec 6, 2018
 *      Author: Shanty1
 */


#include "Node.h"
#include <iostream>

Node::Node(const char* textureImg, float radius, glm::vec3 centre){
	this->textureImg = textureImg;
	this->radius = radius;
	this->centre = centre;
	//change local matrix to centre of object
		glm::mat4 translationMx = glm::translate((this->idMatrix), centre);
		this->localMatrix = (this->localMatrix) * translationMx;
		this->textureImg = textureImg;
		name = "";


}
void Node::updateLocalMatrix(glm::mat4 translationMx)
{

	this->localMatrix = (this->localMatrix) * translationMx;
}
void Node::setName(const char* name){
	this->name = name;
}




void Node::addChildren(Node child)
{
	this->children.push_back(child);
}

void Node::updateWorldMatrix(glm::mat4 parentWorldMatrix){
	this->worldMatrix = parentWorldMatrix * (this->localMatrix);
	if (this->children.size() > 0 ) {
	for (Node child : this->children)
	{
		//std::cout<<"x\n";
		//std::cout<<child.name;
		child.updateWorldMatrix(this->worldMatrix);

	}
	}

}

void Node::assignBody(Geometry primitiveShape){
	this->body = primitiveShape;
	this->body.assignTexture(this->textureImg);
}
Node::~Node(){}
