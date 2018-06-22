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
	Vector3 pos , dir;//������ײλ�ú����䷽��
	Color power;//��������
	int plane;//������KD���л��ֵ�ƽ��
};

class Nearestphotons {
public:
	Vector3 pos;//����ڵ�pos�����k����
	int max_photons , found;//�����������,�Ѿ��ҵ��Ĺ�����

	bool got_heap;//�Ƿ��ѽ���
	double* dist2;//����������posƽ������
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
	Vector3 box_min , box_max;//�ռ䷶Χ��������,�������еĹ��� 

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
	//����ײ��C��ƽ�淨����N�����Ĺ�ͨ���������n�����������㣬��֤���ڰ뾶max_dist���ҵ���Щ����
		
	double GetRadius2(Collider* collider, double max_dist, int n);
	//����ײ�㴦����n�����ӵ���С�뾶����max_distȡ��Сֵ
	void Store( Photon );//��һ�����Ӵ����ڹ���ͼ��
	void Balance();//��ƽ��KD�����ڹ��Ӵ������֮��ѯ��֮ǰʹ��
};

#endif
