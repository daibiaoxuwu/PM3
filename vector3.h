#ifndef VECTOR3_H
#define VECTOR3_H

#include<sstream>
#include<cmath>

extern const double INF;
extern const double EPS;
extern const double PI;
//#define ran() (double(rand() * (RAND_MAX + 1) + rand()) / ((RAND_MAX + 1) * (RAND_MAX + 1) - 1))
	static unsigned mSeed=0;
    inline  static double ran() {//RandomLCG,产生0-1之间的随机数 
        mSeed = 214013 * mSeed + 2531011;
		return ((double)mSeed  )/ 4294967296 ;//return  mSeed * (( double)1 / 4294967296);
	}
	inline  static int ran1() {//RandomLCG，产生0-40亿之间的随机数 
        mSeed = 214013 * mSeed + 2531011;
		return mSeed;
	}  
    
class Vector3 {
public:
	double x , y , z;
	
	Vector3( double X = 0 , double Y = 0 , double Z = 0 ) : x( X ) , y( Y ) , z( Z ) {}
	~Vector3() {}

	friend Vector3 operator + ( const Vector3& , const Vector3& );
	friend Vector3 operator - ( const Vector3& , const Vector3& );
	friend Vector3 operator * ( const Vector3& , const double& );
	friend Vector3 operator / ( const Vector3& , const double& );
	friend Vector3 operator * ( const Vector3& , const Vector3& );
	friend Vector3& operator += ( Vector3& , const Vector3& );
	friend Vector3& operator -= ( Vector3& , const Vector3& );
	friend Vector3& operator *= ( Vector3& , const double& );
	friend Vector3& operator /= ( Vector3& , const double& );
	friend Vector3& operator *= ( Vector3& , const Vector3& );
	friend Vector3 operator - ( const Vector3& );
	
	void Input( std::stringstream& fin );
	void Output();
	 
	 
	inline double Dot( const Vector3&term ){
		return x * term.x + y * term.y + z * term.z;
	}
	inline Vector3 Cross(const Vector3&term) {
		return Vector3(y * term.z - z * term.y , z * term.x - x * term.z , x * term.y - y * term.x );
	}
	inline double Module2() {
		return x * x + y * y + z * z;
	}
	inline double Module(){
		return sqrt( x * x + y * y + z * z );
	}
	inline double Distance2( Vector3&term){
		return ( term - *this ).Module2();
	}
	inline double Distance( Vector3&term ) {
		return ( term - *this ).Module();
	}
	inline double& GetCoord( int axis ){
		if ( axis == 0 ) return x;
		if ( axis == 1 ) return y;
		if ( axis == 2 ) return z;
	}//得到某一个坐标（3坐标中的1个）的值， 
	inline Vector3 GetUnitVector(){
		double module = Module();
		if (module < EPS) return Vector3(0, 0, 1);
		if ( fabs(module -1)< EPS) return *this;
		return *this / module;
	}
	
		
	inline Vector3 GetAnVerticalVector() {//返回 与Z轴叉积 的单位向量 
		Vector3 ret = Cross(Vector3( 0 , 0 , 1 ));
		if ( ret.IsZeroVector() ) return  Vector3( 1 , 0 , 0 );	
		return ret.GetUnitVector();
	}
	inline bool IsZeroVector(){
		return fabs( x ) < EPS && fabs( y ) < EPS && fabs( z ) < EPS;
	}
		
	inline Vector3 Reflect( Vector3 N ) {
		return *this - N * ( 2 * Dot( N ) );
	}//计算该光线遇到面N的反射光线
	inline Vector3 Diffuse() {
		return Rotate(  Cross(Vector3( 0 , 0 , 1 )).GetUnitVector() , acos( sqrt( ran() ) ) ).Rotate( *this , ran() * 2 * PI );
	}
	
	
	Vector3 Refract( Vector3 N , double n , bool* refracted = NULL );//计算该光线遇到面N的折射光线（折射率为n）
	Vector3 Rotate( Vector3 axis , double theta );
	void AssRandomVector();
};
  




#endif
