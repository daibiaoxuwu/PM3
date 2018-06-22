#include"photontracer.h"
#include"vector3.h"
#include"scene.h"
#include<cstdlib>
#include<cstdio>
#include<thread>
#include<mutex>

const int MAX_PHOTONTRACING_DEP = 20;
Photontracer::Photontracer() {
	scene = NULL;
	photonmap = NULL;
}

bool Photontracer::PhotonDiffusion( Collider* collider , Photon photon , int dep , bool refracted , double* prob ) {
	Primitive* pri = collider->GetPrimitive();
	Material* material = pri->GetMaterial();
	Color color = material->color;
	if ( material->texture != NULL ) color = color * pri->GetTexture(collider->C);
	double eta = material->diff * color.Power();
	if ( eta <= ran() * ( *prob ) ) {
		*prob -= eta;
		return false;
	}

	photon.dir = collider->N.Diffuse();
	photon.power = photon.power * color / color.Power();
	PhotonTracing( photon , dep + 1 , refracted );
	return true;
}

bool Photontracer::PhotonReflection( Collider* collider , Photon photon , int dep , bool refracted , double* prob ) {
	Primitive* pri = collider->GetPrimitive();
	Material* material = pri->GetMaterial();
	Color color = material->color;
	if ( material->texture != NULL ) color = color * pri->GetTexture(collider->C);
	double eta = material->refl * color.Power();

	if ( eta <= ran() * ( *prob ) ) {
		*prob -= material->refl;
		return false;
	}
	photon.dir = photon.dir.Reflect( collider->N );
	if ( material->drefl > EPS ) {		
		Vector3 Dx = photon.dir.Cross(Vector3(0,0,1)).GetUnitVector();
		Vector3 Dy = photon.dir.Cross(Dx).GetUnitVector();
		double dre=material->drefl*material->drefl;
		double x , y;
		do {
			x = ran() * 2 - 1;
			y = ran() * 2 - 1;
		} while ( x * x + y * y > 1 );	
		photon.dir+=Dx*(x*dre)+Dy*(y*dre);
	}
	photon.power = photon.power * color / color.Power();
	PhotonTracing( photon , dep + 1 , refracted );
	return true;
}

bool Photontracer::PhotonRefraction( Collider* collider , Photon photon , int dep , bool refracted , double* prob ) {
	Primitive* pri = collider->GetPrimitive();
	Material* material = pri->GetMaterial();
	double eta = material->refr;
	if ( refracted ) {
		Color trans = (material->absor * -collider->dist).Exp();
		eta *= trans.Power();
		photon.power = photon.power * trans / trans.Power();
	}

	if ( eta <= ran() * ( *prob ) ) {
		*prob -= material->refr;
		return false;
	}
	
	double zhe = material->rindex;
	if ( !refracted ) zhe = 1 / zhe;
	bool nextRefracted = refracted;
	photon.dir = photon.dir.Refract( collider->N , zhe , &nextRefracted );
	PhotonTracing( photon , dep + 1 , nextRefracted );
	return true;
}
void Photontracer::PhotonTracing( Photon photon , int dep , bool refracted ) {
	if ( dep > MAX_PHOTONTRACING_DEP ) return;
	Collider* collider = scene->FindNearestCollide( Ray( photon.pos , photon.dir  ) );	
	if ( collider != NULL ) {
		Primitive* nearest_primitive = collider->GetPrimitive();		
		photon.pos = collider->C;
		if ( nearest_primitive->GetMaterial()->diff > EPS && dep > 1 ) {
			if (photonmap != NULL)
				photonmap->Store(photon);
		
		}
		
		double prob = 1;
		if ( PhotonDiffusion( collider , photon , dep , refracted , &prob ) == false )
		if ( PhotonReflection( collider , photon , dep , refracted , &prob ) == false )
		if ( PhotonRefraction( collider	 , photon , dep , refracted , &prob ) == false );
		delete collider;
	}
}


	extern bool completeThread[ ];
	extern std::mutex* completeRow;
	extern void MultiThreadSampling(int fg,int H1,void*pp );
void Photontracer::Emitting(int threadID ) {
	for (int i = 0; i < hh ; i++) {
		if (!completeRow[i].try_lock()) continue;
		if (i%50 == 0) printf("A%d/%d\n", i, hh);	
		for ( int j = 0 ; j < ww ; j++ ) {
			Photon photon = light->EmitPhoton();
			photon.power *= totalPower;
			PhotonTracing( photon , 1 , false );		
		}
	}
	completeThread[threadID] = true;
}
Photonmap* Photontracer::CalnPhotonmap() {
	photonmap = new Photonmap(scene->GetCamera()->GetMaxPhotons());
	photonmap->SetEmitPhotons(scene->GetCamera()->GetEmitPhotons());
	totalPower = 0;
	for (  light = scene->GetLightHead() ; light != NULL ; light = light->GetNext() )
		totalPower += light->GetColor().Power();
	double photonPower = totalPower / scene->GetCamera()->GetEmitPhotons();

	for (  light = scene->GetLightHead() ; light != NULL ; light = light->GetNext() ) {
		int lightPhotons = (int)(light->GetColor().Power() / photonPower);//lightPhotons = lightPhotons / PM_MAX_THREADS + ((lightPhotons % PM_MAX_THREADS > threadID) ? 1 : 0);
		hh=sqrt(lightPhotons) ;
	
		ww=lightPhotons/hh;
		MultiThreadSampling( 2 ,hh,this);	
	}	
	photonmap->Balance();
	return photonmap;
}

