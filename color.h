#ifndef COLOR_H
#define COLOR_H

#include<sstream>
#include<cmath>
#include"vector3.h"
class Color {
public:
	float r , g , b;

	Color( double R = 0 , double G = 0 , double B = 0 ) : r( R ) , g( G ) , b( B ) {}
	~Color() {}

	friend Color operator + ( const Color& , const Color& );
	friend Color operator - ( const Color& , const Color& );
	friend Color operator * ( const Color& , const Color& );
	friend Color operator * ( const Color& , const double& );
	friend Color operator / ( const Color& , const double& );
	friend Color& operator += ( Color& , const Color& );
	friend Color& operator -= ( Color& , const Color& );
	friend Color& operator *= ( Color& , const Color& );
	friend Color& operator *= ( Color& , const double& );
	friend Color& operator /= ( Color& , const double& );


	void Input( std::stringstream& );

	
	inline Color Confine(){ // 3��������������1��luminance must be less than or equal to 1
		//Color Color::Confine() {
		return Color( std::min( r , 1.0f ) , std::min( g , 1.0f ) , std::min( b , 1.0f ) );
	}
	inline Color Exp(){//3���������ֱ���ָ����e��r�η�,e��g�η�,e��b�η���
		//Color Color::Exp() {
		return Color(exp(r), exp(g), exp(b));
	}
	inline double Power(){//3������r,g,bƽ�� 
		//double Color::Power() {
		return ( r + g + b ) / 3;
	}
	inline double RGBMax(){//3������������һ�� 
		//double Color::RGBMax() {
		if (r > g)
			return (r > b) ? r : b;
		return (g > b) ? g : b;
	}
	inline bool IsZeroColor(){//3������ r,g,b�ֱ�������0 
		//bool Color::IsZeroColor() {
		return fabs( r ) < EPS && fabs( g ) < EPS && fabs( b ) < EPS;
	}
};



#endif
