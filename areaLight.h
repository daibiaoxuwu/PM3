#ifndef AREALIGHT_H
#define AREALIGHT_H

#include "light.h"

class AreaLight : public Light {
	Vector3 O , Dx , Dy;//O��Դ���ĵ�λ��
	//,dx���ι�Դ��x����,dy���ι�Դ��y����,��Դ�����꣬�䳤�������壨���᳤��
public:
	AreaLight() : Light() {}
	~AreaLight() {}
	
	Vector3 GetO() { return O; }
	void Input( std::string , std::stringstream& );
	LightCollider Collide(  Ray ray );
	Color GetIrradiance( Collider* collider , Primitive* primitive_head , int shade_quality , int* hash );
	Photon EmitPhoton();
};

#endif
