#include "sphere.h"
#include <cmath>

Sphere::Sphere() : Primitive() {
	De = Vector3( 0 , 0 , 1 );
	Dc = Vector3( 0 , 1 , 0 );
}

void Sphere::Input( std::string var , std::stringstream& fin ) {
	if ( var == "O=" ) O.Input( fin );
	if ( var == "R=" ) fin >> R;
	if ( var == "De=" ) De.Input( fin );
	if ( var == "Dc=" ) Dc.Input( fin );
	Primitive::Input( var , fin );
}

Collider Sphere::Collide( Ray ray ) {
	Collider collider;
	ray.dir = ray.dir.GetUnitVector();//?????????
	Vector3 eo = O - ray.pos;//��Դָ�����ĵ�����
	double eA = eo .Dot( ray.dir );//��Դ��ͶӰ��ľ��� eA= |eo|cosab��
	if (eA <= EPS) return collider;//1�������Դ�������ⲿ���� eA < 0����ô�� �������治�ཻ  
	
	double AB2 = R * R- eo.Module2() + eA * eA;//AB2ͶӰ��  �� ����������Ľ���ľ���ƽ�� 	
	if (AB2 <= EPS) return collider;//2��AB<=0,�����ĵ����ߵľ�����ڰ뾶����ô���������治�ཻ 
	
		AB2 = sqrt( AB2 );
		double eB1 = eA - AB2  , eB2 = eA + AB2; 
		if ( eB2 < EPS ) return collider;
		 
		if ( eB1 > EPS ) {
			collider.dist = eB1;
			collider.front = true;
		} else {
			collider.dist = eB2;
			collider.front = false;
		} 
	collider.crash = true;
	collider.I = ray.dir;
	collider.SetPrimitive(this);
	collider.C = ray.pos + ray.dir * collider.dist;
	collider.N = ( collider.C - O ).GetUnitVector();
	if (!collider.front) collider.N = -collider.N;
	return collider;
}

Color Sphere::GetTexture(Vector3 C) {
	Vector3 I = ( C - O ).GetUnitVector();
	double a = acos( -I.Dot( De ) );
	double b = acos( std::min( std::max( I.Dot( Dc ) / sin( a ) , -1.0 ) , 1.0 ) );
	double u = a / PI , v = b / 2 / PI;
	if ( I.Dot(Dc.Cross(De)) < 0 ) v = 1 - v;
	return material->texture->GetSmoothColor( u , v );
}
