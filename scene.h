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
	Primitive* primitive_head;//��Ʒ��������ͷ
	Light* light_head;//��Դ��������ͷ
	Camera* camera;//�������ָ��
	Color background_color;//������ɫ
	

public:
	Scene();
	~Scene();
	
	Primitive* GetPrimitiveHead() { return primitive_head; }
	Light* GetLightHead() { return light_head; }
	Camera* GetCamera() { return camera; }
	Color GetBackgroundColor() { return background_color; }

	void AddPrimitive( Primitive* );
	void AddLight( Light* );
	void CreateScene( std::string file );//���ļ��ж��볡��������
	Collider* FindNearestCollide( Ray ray );
	//�������(ray_O,ray_V)�����ĵ�һ�����壬����ײ����Collider��װ����
	LightCollider* FindNearestLight( Ray ray );
	//�������(ray_O,ray_V)�����ĵ�һ����Դ���Թ�Դ��ײ����LightCollider��װ����

};

#endif
