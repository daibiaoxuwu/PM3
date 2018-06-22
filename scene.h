#ifndef SCENE_H
#define SCENE_H

#include "color.h"
#include "vector3.h"
#include "plane.h"
#include "sphere.h"
//#include "rectangle.h"
//#include "triangle.h"
#include "pointLight.h"
#include "areaLight.h"
#include "camera.h"
#include <string>
#include <fstream>
#include <sstream>

class Scene {
	Primitive* primitive_head;//物品集的链表头
	Light* light_head;//光源集的链表头
	Camera* camera;//摄像机的指针
	Color background_color;//背景颜色
	

public:
	Scene();
	~Scene();
	
	Primitive* GetPrimitiveHead() { return primitive_head; }
	Light* GetLightHead() { return light_head; }
	Camera* GetCamera() { return camera; }
	Color GetBackgroundColor() { return background_color; }

	void AddPrimitive( Primitive* );
	void AddLight( Light* );
	void CreateScene( std::string file );//从文件中读入场景的数据
	Collider* FindNearestCollide( Ray ray );
	//计算光线(ray_O,ray_V)碰到的第一个物体，以碰撞器类Collider包装返回
	LightCollider* FindNearestLight( Ray ray );
	//计算光线(ray_O,ray_V)碰到的第一个光源，以光源碰撞器类LightCollider包装返回

};

#endif
