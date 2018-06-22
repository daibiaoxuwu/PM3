#ifndef SPHERE_H
#define SPHERE_H

#include "primitive.h"

class Sphere : public Primitive {
	Vector3 O , De , Dc;//O��������,dedc��������ᣨz�����֮��ֱ�ķ���Ϊ0���ᣩ�����ڼ�������
	double R;//��İ뾶

public:
	Sphere();
	~Sphere() {}

	void Input( std::string , std::stringstream& );
	Collider Collide(  Ray ray );
	Color GetTexture(Vector3 C);
};

#endif
