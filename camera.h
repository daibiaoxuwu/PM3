#ifndef CAMERA_H
#define CAMERA_H

#include"vector3.h"
#include"color.h"
#include"bmp.h"
#include<string>
#include<sstream>

extern const int KD_MAX_THREADS; //�������ӳ�����KD��ʱ������߳���the maxinum threads of Building KDTree
extern const int PM_MAX_THREADS; //����ӳ�䲿�ֵ��߳���the maxinum threads of PhotonsMapping
extern const int STD_DOF_SAMPLE;//��׼�������Ⱦ����Ч���Ĳ�����

extern const std::string STD_ALGORITHM;//��׼�����ʹ�õ���Ⱦ�㷨
extern const double STD_APERTURE;//��׼����¹�Ȧ��С������0ʱ��Ⱦ����Ч��
extern const double STD_FOCAL_LEN;//��׼����µĽ���
extern const double STD_LENS_WIDTH; //��׼��ͷ�ĳ��������й�㵽��ͷ����ı�ֵthe width of lens in the scene
extern const double STD_LENS_HEIGHT;//��׼��ͷ�ĳ��������й�㵽��ͷ����ı�ֵ
extern const int STD_IMAGE_WIDTH;//��׼��Ƭ�����س������
extern const int STD_IMAGE_HEIGTH;//��׼��Ƭ�����س������
extern const int STD_SHADE_QUALITY; //*16=��׼����£����ؿ��޼�����Ӱʱ�ļ������caln shade :: how many times will be run (*16)
extern const int STD_DREFL_QUALITY; //��׼����£����ؿ��޼��㾵��������ʱ�ļ������	caln drefl :: how many times will be run (*16)
extern const int STD_MAX_HITPOINTS;//��׼�������ײ��ͼ����
extern const int STD_ITERATIONS;//��׼�����PPM�㷨�ĵ�������
extern const double STD_REDUCTION;//��׼�����PPM�㷨�������뾶������ϵ��
extern const int STD_MAX_PHOTONS;//��׼����¹���ͼ����
extern const int STD_EMIT_PHOTONS;//��׼����·��������
extern const int STD_SAMPLE_PHOTONS;//��׼����²���������
extern const double STD_SAMPLE_DIST;//��׼����²������Ӱ뾶

class Camera {
	std::string algorithm;//ʹ�õ��㷨����ѡֵΪRC��RT��PM��PPM��
	Vector3 O , N , Dx , Dy;//�й��λ��O�������������N����ͷ�������
	//Dx,N��Z��ĵ�λ��ˣ���Dy=Dx��N�Ĳ�� ��3���������ഹֱ 
	double dofSample, aperture, focalLen;//dofSample--SPPM��������--��Ⱦ����Ч���Ĳ�����,
	//aperture��Ȧ��С������0ʱ��Ⱦ����Ч��,focalLen����
	double lens_W , lens_H;//��ͷ�ĳ��������й�㵽��ͷ����ı�ֵ---�������ͷ�ĸߣ��뾵ͷ���й�����֮�ȣ�
	int W , H;//��Ƭ�����س������---  bmpͼ������ظ�
	Color** data;//������Ⱦ������ͼƬ��ÿ�����ص�ɫ�ʣ�
	double shade_quality;//~*16=���ؿ��޼�����Ӱʱ�ļ������--���������Դ���յ�����
	double drefl_quality;//���ؿ��޼��㾵��������ʱ�ļ������-���㾵�������������
	int max_hitpoints;//��ײ��ͼ����
	int iterations;//PPM�㷨�ĵ�������
	double reduction;//PPM�㷨�������뾶������ϵ��
	int max_photons;//����ͼ����--����ͼ�����ɹ�����
	int emit_photons;//���������
	int sample_photons;//����������---����ʱ���������
	double sample_dist;//�������Ӱ뾶--����ʱ���뾶

public:
	Camera();
	~Camera();
	
	std::string GetAlgorithm() { return algorithm; }
	Vector3 GetO() { return O; }
	Vector3 GetN() { return N; }
	int GetDofSample() { return dofSample; }
	double GetAperture() { return aperture; }
	double GetFocalLen() { return focalLen; }
	int GetW() { return W; }
	int GetH() { return H; }
	Color GetColor(int i, int j) { return data[i][j]; }
	void SetColor( int i , int j , Color color ) { data[i][j] = color; }
	double GetShadeQuality() { return shade_quality; }
	double GetDreflQuality() { return drefl_quality; }
	int GetMaxHitpoints() { return max_hitpoints; }
	int GetIterations() { return iterations; }
	double GetReduction() { return reduction; }
	int GetMaxPhotons() { return max_photons; }
	int GetEmitPhotons() { return emit_photons; }
	int GetSamplePhotons() { return sample_photons; }
	double GetSampleDist() { return sample_dist; }

	Vector3 Emit( double i , double j );//�õ�����(i,j)��Ӧ���������
	void DofEmit(double i, double j, Vector3* dof_O, Vector3* dof_V);
	//��Ҫ��Ⱦ����ʱ����(i,j)��Ӧ��һ�������������ߣ��ı�dof_O��dof_V��
	void Initialize();//��ʼ��������������ݶ������֮��
	//����֮ǰִ�У�Scene::CreateScene()����󣩡����ڿ�����ͼ���һЩ��Ҫ����
	void Input( std::string var , std::stringstream& fin );
	void Output( Bmp* );//�ѵõ���ɫ�����ݵ��뵽bmp�ļ���
};

#endif
