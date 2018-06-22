#include"raytracer.h"
#include"vector3.h"
#include<cstdlib>
#include<thread>
#include<cstdio>
#include<mutex>
#include<cmath>
//#define ran() (double(rand() * (RAND_MAX + 1) + rand()) / ((RAND_MAX + 1) * (RAND_MAX + 1) - 1))


const int MAX_DREFL_DEP = 2;
const int MAX_RAYTRACING_DEP = 20;
const int HASH_FAC = 7;
const int HASH_MOD = 10000007;
	bool completeThread[10];
	std::mutex* completeRow;
Raytracer::Raytracer() {
	scene = new Scene;
	photonmap = NULL;
}
Color Raytracer::CalnDiffusion( Collider* collider , int* hash, int rc, Color weight) {
	Primitive* pri = collider->GetPrimitive();
	Color color = pri->GetMaterial()->color;
	if ( pri->GetMaterial()->texture != NULL ) color = color * pri->GetTexture(collider->C);
		
	Color ret = color * scene->GetBackgroundColor() * pri->GetMaterial()->diff;
	for ( Light* light = scene->GetLightHead() ; light != NULL ; light = light->GetNext() )
		ret += color * light->GetIrradiance( collider , scene->GetPrimitiveHead() , scene->GetCamera()->GetShadeQuality() , hash );
	ret += color * photonmap->GetIrradiance( collider , camera->GetSampleDist() , camera->GetSamplePhotons() );
	return ret;
}

Color Raytracer::CalnReflection( Collider* collider, Ray ray, int dep , bool refracted , int* hash, int rc, Color weight) {
	Primitive* pri = collider->GetPrimitive(); 
	ray.dir = ray.dir.Reflect( collider->N );
	if ( pri->GetMaterial()->drefl < EPS || dep > MAX_DREFL_DEP ) {
		Color alpha = pri->GetMaterial()->color * pri->GetMaterial()->refl;
		return RayTracing(ray , dep + 1 , refracted , hash, rc, weight * alpha) * alpha;
	}

	int totalSample = camera->GetDreflQuality();
	Color ret, alpha = pri->GetMaterial()->color * pri->GetMaterial()->refl / totalSample;
	
	Vector3 Dx = ray.dir.Cross(Vector3(0,0,1)).GetUnitVector();	
	Vector3 Dy=ray.dir.Cross(Dx).GetUnitVector();
	double dre=pri->GetMaterial()->drefl*pri->GetMaterial()->drefl;
	for ( int k = 0 ; k < totalSample ; k++ ) {
		double x , y;
		do {
			x = ran() * 2 - 1;
			y = ran() * 2 - 1;
		} while ( x * x + y * y > 1 );
		ret += RayTracing(Ray(ray.pos,  ray.dir+Dx*(x*dre)+Dy*(y*dre)   ), dep + MAX_DREFL_DEP , refracted , NULL, rc, weight * alpha);
	}
	return ret * alpha;
}

Color Raytracer::CalnRefraction( Collider* collider, Ray ray, int dep , bool refracted , int* hash, int rc, Color weight) {
	Primitive* pri = collider->GetPrimitive();	
	double zhe = pri->GetMaterial()->rindex;
	if ( !refracted ) zhe = 1 / zhe;//false,表示在空气中，或者是在物体外面 
	bool nextRefracted = refracted;
		ray.dir = ray.dir.Refract( collider->N , zhe , &nextRefracted );
		
	Color alpha = Color(1, 1, 1) * pri->GetMaterial()->refr;
	if (refracted)
		alpha *= (pri->GetMaterial()->absor * -collider->dist).Exp();//3个分量，分别求指数，e的r次方,e的g次方,e的b次方。

	Color ret = RayTracing(ray, dep + 1 , nextRefracted , hash, rc, weight * alpha);
	return ret * alpha;
}

Color Raytracer::RayTracing(Ray ray , int dep , bool refracted , int* hash, int rc, Color weight) {
	if ( dep > MAX_RAYTRACING_DEP ) return Color();
	if ( hash != NULL ) *hash = ( *hash * HASH_FAC ) % HASH_MOD;

	Color ret;
	Collider* collider = scene->FindNearestCollide(ray );
	LightCollider* lightCollider = scene->FindNearestLight(ray );
	
	if (lightCollider != NULL) {
		Light* nearest_light = lightCollider->GetLight();
		if (collider == NULL || lightCollider->dist < collider->dist) {
			if ( hash != NULL ) *hash = ( *hash + nearest_light->GetSample() ) % HASH_MOD;
			ret += nearest_light->GetColor() / nearest_light->GetColor().RGBMax();
		}
		delete lightCollider;
	}
	
	if ( collider != NULL ) {
		Primitive* nearest_primitive = collider->GetPrimitive();
		ray.pos = collider->C;	
		if ( hash != NULL ) *hash = ( *hash + nearest_primitive->GetSample() ) % HASH_MOD;
				
		if ( nearest_primitive->GetMaterial()->diff > EPS ) ret += CalnDiffusion( collider , hash, rc, weight);		
		if ( nearest_primitive->GetMaterial()->refl > EPS ) ret += CalnReflection( collider, ray, dep , refracted , hash, rc, weight);
		if ( nearest_primitive->GetMaterial()->refr > EPS ) ret += CalnRefraction( collider, ray, dep , refracted , hash, rc, weight);
		delete collider;
	}
	if ( dep == 1 ) ret = ret.Confine();
	return ret;
}



void Raytracer::Sampling(int threadID ) {
	for ( int i = 0 ; i < H ; i++ ) {		
		if (!completeRow[i].try_lock()) continue;
		if (i%10 == 0) printf("B%d/%d\n", i, H);//std::cout << "Sampling:   " << i << "/" << H << std::endl;//
		for ( int j = 0 ; j < W ; j++ ) {
			if (camera->GetAperture() < EPS) {	//aperture光圈大小		
				sample[i][j] = 0;
				Color color = camera->GetColor(i, j);
				color += RayTracing(Ray(camera->GetO(),camera->Emit( i , j ) ), 1, false, &sample[i][j], i * W + j, Color(1, 1, 1));
				camera->SetColor(i, j, color.Confine());
			} else {
				int dofSample = camera->GetDofSample();//dofSample= 128	
				Color color = camera->GetColor(i, j);
				for (int k = 0; k < dofSample; k++) {
					Vector3 dof_O, dof_V;
					camera->DofEmit(i, j, &dof_O, &dof_V);//DofEmit里面含有focalLen焦距 
					color += RayTracing(Ray(dof_O, dof_V), 1, false, NULL, i * W + j, Color(1, 1, 1) / dofSample) / dofSample;
				}
				camera->SetColor(i, j, color.Confine());
			}
		}
	}
	completeThread[threadID] = true;
}
void Raytracer::reSampling(int threadID ) {
	for ( int i = 0 ; i < H ; i++ ) {
		if (!completeRow[i].try_lock()) continue;
		if (i%10 == 0) printf("C%d/%d\n", i, H);
		for ( int j = 0 ; j < W ; j++ ) {
			if ( (i == 0 || sample[i][j] == sample[i - 1][j])&&(i == H - 1 || sample[i][j] == sample[i + 1][j])
			&&(j == 0 || sample[i][j] == sample[i][j - 1])&&(j == W - 1 || sample[i][j] == sample[i][j + 1]))continue;
			Color color = camera->GetColor(i, j) / 5;
				for ( int r = -1 ; r <= 1 ; r++ )
					for ( int c = -1 ; c <= 1 ; c++ ) {
						if (((r + c) & 1) == 0) continue;
						color += RayTracing(Ray(camera->GetO(),  camera->Emit( i + ( double ) r / 3 , j + ( double ) c / 3 )
						) , 1, false, NULL, i * W + j, Color(1, 1, 1) / 5) / 5;
					}
			camera->SetColor( i , j , color.Confine() );
		}
	}
	completeThread[threadID] = true;
}

void MultiThreadSampling(int fg,int H1,void*pp ) {
	for (int i = 0; i < PM_MAX_THREADS; i++) completeThread[i] = false;
	completeRow = new std::mutex[H1];
	int i=0;
	for (i = 0; i < PM_MAX_THREADS; i++) {
		if(fg==0) {//std::thread t1(&MyClass::func1,&myclass);   
			std::thread subThread(&Raytracer::Sampling, (Raytracer*)pp, i);
			if (i == PM_MAX_THREADS - 1) subThread.join();
			else subThread.detach();
		}else if(fg==1) {
			std::thread subThread(&Raytracer::reSampling,  (Raytracer*)pp,  i);
			if (i == PM_MAX_THREADS - 1) subThread.join();
			else subThread.detach();
		}else{
			std::thread subThread(&Photontracer::Emitting,  (Photontracer*)pp,  i);
			if (i == PM_MAX_THREADS - 1) subThread.join();
			else subThread.detach();
		}
	} 
	for (bool end = false; !end; ) {
		end = true;
		for (int j = 0; j < PM_MAX_THREADS; j++){		
			if (!completeThread[j]) end = false;			
		}
	}
	delete[] completeRow;
}
void Raytracer::GenerateImage(std::string file) {
	Bmp* bmp = new Bmp;
	camera->Output(bmp);
	bmp->Output(file);
	delete bmp;
}
void Raytracer::Run() {
	scene->CreateScene( input );
	camera = scene->GetCamera();
	
			Photontracer* photontracer = new Photontracer;
			photontracer->SetScene( scene );
			photonmap = photontracer->CalnPhotonmap();
			delete photontracer;
		
		H = camera->GetH(); W = camera->GetW();//rayQ= Ray(camera->GetO());
		sample = new int*[H];
		for ( int i = 0 ; i < H ; i++ )
			sample[i] = new int[W];
			
		MultiThreadSampling( 0 ,H,this);//Sampling(0);//
		if (camera->GetAperture() < EPS) {
			MultiThreadSampling( 1 ,H,this);//reSampling(0);//
		}
		
		for ( int i = 0 ; i < H ; i++ )
			delete[] sample[i];
		delete[] sample;
		
		GenerateImage(output);		
	
		if (photonmap != NULL) {
			delete photonmap;
			photonmap = NULL;
		}
}
