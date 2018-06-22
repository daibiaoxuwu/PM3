#ifndef PLANE_H
#define PLANE_H

#include "primitive.h"

class Plane : public Primitive {
	Vector3 N , Dx , Dy;//Nƽ�淨����
	//dx,dyƽ��������ᣬ���ڼ�����������ͼƬ�ڳ����еĳ���
	double R;//ƽ����ԭ�����

public:
	Plane();
	~Plane() {}

	void Input( std::string , std::stringstream& );
	Collider Collide(  Ray ray );
	Color GetTexture(Vector3 C);
};

#endif
