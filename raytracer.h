#ifndef RAYTRACER_H
#define RAYTRACER_H

#include"bmp.h"
#include"scene.h"
#include"photontracer.h"
#include<string>

extern const int PM_MAX_THREADS;
extern const int MAX_DREFL_DEP;
extern const int MAX_RAYTRACING_DEP;
extern const int HASH_FAC;
extern const int HASH_MOD;

class Raytracer {
	std::string input , output;//�����ļ�����txt�ļ���������ļ�����bmp�ļ���
	Scene* scene;//����Ⱦ�ĳ���
	Camera* camera;//�����е�����ͷ
	Photonmap* photonmap;//����ͼ 
	
	int  H,W;
	int** sample;
	
	
	Color CalnDiffusion( Collider* collider , int* hash, int rc, Color weight);
	//����������ɫ�⣬hash���ڼ�¼���߾�������Ʒ��hashֵ��ͬ����������Ҫʹ�ó���ȡ��ȥ��ݣ�
	// ��rcΪ��Ӧ���ص㣬weightΪ�����߶����ص�Ȩ��
	Color CalnReflection( Collider* collider, Ray ray, int dep , bool refracted , int* hash, int rc, Color weight);
//���㷴���õ���ɫ�⣬����ray_V��������ߵķ���dep�ǵ���������
//refracted��ʾ�����Ƿ������������	
	Color CalnRefraction( Collider* collider, Ray ray, int dep , bool refracted , int* hash, int rc, Color weight);
//���������õ���ɫ��
	Color RayTracing(Ray ray, int dep, bool refracted, int* hash, int rc, Color weight);	
//����׷�ٵ���Ҫ�����������νӲ�ͬ��εĹ���

	void GenerateImage(std::string file);//��camera�е����ݵ�����fileͼ���ļ���

public:
	Raytracer();
	~Raytracer(){
		if(scene!=NULL) {	
			delete  scene;
			scene=NULL;
		}
	}
	void Sampling(int threadID ); //		����׷�ٲ�����threadID���̱߳��  
	void reSampling(int threadID );//����׷�ٳ���������ȥ��ݣ�	

	void SetInput( std::string file ) { input = file; }
	void SetOutput( std::string file ) { output = file; }
	void Run();//��ʼ��Ⱦ����������txt�ļ��е����ݣ���Ⱦ��ͼ�񵼳������bmp�ļ���
};

#endif
