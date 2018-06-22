#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "color.h"
#include "vector3.h"
#include "material.h"
#include <iostream>
#include <sstream>
#include <string>

extern const double INF;
extern const double EPS;
extern const double PI;

class Primitive;
class Ray {	
public:
	Vector3 dir; 
	Vector3 pos; 
	Ray(Vector3 pos1,Vector3 dir1 ):  pos(pos1),dir(dir1) {}
	Ray(Vector3 pos1  ):  pos(pos1)  {}
	Ray( )  {}
	~Ray() {}
};
class Collider {
	Primitive* pri;//碰撞到的物体
	
public:
	double dist;//射线到碰撞点的距离---碰撞前光线走过距离
	bool crash, front;//crash是否碰撞到;front碰撞到物体法向量方向的表面--是否从物体正面碰撞
	Vector3 N, C, I;//碰撞处的法平面法向量N和碰撞点坐标C,I入射射线的方向
	
	Collider() {
		pri = NULL;
		crash = false;
	}
	~Collider() {}

	Primitive* GetPrimitive() { return pri; }
	void SetPrimitive(Primitive* _pri) { pri = _pri; }
};

class Primitive {
protected:
	int sample;
	Material* material;
	Primitive* next;

public:
	Primitive();
	Primitive( const Primitive& );
	~Primitive();
	
	void SetSample(int _sample) { sample = _sample; }
	int GetSample() { return sample; }
	void SetMaterial(Material* _material) { material = _material; }
	Material* GetMaterial() { return material; }
	Primitive* GetNext() { return next; }
	void SetNext( Primitive* primitive ) { next = primitive; }
	
	virtual void PreTreatment() {}//在读入完物体信息后调用，进行预处理
	virtual void Input( std::string , std::stringstream& );//从流中读取数据，其中var提示要读取的变量
	virtual Collider Collide(  Ray ray ) {}
	//计算光线(ray_O,ray_V)到物品碰撞情况，碰撞数据存在碰撞器类中，返回是否有碰撞
	virtual Color GetTexture(Vector3 C);
	//在碰撞点c，碰撞点的碰撞物表面纹理的颜色
};

#endif
