#ifndef LIGHT_H
#define LIGHT_H

#include"vector3.h"
#include"color.h"
#include"primitive.h"
#include"photonmap.h"
#include<sstream>
#include<string>

extern const double EPS;
extern const int HASH_MOD;
extern const double SPEC_POWER;

class Light;

class LightCollider {
	Light* light;

public:
	bool crash;
	double dist;//碰撞前光线走过的距离

	LightCollider() {
		light = NULL;
		crash = false;
	}
	~LightCollider() {}

	Light* GetLight() { return light; }
	void SetLight(Light* _light) { light = _light; }
};

class Light {
protected:
	int sample;//每个光源都有的一个随机数，用于渲染时hash判断是否可能有锯齿
	Color color;//光源的色光
	Light* next;///下一个光源的指针，这里不用删除内存（用链表储存光源集，在场景中被删除，）

	Color CalnIrradiance( Collider* collider , Vector3 V , int* hash );
//计算一个光子对碰撞点色光的影响
public:
	Light();
	~Light() {}
	
	int GetSample() { return sample; }
	Color GetColor() { return color; }
	Light* GetNext() { return next; }
	void SetNext( Light* light ) { next = light; }

	virtual void Input( std::string , std::stringstream& );
	virtual Vector3 GetO() = 0;//得到光源中心位置
	virtual LightCollider Collide(  Ray ray ) = 0;
	//计算光线(ray_O,ray_V)到物品碰撞情况，碰撞前走过距离存在 dist中，返回是否有碰撞
	virtual Color GetIrradiance( Collider* collider , Primitive* primitive_head , int shade_quality , int* hash ) = 0;
	//计算碰撞点的光照度乘上diff或spec的比率，考虑阴影
	virtual Photon EmitPhoton() = 0;//随机得到一个刚被发射出来的光子
};

#endif
