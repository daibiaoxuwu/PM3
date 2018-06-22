#include "areaLight.h"
#include"color.h"
#include"vector3.h"
#include <cmath>
#include <cstdlib>
//#define ran() (double(rand() * (RAND_MAX + 1) + rand()) / ((RAND_MAX + 1) * (RAND_MAX + 1) - 1))

void AreaLight::Input( std::string var , std::stringstream& fin ) {
	if ( var == "O=" ) O.Input( fin );
	if ( var == "Dx=" ) Dx.Input( fin );
	if ( var == "Dy=" ) Dy.Input( fin );
	Light::Input( var , fin );
}

LightCollider AreaLight::Collide(   Ray ray ) {
	LightCollider ret;
	ret.SetLight(this);
	
	ray.dir = ray.dir.GetUnitVector();
	Vector3 N = (Dx.Cross(Dy)).GetUnitVector();
	double d = N.Dot( ray.dir );
	if ( fabs( d ) < EPS ) return ret;
	double l = ( N * O.Dot( N ) - ray.pos ).Dot( N ) / d;
	if ( l < EPS ) return ret;

	Vector3 C = ( ray.pos + ray.dir * l ) - O;
	if ( fabs( Dx.Dot( C ) ) > Dx.Dot( Dx ) ) return ret;
	if ( fabs( Dy.Dot( C ) ) > Dy.Dot( Dy ) ) return ret;

	ret.crash = true;
	ret.dist = l;
	return ret;
}

Photon AreaLight::EmitPhoton() {
	Photon ret;
	ret.power = color / color.Power();
	ret.pos = O + Dx * ( ran() * 2 - 1 ) + Dy * ( ran() * 2 - 1 );
	ret.dir.AssRandomVector();
	return ret;
}

Color AreaLight::GetIrradiance( Collider* collider , Primitive* primitive_head , int shade_quality , int* hash ) {
	Primitive* pri = collider->GetPrimitive();
	Color ret;
	Ray ray( collider->C);
	for ( int i = -2 ; i < 2 ; i++ )
		for ( int j = -2 ; j < 2 ; j++ )
			for ( int k = 0 ; k < shade_quality ; k++ ) {//shade_quality= 4,计算面积光源光照的质量
				ray.dir = (O + Dx * ( ( ran() + i ) / 2 ) + Dy * ( ( ran() + j ) / 2 )) - collider->C;
				double dist = ray.dir.Module();
				bool shade = false;
				for ( Primitive* now = primitive_head ; now != NULL ; now = now->GetNext() ) {
					Collider thisCollider = now->Collide( ray);
					if ( thisCollider.crash && thisCollider.dist < dist ) {
						shade = true;
						break;
					}
				}				
				if ( shade == false ) ret += CalnIrradiance( collider , ray.dir , NULL );
			}
	
	ret /= 16.0 * shade_quality;
	return ret;
}
