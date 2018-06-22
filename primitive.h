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
	Primitive* pri;//��ײ��������
	
public:
	double dist;//���ߵ���ײ��ľ���---��ײǰ�����߹�����
	bool crash, front;//crash�Ƿ���ײ��;front��ײ�����巨��������ı���--�Ƿ������������ײ
	Vector3 N, C, I;//��ײ���ķ�ƽ�淨����N����ײ������C,I�������ߵķ���
	
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
	
	virtual void PreTreatment() {}//�ڶ�����������Ϣ����ã�����Ԥ����
	virtual void Input( std::string , std::stringstream& );//�����ж�ȡ���ݣ�����var��ʾҪ��ȡ�ı���
	virtual Collider Collide(  Ray ray ) {}
	//�������(ray_O,ray_V)����Ʒ��ײ�������ײ���ݴ�����ײ�����У������Ƿ�����ײ
	virtual Color GetTexture(Vector3 C);
	//����ײ��c����ײ�����ײ������������ɫ
};

#endif
