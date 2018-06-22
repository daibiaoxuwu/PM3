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
	std::string input , output;//输入文件名（txt文件）和输出文件名（bmp文件）
	Scene* scene;//待渲染的场景
	Camera* camera;//场景中的摄像头
	Photonmap* photonmap;//光子图 
	
	int  H,W;
	int** sample;
	
	
	Color CalnDiffusion( Collider* collider , int* hash, int rc, Color weight);
	//计算漫反射色光，hash用于记录光线经过的物品（hash值不同的相邻象素要使用超级取样去锯齿）
	// ，rc为对应像素点，weight为此射线对像素的权重
	Color CalnReflection( Collider* collider, Ray ray, int dep , bool refracted , int* hash, int rc, Color weight);
//计算反射后得到的色光，其中ray_V是入射光线的方向，dep是迭代层数，
//refracted表示射线是否被折射过奇数次	
	Color CalnRefraction( Collider* collider, Ray ray, int dep , bool refracted , int* hash, int rc, Color weight);
//计算折射后得到的色光
	Color RayTracing(Ray ray, int dep, bool refracted, int* hash, int rc, Color weight);	
//光线追踪的主要迭代函数，衔接不同层次的光线

	void GenerateImage(std::string file);//将camera中的数据导出到file图像文件中

public:
	Raytracer();
	~Raytracer(){
		if(scene!=NULL) {	
			delete  scene;
			scene=NULL;
		}
	}
	void Sampling(int threadID ); //		光线追踪采样，threadID是线程编号  
	void reSampling(int threadID );//光线追踪超级采样（去锯齿）	

	void SetInput( std::string file ) { input = file; }
	void SetOutput( std::string file ) { output = file; }
	void Run();//开始渲染，根据输入txt文件中的数据，渲染出图像导出在输出bmp文件中
};

#endif
