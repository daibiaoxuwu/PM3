#ifndef SPHERE_H
#define SPHERE_H

#include "primitive.h"

class Sphere : public Primitive {
	Vector3 O , De , Dc;//O球心坐标,dedc球的坐标轴（z轴和与之垂直的辐角为0的轴），用于计算纹理
	double R;//球的半径

public:
	Sphere();
	~Sphere() {}

	void Input( std::string , std::stringstream& );
	Collider Collide(  Ray ray );
	Color GetTexture(Vector3 C);
};

#endif
