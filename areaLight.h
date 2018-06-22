#ifndef AREALIGHT_H
#define AREALIGHT_H

#include "light.h"

class AreaLight : public Light {
	Vector3 O , Dx , Dy;//O光源中心的位置
	//,dx矩形光源的x半轴,dy矩形光源的y半轴,光源的坐标，其长度有意义（半轴长）
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
