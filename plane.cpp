#include "plane.h"
#include <cmath>

Plane::Plane() : Primitive() {
	
}

void Plane::Input( std::string var , std::stringstream& fin ) {
	if ( var == "N=" ) N.Input( fin );
		N = N.GetUnitVector();	
	if ( var == "R=" ) fin >> R;
	if ( var == "Dx=" ) Dx.Input( fin );
	if ( var == "Dy=" ) Dy.Input( fin );
	Primitive::Input( var , fin );
}

Collider Plane::Collide( Ray ray ) {
	Collider collider;
	double nomDi = N.Dot( ray.dir );
	if ( fabs( nomDi ) <= EPS ) return collider;
	
	double tt=( - R  -N.Dot( ray.pos ) )/nomDi;//double tt = ( N * R - ray.pos ).Dot( N ) / nomDi;
	if (tt <= EPS||tt >= INF) return collider;
	
	
	collider.crash = true;
	collider.I = ray.dir;
	collider.SetPrimitive(this);
	collider.dist = tt;
	collider.front = true;//( d < 0 );
	collider.C = ray.pos + ray.dir * collider.dist;
	collider.N =N;// ( collider.front ) ? N : -N;
	return collider;
}

Color Plane::GetTexture(Vector3 C) {
	double u = C.Dot( Dx ) / Dx.Module2() + 0.5;
	double v = C.Dot( Dy ) / Dy.Module2() + 0.5;
	return material->texture->GetSmoothColor( u , v );
}
