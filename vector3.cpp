#include"vector3.h"

#include<sstream>
#include<cstdlib>
#include<iostream>

    
const double INF = 1e8;
const double EPS = 1e-8;
const double PI = 3.1415926535897932384626;

Vector3 operator + ( const Vector3& A , const Vector3& B ) {
	return Vector3( A.x + B.x , A.y + B.y , A.z + B.z );
}

Vector3 operator - ( const Vector3& A , const Vector3& B ) {
	return Vector3( A.x - B.x , A.y - B.y , A.z - B.z );
}

Vector3 operator * ( const Vector3& A , const double& k ) {
	return Vector3( A.x * k , A.y * k , A.z * k );
}

Vector3 operator / ( const Vector3& A , const double& k ) {
	return Vector3( A.x / k , A.y / k , A.z / k );
}

Vector3 operator * ( const Vector3& A , const Vector3& B ) {
	return Vector3( A.x * B.x , A.y * B.y , A.z * B.z );
}

Vector3& operator += ( Vector3& A , const Vector3& B ) {
	A = A + B;
	return A;
}

Vector3& operator -= ( Vector3& A , const Vector3& B ) {
	A = A - B;
	return A;
}

Vector3& operator *= ( Vector3& A , const double& k ) {
	A = A * k;
	return A;
}

Vector3& operator += ( Vector3& A , const double& k ) {
	A = A / k;
	return A;
}

Vector3& operator *= ( Vector3& A , const Vector3& B ) {
	A = A * B;
	return A;
}

Vector3 operator - ( const Vector3& A ) {
	return Vector3( -A.x , -A.y , -A.z );
}


 void Vector3::Input( std::stringstream& fin ) {
	fin >> x >> y >> z;
}
 void Vector3::Output() {
	std::cout << "x=" << x << ", y=" << y << ",z= " << z << std::endl;
}


void Vector3::AssRandomVector() {
	double dist2;
	Vector3 c1=Vector3(2 * ran() - 1,2 * ran() - 1,2 * ran() - 1).GetUnitVector();
	do 
	{
		x = 2 * ran() - 1;
		y = 2 * ran() - 1;
		z = 2 * ran() - 1;
		dist2=x * x + y * y + z * z;
	} while (dist2> 1 || dist2< EPS );
	dist2=sqrt(dist2);
	x/= dist2;
	y/= dist2;
	z/= dist2;
}

Vector3 Vector3::Refract( Vector3 N , double n , bool* refracted ) {
	Vector3 V = GetUnitVector();
	double cosI = -N.Dot( V ) , cosT2 = 1 -  n * n * ( 1 - cosI * cosI );
	if ( cosT2 <= EPS ) return V.Reflect( N );
	if (refracted != NULL) *refracted ^= true;
	return V * n + N * ( n * cosI - sqrt( cosT2 ) );	
}
Vector3 Vector3::Rotate( Vector3 axis , double theta ) {
	Vector3 ret;
	double cost = cos( theta );
	double sint = sin( theta );
	ret.x += x * ( axis.x * axis.x + ( 1 - axis.x * axis.x ) * cost );
	ret.x += y * ( axis.x * axis.y * ( 1 - cost ) - axis.z * sint );
	ret.x += z * ( axis.x * axis.z * ( 1 - cost ) + axis.y * sint );
	ret.y += x * ( axis.y * axis.x * ( 1 - cost ) + axis.z * sint );
	ret.y += y * ( axis.y * axis.y + ( 1 - axis.y * axis.y ) * cost );
	ret.y += z * ( axis.y * axis.z * ( 1 - cost ) - axis.x * sint );
	ret.z += x * ( axis.z * axis.x * ( 1 - cost ) - axis.y * sint );
	ret.z += y * ( axis.z * axis.y * ( 1 - cost ) + axis.x * sint );
	ret.z += z * ( axis.z * axis.z + ( 1 - axis.z * axis.z ) * cost );
	return ret;
}

