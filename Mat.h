#ifndef MAT_H
#define MAT_H

#include "Vec.h"

class Mat{
protected:
	double** data;
	int row,col;
	void initData(int r,int c);
	void deleteData();
	Mat minor_mat(int r,int c);
	void swapRow(int x,int y);
public:
	Mat();
	Mat(int r,int c);
	Mat(double** d,int r,int c);
	Mat(Vec *v,int r,int c);
	Mat(Vec& v);
	Mat(const Mat& m);
	~Mat();
////////// ////////// ////////// //////////
	Mat operator+(const Mat& m);
	Mat operator-(const Mat& m);
	Mat operator*(const Mat& m);
	Mat operator*(Vec& v);
	//第1參數非Vec,不能是memberFunc,有其它memberFunc用到,so要宣告,不加friend會當作memberFunc
	friend Mat operator*(Vec& v,const Mat& m);
	Mat operator*(const double c);
	//第1參數非Vec,不能是memberFunc,有其它memberFunc用到,so要宣告,不加friend會當作memberFunc
	friend Mat operator*(const double c,const Mat& m);
	Mat operator/(const double c);
	bool operator==(const Mat& m);
	bool operator!=(const Mat& m);
	Mat operator=(const Mat& m);
////////// ////////// ////////// //////////
	void setData(double** d,int r,int c);
	void setData(Vec* v,int r,int c);
	void setData(double d,int r,int c);
	Vec getRowData(int r);
	Vec getColData(int c);
	int getRow();
	int getCol();
	static Mat identity(int s);
	Mat trans();
	double cofactor(int r,int c);
	double det();//type1 LU()
	Mat Adj();
	Mat GaussJordanRowReduction();
	Mat Inverse();//type2 rrf
	void LU(Mat& p,Mat& l,Mat& u);
	void LU(Mat& l,Mat& u,int& swapCount);//type1 row reduction
	bool IsLI();
	Mat SolveSquareLinearSys(const Mat& b);
	void eigen3(Mat& vecs,Vec& vals);
	int Rank();
	Mat LS(Mat& v);
	double PowerMethod(Vec& xn);
	Mat nullspace();
////////// ////////// ////////// //////////
	std::string toString();
};

#endif // MAT_H
