#include"raytracer.h"

#include<string>
#include<ctime>
#include<cmath>
double start_time ;
int main() {
	
	start_time = clock();
	
	Raytracer* raytracer = new Raytracer;
	raytracer->SetInput("scene.txt");
	raytracer->SetOutput("picture.bmp");
	raytracer->Run(); 
	delete raytracer;
	
	double end_time = clock();
	printf("Escaped time: %.5lf\n" , (end_time - start_time)/CLOCKS_PER_SEC);
	return 0;
}
