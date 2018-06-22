#ifndef CAMERA_H
#define CAMERA_H

#include"vector3.h"
#include"color.h"
#include"bmp.h"
#include<string>
#include<sstream>

extern const int KD_MAX_THREADS; //建立复杂场景的KD树时的最大线程数the maxinum threads of Building KDTree
extern const int PM_MAX_THREADS; //光子映射部分的线程数the maxinum threads of PhotonsMapping
extern const int STD_DOF_SAMPLE;//标准情况下渲染景深效果的采样数

extern const std::string STD_ALGORITHM;//标准情况下使用的渲染算法
extern const double STD_APERTURE;//标准情况下光圈大小，大于0时渲染景深效果
extern const double STD_FOCAL_LEN;//标准情况下的焦距
extern const double STD_LENS_WIDTH; //标准镜头的长（或宽）与感光点到镜头距离的比值the width of lens in the scene
extern const double STD_LENS_HEIGHT;//标准镜头的长（或宽）与感光点到镜头距离的比值
extern const int STD_IMAGE_WIDTH;//标准照片的象素长（或宽）
extern const int STD_IMAGE_HEIGTH;//标准照片的象素长（或宽）
extern const int STD_SHADE_QUALITY; //*16=标准情况下，蒙特卡罗计算阴影时的计算次数caln shade :: how many times will be run (*16)
extern const int STD_DREFL_QUALITY; //标准情况下，蒙特卡罗计算镜面漫反射时的计算次数	caln drefl :: how many times will be run (*16)
extern const int STD_MAX_HITPOINTS;//标准情况下碰撞点图容量
extern const int STD_ITERATIONS;//标准情况下PPM算法的迭代次数
extern const double STD_REDUCTION;//标准情况下PPM算法中收敛半径的收敛系数
extern const int STD_MAX_PHOTONS;//标准情况下光子图容量
extern const int STD_EMIT_PHOTONS;//标准情况下发射光子数
extern const int STD_SAMPLE_PHOTONS;//标准情况下采样光子数
extern const double STD_SAMPLE_DIST;//标准情况下采样光子半径

class Camera {
	std::string algorithm;//使用的算法（可选值为RC、RT、PM和PPM）
	Vector3 O , N , Dx , Dy;//感光点位置O、朝摄像机的向N、镜头长宽半轴
	//Dx,N与Z轴的单位叉乘，；Dy=Dx与N的叉乘 ，3个向量互相垂直 
	double dofSample, aperture, focalLen;//dofSample--SPPM迭代次数--渲染景深效果的采样数,
	//aperture光圈大小，大于0时渲染景深效果,focalLen焦距
	double lens_W , lens_H;//镜头的长（或宽）与感光点到镜头距离的比值---摄像机镜头的高（与镜头到感光点距离之比）
	int W , H;//照片的象素长（或宽）---  bmp图像的象素高
	Color** data;//储存渲染出来的图片（每个象素的色彩）
	double shade_quality;//~*16=蒙特卡罗计算阴影时的计算次数--计算面积光源光照的质量
	double drefl_quality;//蒙特卡罗计算镜面漫反射时的计算次数-计算镜面漫反射的质量
	int max_hitpoints;//碰撞点图容量
	int iterations;//PPM算法的迭代次数
	double reduction;//PPM算法中收敛半径的收敛系数
	int max_photons;//光子图容量--光子图可容纳光子数
	int emit_photons;//发射光子数
	int sample_photons;//采样光子数---采样时所需光子数
	double sample_dist;//采样光子半径--采样时最大半径

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

	Vector3 Emit( double i , double j );//得到象素(i,j)对应的射出光线
	void DofEmit(double i, double j, Vector3* dof_O, Vector3* dof_V);
	//需要渲染景深时象素(i,j)对应的一个随机的射出光线（改变dof_O和dof_V）
	void Initialize();//初始化，在摄像机数据读入完成之后，
	//摄像之前执行（Scene::CreateScene()的最后）。用于开数组和计算一些必要的量
	void Input( std::string var , std::stringstream& fin );
	void Output( Bmp* );//把得到的色光数据导入到bmp文件中
};

#endif
