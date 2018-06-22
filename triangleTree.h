#ifndef TRIANGLETREE_H
#define TRIANGLETREE_H

#include "triangle.h"
#include <string>

extern const int KD_MAX_THREADS;

class Triangle;
class Collider;

class TriangleBox {
public:
	Vector3 minPos, maxPos;
	
	TriangleBox();
	~TriangleBox() {}

	void Update(Triangle* tri);
	bool Cantain(Vector3 O);
	double CalnArea();
	double Collide(Ray ray);
};

class TriangleNode {
public:
	Triangle** tris;
	int size, plane;
	double split;
	TriangleBox box;
	TriangleNode* leftNode;
	TriangleNode* rightNode;

	TriangleNode();
	~TriangleNode();
};

class TriangleTree {
	TriangleNode* root;

	void DeleteTree(TriangleNode* node);
	void SortTriangle(Triangle** tris, int l, int r, int coord, bool minCoord);
	void DivideNode(TriangleNode* node);
	Collider TravelTree(TriangleNode* node, Ray ray);

public:
	TriangleTree();
	~TriangleTree();

	TriangleNode* GetRoot() { return root; }
	void BuildTree();
	Collider Collide(Ray ray);
};

#endif
