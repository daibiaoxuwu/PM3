#include"color.h"
#include<sstream>
#include<cmath>

extern const double EPS;

Color operator + ( const Color& A , const Color& B ) {
	return Color( A.r + B.r , A.g + B.g , A.b + B.b );
}

Color operator - ( const Color& A , const Color& B ) {
	return Color( A.r - B.r , A.g - B.g , A.b - B.b );
}

Color operator * ( const Color& A , const Color& B ) {
	return Color( A.r * B.r , A.g * B.g , A.b * B.b );
}

Color operator * ( const Color& A , const double& k ) {
	return Color( A.r * k , A.g * k , A.b * k );
}

Color operator / ( const Color& A , const double& k ) {
	return Color( A.r / k , A.g / k , A.b / k );
}

Color& operator += ( Color& A , const Color& B ) {
	A = A + B;
	return A;
}

Color& operator -= ( Color& A , const Color& B ) {
	A = A - B;
	return A;
}

Color& operator *= ( Color& A , const Color& B ) {
	A = A * B;
	return A;
}

Color& operator *= ( Color& A , const double& k ) {
	A = A * k;
	return A;
}

Color& operator /= ( Color& A , const double& k ) {
	A = A / k;
	return A;
}


void Color::Input( std::stringstream& fin ) {//0.05 0.05 0.05	
	fin >> r >> g >> b;
}

