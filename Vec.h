#ifndef VEC_H
#define VEC_H

#ifndef M_PI//有時候我需要切換許多的編譯器來DEBUG
#define M_PI 3.1415926535897932384626
#endif
#define EQU(a,b) (fabs(a-b)<0.00000001)

#include <cmath>
#include <sstream>

class Vec{
protected:
	double* data;
	int dim;
public:
	Vec(void);
	Vec(int s);
	Vec(double* d,int s);
	Vec(const Vec& v);//----Asign
	~Vec();
////////// ////////// ////////// //////////
	Vec operator+(const Vec& v);
	Vec operator-(const Vec& v);
	//第1參數非Vec,不能是memberFunc,有其它memberFunc用到,so要宣告,不加friend會當作memberFunc
	friend Vec operator*(const double c,const Vec& v1);
	Vec operator*(const double c);
	double operator*(const Vec& c);
	Vec operator/(const double c);
	bool operator==(Vec& v);
	bool operator!=(Vec& v);
	Vec operator=(const Vec& v);
////////// ////////// ////////// //////////
	void setO();
	void setI();
	void setData(double* d,int s);
	void setData(double d,int i);
	double getData(int i);
	int getDim();
	bool isEqual(const Vec& v);
	double distance(const Vec& v);
	double norm()const;
	Vec normal();
	void normalize();
	double dot(const Vec& v);
	double angle_cos(const Vec &v);
	double angle_radian(const Vec &v);
	double angle_degree(const Vec &v);
	double comp(Vec& v);
	Vec proj(Vec& v);
	double Area(Vec& v);
	Vec cross3(const Vec& v);
	double cross3_norm(const Vec& v);
	bool isParal(Vec &v);
	bool isOrtho(const Vec& v);
	Vec planeNormal(Vec& v);
	static void ob(Vec* v);
////////// ////////// ////////// //////////
	std::string toString();
};

#endif // VEC_H
