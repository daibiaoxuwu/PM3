#ifndef PLANE_H
#define PLANE_H

#include "primitive.h"

class Plane : public Primitive {
	Vector3 N , Dx , Dy;//N平面法向量
	//dx,dy平面的坐标轴，用于计算纹理，纹理图片在场景中的长宽
	double R;//平面与原点距离

public:
	Plane();
	~Plane() {}

	void Input( std::string , std::stringstream& );
	Collider Collide(  Ray ray );
	Color GetTexture(Vector3 C);
};

#endif
