#include"primitive.h"
#include<sstream>
#include<cstdio>
#include<string>
#include<cmath>
#include<fstream>
#include<iostream>
#include<cstdlib>

extern const double EPS;
/* 
Primitive::Primitive( const Primitive& primitive ) {
	*this = primitive;
	material = new Material;11111111 
	*material = *primitive.material;1111111111111 
}*/ 
Primitive::Primitive() {
	sample = ran1();
	material = new Material;
	next = NULL;
}

Primitive::Primitive( const Primitive& primitive ) {
	*this = primitive;
	material = new Material;
	*material = *primitive.material;
}

Primitive::~Primitive() {
	delete material;
}

void Primitive::Input( std::string var , std::stringstream& fin ) {
	material->Input( var , fin );
}

Color Primitive::GetTexture(Vector3 C) {
	return Color(0, 0, 0);
}
