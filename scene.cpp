#include"scene.h"
#include "polyhedron.h"
#include<string>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<ctime>

Scene::Scene() {
	primitive_head = NULL;
	light_head = NULL;
	background_color = Color();
	camera = new Camera;
}

Scene::~Scene() {
	while ( primitive_head != NULL ) {
		Primitive* next_head = primitive_head->GetNext();	
		delete primitive_head;
		primitive_head = next_head;
	}

	while ( light_head != NULL ) {
		Light* next_head = light_head->GetNext();
		delete light_head;
		light_head = next_head;
	}

	delete camera;
}


void Scene::AddPrimitive ( Primitive* pri ) {
	if ( pri != NULL ) {
		pri->SetNext( primitive_head );
		primitive_head = pri;
	}
}

void Scene::AddLight( Light* light ) {
	if ( light != NULL ) {
		light->SetNext( light_head );
		light_head = light;
	}
}

void Scene::CreateScene( std::string file ) {
	if ( file == "" ) return;

	std::ifstream fin( file.c_str() );

	std::string obj;
	while ( fin >> obj ) {
		Primitive* new_primitive = NULL;
		Light* new_light = NULL;

		if ( obj == "primitive" ) {
			std::string type; fin >> type;
			if ( type == "sphere" ) new_primitive = new Sphere;
			if ( type == "plane" ) new_primitive = new Plane;
			//if ( type == "rectangle" ) new_primitive = new Rectangle;
			//if ( type == "triangle" ) new_primitive = new Triangle;
			if ( type == "polyhedron" ) new_primitive = new Polyhedron;
			AddPrimitive( new_primitive );
		} else
		if ( obj == "light" ) {
			std::string type; fin >> type;
			if ( type == "point" ) new_light = new PointLight;
			if ( type == "area" ) new_light = new AreaLight;
			AddLight( new_light );
		} else
		if ( obj != "background" && obj != "camera" ) continue;

		fin.ignore( 1024 , '\n' );
		
		std::string order;
		while ( getline( fin , order , '\n' ) ) {
			std::stringstream fin2( order );
			std::string var; fin2 >> var;
			if ( var == "end" ) {
				if (obj == "primitive" && new_primitive != NULL) new_primitive->PreTreatment();
				break;
			}

			if ( obj == "background"&&var == "color=")  background_color.Input(fin2);			
			if ( obj == "primitive" && new_primitive != NULL ) new_primitive->Input( var , fin2 );
			if ( obj == "light" && new_light != NULL ) new_light->Input( var , fin2 );
			if ( obj == "camera" ) camera->Input( var , fin2 );
		}
	}

	camera->Initialize();
}

Collider* Scene::FindNearestCollide( Ray ray ) {
	Collider* ret = NULL;
	for ( Primitive* now = primitive_head ; now != NULL ; now = now->GetNext() ) {
		Collider collider = now->Collide( ray);
		if ( collider.crash && ( ret == NULL || collider.dist < ret->dist ) ) {
			if (ret == NULL) ret = new Collider;
			*ret = collider;
		}
	}

	return ret;
}

LightCollider* Scene::FindNearestLight( Ray ray ) {
	LightCollider* ret = NULL;
	for (Light* now = light_head; now != NULL; now = now->GetNext()) {
		LightCollider lightCollider = now->Collide( ray);
		if (lightCollider.crash && (ret == NULL || lightCollider.dist < ret->dist)) {
			if (ret == NULL) ret = new LightCollider;
			*ret = lightCollider;
		}
	}
	
	return ret;
}
