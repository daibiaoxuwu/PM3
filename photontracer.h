#ifndef PHOTONTRACER
#define PHOTONTRACER

#include"scene.h"
#include"photonmap.h"


extern const int PM_MAX_THREADS;
extern const int MAX_PHOTONTRACING_DEP;

class Photontracer {
	Scene* scene;//待渲染的场景
	Photonmap* photonmap;//光子图
	int hh,ww;
	Light* light;
	double totalPower;

	void PhotonTracing( Photon , int dep , bool refracted );//光子映射的主要迭代函数，起衔接作用
	bool PhotonDiffusion( Collider* , Photon , int dep , bool refracted , double* prob );
//判断光子是否作漫反射（概率事件），是则模拟之，refracted表示射线是否被折射过奇数次
		
	bool PhotonReflection( Collider* , Photon , int dep , bool refracted , double* prob );
	//判断光子是否作反射
	
	bool PhotonRefraction( Collider* , Photon , int dep , bool refracted , double* prob );
	//判断光子是否作折射
	
	
	
	
public:
	Photontracer();
	~Photontracer() {}
	

	void SetScene( Scene* _scene ) { scene = _scene; }
	void SetPhotonmap(Photonmap* _photonmap) { photonmap = _photonmap; }
	Photonmap* GetPhotonmap() { return photonmap; }

	Photonmap* CalnPhotonmap();//根据场景计算出光子图	//开始运算，根据场景计算出光子图（随机种子从randIDBase往上取）
	void Emitting(int threadID ); //组织进行多线程的光子映射，threadID是线程编号 
};

#endif
