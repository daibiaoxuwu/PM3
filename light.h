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
	double dist;//��ײǰ�����߹��ľ���

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
	int sample;//ÿ����Դ���е�һ���������������Ⱦʱhash�ж��Ƿ�����о��
	Color color;//��Դ��ɫ��
	Light* next;///��һ����Դ��ָ�룬���ﲻ��ɾ���ڴ棨���������Դ�����ڳ����б�ɾ������

	Color CalnIrradiance( Collider* collider , Vector3 V , int* hash );
//����һ�����Ӷ���ײ��ɫ���Ӱ��
public:
	Light();
	~Light() {}
	
	int GetSample() { return sample; }
	Color GetColor() { return color; }
	Light* GetNext() { return next; }
	void SetNext( Light* light ) { next = light; }

	virtual void Input( std::string , std::stringstream& );
	virtual Vector3 GetO() = 0;//�õ���Դ����λ��
	virtual LightCollider Collide(  Ray ray ) = 0;
	//�������(ray_O,ray_V)����Ʒ��ײ�������ײǰ�߹�������� dist�У������Ƿ�����ײ
	virtual Color GetIrradiance( Collider* collider , Primitive* primitive_head , int shade_quality , int* hash ) = 0;
	//������ײ��Ĺ��նȳ���diff��spec�ı��ʣ�������Ӱ
	virtual Photon EmitPhoton() = 0;//����õ�һ���ձ���������Ĺ���
};

#endif
