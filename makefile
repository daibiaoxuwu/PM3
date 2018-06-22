objects = vector3.o color.o bmp.o material.o \
	scene.o \
	camera.o \
	primitive.o plane.o sphere.o triangle.o polyhedron.o \
	light.o pointLight.o areaLight.o \
	objReader.o triangleTree.o photonmap.o\
	raytracer.o photontracer.o

all: main.exe clear

main.exe: main.cpp $(objects)
	g++ main.cpp $(objects) `pkg-config --cflags --libs opencv` -o main.exe -O3

vector3.o: vector3.h vector3.cpp
	g++ -c vector3.cpp -O3

color.o: color.h color.cpp
	g++ -c color.cpp -O3

bmp.o: bmp.h bmp.cpp
	g++ -c bmp.cpp -O3

material.o: material.h material.cpp
	g++ -c material.cpp -O3

scene.o: scene.h scene.cpp
	g++ -c scene.cpp -O3

camera.o: camera.h camera.cpp
	g++ -c camera.cpp -O3

primitive.o: primitive.h primitive.cpp
	g++ -c primitive.cpp -O3

plane.o: plane.h plane.cpp
	g++ -c plane.cpp -O3
	
sphere.o: sphere.h sphere.cpp
	g++ -c sphere.cpp -O3
	
triangle.o: triangle.h triangle.cpp
	g++ -c triangle.cpp -O3

polyhedron.o: polyhedron.h polyhedron.cpp
	g++ -c polyhedron.cpp -O3

light.o: light.h light.cpp
	g++ -c light.cpp -O3

pointLight.o: pointLight.h pointLight.cpp
	g++ -c pointLight.cpp -O3

areaLight.o: areaLight.h areaLight.cpp
	g++ -c areaLight.cpp -O3

objReader.o: objReader.h objReader.cpp
	g++ -c objReader.cpp -O3

triangleTree.o: triangleTree.h triangleTree.cpp
	g++ -c triangleTree.cpp -O3 -std=c++11

photonmap.o: photonmap.h photonmap.cpp
	g++ -c photonmap.cpp -O3

raytracer.o: raytracer.h raytracer.cpp
	g++ -c raytracer.cpp -O3 -std=c++11

photontracer.o: photontracer.h photontracer.cpp
	g++ -c photontracer.cpp -O3 -std=c++11

clear:
	del *.bak *.o
