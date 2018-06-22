#ifndef PHOTONMAP_H
#define PHOTONMAP_H

#include"vector3.h"
#include"color.h"
#include"primitive.h"

extern const double PI;
extern const double INF;

class Collider;

class Photon {
public:
	Vector3 pos , dir;//光子碰撞位置和入射方向
	Color power;//光子能量
	int plane;//光子在KD树中划分的平面
};

class Nearestphotons {
public:
	Vector3 pos;//求关于点pos的最近k光子
	int max_photons , found;//最大保留光子数,已经找到的光子数

	bool got_heap;//是否已建堆
	double* dist2;//最近光子与点pos平方距离
	Photon** photons;

	Nearestphotons();
	~Nearestphotons() {
		delete[] dist2;
		delete[] photons;
	}

};

class Photonmap {
	int max_photons , stored_photons , emit_photons;
	Photon* photons;
	Vector3 box_min , box_max;//空间范围的两个角,包含所有的光子 

	void BalanceSegment( Photon* porg , int index , int st , int en );

	void MedianSplit( Photon* porg , int st , int en , int med , int axis );

	void LocatePhotons( Nearestphotons* np , int index ); //called by index = 1

public:
	Photonmap(int size)  {
		max_photons = size;
		stored_photons = 0;
		photons = new Photon[size + 1];
		box_min = Vector3( INF , INF , INF );
		box_max = Vector3( -INF , -INF , -INF );
	}
	~Photonmap() {
		delete[] photons;
	}
	
	int GetStoredPhotons() { return stored_photons; }
	void SetEmitPhotons( int photons ) { emit_photons = photons; }

	Color GetIrradiance( Collider* collider , double max_dist , int n );
	//求碰撞点C（平面法向量N）处的光通量，用最近n个光子来估算，保证能在半径max_dist中找到这些光子
		
	double GetRadius2(Collider* collider, double max_dist, int n);
	//求碰撞点处包含n个光子的最小半径，与max_dist取较小值
	void Store( Photon );//把一个光子储存在光子图中
	void Balance();//建平衡KD树，在光子储存完毕之后，询问之前使用
};

#endif
