#ifndef PHOTONTRACER
#define PHOTONTRACER

#include"scene.h"
#include"photonmap.h"


extern const int PM_MAX_THREADS;
extern const int MAX_PHOTONTRACING_DEP;

class Photontracer {
	Scene* scene;//����Ⱦ�ĳ���
	Photonmap* photonmap;//����ͼ
	int hh,ww;
	Light* light;
	double totalPower;

	void PhotonTracing( Photon , int dep , bool refracted );//����ӳ�����Ҫ�������������ν�����
	bool PhotonDiffusion( Collider* , Photon , int dep , bool refracted , double* prob );
//�жϹ����Ƿ��������䣨�����¼���������ģ��֮��refracted��ʾ�����Ƿ������������
		
	bool PhotonReflection( Collider* , Photon , int dep , bool refracted , double* prob );
	//�жϹ����Ƿ�������
	
	bool PhotonRefraction( Collider* , Photon , int dep , bool refracted , double* prob );
	//�жϹ����Ƿ�������
	
	
	
	
public:
	Photontracer();
	~Photontracer() {}
	

	void SetScene( Scene* _scene ) { scene = _scene; }
	void SetPhotonmap(Photonmap* _photonmap) { photonmap = _photonmap; }
	Photonmap* GetPhotonmap() { return photonmap; }

	Photonmap* CalnPhotonmap();//���ݳ������������ͼ	//��ʼ���㣬���ݳ������������ͼ��������Ӵ�randIDBase����ȡ��
	void Emitting(int threadID ); //��֯���ж��̵߳Ĺ���ӳ�䣬threadID���̱߳�� 
};

#endif
