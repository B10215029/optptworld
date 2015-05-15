#include "Mat.h"

void Mat::initData(int r,int c){
	row=r;
	col=c;
	data=new double*[r];
	for(int i=0;i<r;i++)
		data[i]=new double[c];
	for(int i=0;i<r;i++)
		for(int j=0;j<c;j++)
			data[i][j]=0;
}
void Mat::deleteData(){
	if(data){
		for(int i=0;i<row;i++)
			delete[] data[i];
		delete[] data;
	}
	row=0;
	col=0;
}
Mat::Mat(){
	initData(1,1);
}
Mat::Mat(int r,int c){
	initData(r,c);
}
Mat::Mat(double** d,int r,int c){
	initData(r,c);
	for(int i=0;i<r;i++)
		for(int j=0;j<c;j++)
			data[i][j]=d[i][j];
}
Mat::Mat(Vec* v,int r,int c){
	initData(r,c);
	for(int i=0;i<r;i++)
		for(int j=0;j<c;j++)
			data[i][j]=v[i].getData(j);
}
Mat::Mat(Vec& v){
	initData(1,v.getDim());
	for(int i=0;i<v.getDim();i++)
		data[0][i]=v.getData(i);
}
Mat::Mat(const Mat& m){
	initData(m.row,m.col);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			data[i][j]=m.data[i][j];
}
Mat::~Mat(){
	deleteData();
}
////////// ////////// ////////// //////////
Mat Mat::operator+(const Mat& m){//operator override +
	if(row!=m.row||col!=m.col) throw "+失敗，維度不同!";
	Mat re(row,col);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			re.data[i][j]=data[i][j]+m.data[i][j];
	return re;
}
Mat Mat::operator-(const Mat& m){//operator override -
	if(row!=m.row||col!=m.col) throw "-失敗，維度不同!";
	Mat re(row,col);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			re.data[i][j]=data[i][j]-m.data[i][j];
	return re;
}
Mat Mat::operator*(const Mat& m){//operator override *
	if(col!=m.row) throw "*失敗，維度不同!";
	Mat re(row,m.col);
	for(int i=0;i<row;i++)
		for(int j=0;j<m.col;j++)
			for(int k=0;k<col;k++)
				re.data[i][j]+=data[i][k]*m.data[k][j];
	return re;
}
Mat Mat::operator*(Vec& v){
	Mat re=*this*Mat(v).trans();
	return re;
}
Mat operator*(Vec& v,const Mat& m){
	Mat re=Mat(v)*m;
	return re;
}
Mat Mat::operator*(const double c){//operator override *
	Mat re(row,col);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			re.data[i][j]=data[i][j]*c;
	return re;
}
Mat operator*(const double c,const Mat& m){//operator override *
	Mat re(m.row,m.col);
	for(int i=0;i<m.row;i++)
		for(int j=0;j<m.col;j++)
			re.data[i][j]=m.data[i][j]*c;
	return re;
}
Mat Mat::operator/(const double c){//operator override /
	Mat re(row,col);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			re.data[i][j]=data[i][j]/c;
	return re;
}
bool Mat::operator==(const Mat& m){
	if(row!=m.row || col!=m.col) return false;
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			if(data[i][j]!=m.data[i][j]) return false;
	return true;
}
bool Mat::operator!=(const Mat& m){
	return !(*this==m);
}

Mat Mat::operator=(const Mat& m){//operator override =
	deleteData();
	initData(m.row,m.col);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			data[i][j]=m.data[i][j];
	return *this;
}
////////// ////////// ////////// //////////
void Mat::setData(double** d,int r,int c){//屬重新設定，不判斷範圍
	deleteData();
	initData(r,c);
	for(int i=0;i<r;i++)
		for(int j=0;j<c;j++)
			data[i][j]=d[i][j];
}
void Mat::setData(Vec* v,int r,int c){//屬重新設定，不判斷範圍
	deleteData();
	initData(r,c);
	for(int i=0;i<r;i++)
		for(int j=0;j<c;j++)
			data[i][j]=v[i].getData(j);
}
void Mat::setData(double d,int r,int c){
	if((r>=row||r<0)||(c>=col||c<0)) throw "setData失敗，超出範圍!";
	data[r][c]=d;
}
Vec Mat::getRowData(int r){
	if(r>=row||r<0) throw "getRowData失敗，超出範圍!";
	Vec v(data[r],col);
	return v;
}
Vec Mat::getColData(int c){
	if(c>=col||c<0) throw "getColData失敗，超出範圍!";
	Vec v(row);
	for(int i=0;i<row;i++)
		v.setData(data[i][c],i);
	return v;
}
int Mat::getRow(){
	return row;
}
int Mat::getCol(){
	return col;
}
Mat Mat::identity(int s){
	Mat m(s,s);
	for(int i=0;i<m.row;i++)
		m.data[i][i]=1;
	return m;
}
Mat Mat::trans(){
	Mat m(col,row);
	for(int i=0;i<m.row;i++)
		for(int j=0;j<m.col;j++)
			m.data[i][j]=data[j][i];
	return m;
}
Mat Mat::minor_mat(int r,int c){
	Mat m(row-1,col-1);
	for(int i=0,mi=0;i<row;i++){
		if(i==r) continue;
		for(int j=0,mj=0;j<col;j++){
			if(j==c) continue;
			m.setData(data[i][j],mi,mj++);
		}
		mi++;
	}
	return m;
}
void Mat::swapRow(int x,int y){
	for(int i=0;i<col;i++){
		double t;
		t=data[x][i];
		data[x][i]=data[y][i];
		data[y][i]=t;
	}
}
double Mat::cofactor(int r, int c){
	return pow(-1.,r+c)*minor_mat(r,c).det();
}
double Mat::det(){
	if(row!=col)
		throw "det失敗，not square matrix!";
	else if(row==3)
		return
				data[0][0]*data[1][1]*data[2][2]+
				data[0][1]*data[1][2]*data[2][0]+
				data[0][2]*data[1][0]*data[2][1]-

				data[0][2]*data[1][1]*data[2][0]-
				data[0][0]*data[1][2]*data[2][1]-
				data[0][1]*data[1][0]*data[2][2];
	else if(row==2)
		return data[0][0]*data[1][1]-data[0][1]*data[1][0];
	else if(row==1)
		return data[0][0];

	double x=1;
	//type1 LU()
	Mat l,u;
	int swapCount;
	LU(l,u,swapCount);
	for(int i=0;i<row;i++)
		x*=u.data[i][i];
	x*=pow(-1.,swapCount);

	//type2 cofactor
//	for(int i=0;i<col;i++)
//		x+=data[0][i]*pow(-1,0+i)*minor_mat(0,i).det();
	return x;
}
Mat Mat::Adj(){
	Mat m(row,col);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			m.setData(cofactor(i,j),j,i);
	return m;
}
Mat Mat::GaussJordanRowReduction(){
	Mat p=Mat::identity(row);
	for(int i=0;i<row;i++) //find nearest row not 0 element
		if(data[i][i]==0)
			for(int j=i+1;j<row;j++)
				if(data[j][i]!=0){
					swapRow(i,j);
					p.swapRow(i,j);
					break;
				}
	for(int i=0;i<row;i++) //從上往下消其他row
		for(int j=i+1;j<row;j++){
			if(data[i][i]==0) break;
			double muti=data[j][i]/data[i][i];
			data[j][i]=0;
			for(int k=i+1;k<col;k++)
				data[j][k]-=muti*data[i][k];
			for(int k=0;k<col;k++)
				p.data[j][k]-=muti*p.data[i][k];
		}
	for(int i=0;i<row;i++) //變0
		for(int j=0;j<col;j++)
			if(EQU(data[i][j],0)) data[i][j]=0;
	for(int i=row-1;i>=0;i--) //從下往上消其他row
		for(int j=i-1;j>=0;j--){
			if(data[i][i]==0) break;
			double muti=data[j][i]/data[i][i];
			data[j][i]=0;
			for(int k=i-1;k>=0;k--)
				data[j][k]-=muti*data[i][k];
			for(int k=row-1;k>=0;k--)
				p.data[j][k]-=muti*p.data[i][k];
		}
	for(int i=0;i<row;i++) //變0
		for(int j=0;j<col;j++)
			if(EQU(data[i][j],0)) data[i][j]=0;
	for(int i=0;i<row;i++) //變1
		if(data[i][i]!=0){
			double div=data[i][i];
			for(int j=0;j<col;j++){
				data[i][j]/=div;
				p.data[i][j]/=div;
			}
		}
	for(int i=0;i<row;i++) //變0
		for(int j=0;j<col;j++)
			if(EQU(data[i][j],0)) data[i][j]=0;
	return p;
}
Mat Mat::Inverse(){
	//type1 Adj()/det() 可能造成太多cost
//	double x=det();
//	if(x==0) throw "沒有反矩陣";
//	return Adj()/x;

	//type2 rrf
	if(row!=col) throw "Inverse失敗，not square matrix!";
	Mat a(*this),p,iden=Mat::identity(row);
	p=a.GaussJordanRowReduction();
	if(a!=iden) throw "沒有Inverse!";
	return p;
}
void Mat::LU(Mat& l,Mat& u,int& swapCount){
	if(row!=col) throw "LU失敗，not square matrix!";
	//type1 row reduction
	Mat p=Mat::identity(row);
	u=*this;
	swapCount=0;
	for(int i=0;i<row;i++) //find nearest row not 0 element
		if(u.data[i][i]==0)
			for(int j=i+1;j<row;j++)
				if(u.data[j][i]!=0){
					u.swapRow(i,j);
					p.swapRow(i,j);
					swapCount++;
					break;
				}
	for(int i=0;i<row;i++) //從上往下消其他row
		for(int j=i+1;j<row;j++){
			if(u.data[i][i]==0) break;
			double muti=u.data[j][i]/u.data[i][i];
			u.data[j][i]=0;
			for(int k=i+1;k<col;k++)
				u.data[j][k]-=muti*u.data[i][k];
			for(int k=0;k<col;k++)
				p.data[j][k]-=muti*p.data[i][k];
		}
	l=p.Inverse();
}
bool Mat::IsLI(){
	if(row>col)
		return false;
	if(Rank()==row)
		return true;
	return false;
}
Mat Mat::SolveSquareLinearSys(const Mat& b){
	if(row!=col || row!=b.row || b.col!=1) throw "SolveSquareLinearSys失敗，維度不同!";
	Mat l,u,x(row,1),y(row,1);
	y.data[0][0]=x.data[0][0]=1;
	int s;
	LU(l,u,s); //ax=b=lux=ly
	for(int i=0;i<row;i++){ //ly=b
		double sum=0;
		for(int j=i-1;j>=0;j--)
			sum+=l.data[i][j]*y.data[j][0];
		y.data[i][0]=(b.data[i][0]-sum)/l.data[i][i];
	}
	for(int i=row-1;i>=0;i--){ //ux=y
		double sum=0;
		for(int j=i+1;j<col;j++)
			sum+=u.data[i][j]*x.data[j][0];
		x.data[i][0]=(y.data[i][0]-sum)/u.data[i][i];
	}
	return x;
}
void Mat::eigen3(Mat& vecs,Vec& vals){
	if(row!=col || row>3) throw "eigen3失敗，維度不同!";
	if(row==3){
		Mat rm(col,col);
		Vec eigenVal(col);
		double ta=-data[0][0]-data[1][1]-data[2][2],
				tb=data[0][0]*data[1][1]+data[1][1]*data[2][2]+data[2][2]*data[0][0]
				-data[0][1]*data[1][0]
				-data[0][2]*data[2][0]
				-data[1][2]*data[2][1],
				tc=-det(),
				tQ=(ta*ta-3*tb)/9,
				tR=(2*ta*ta*ta-9*ta*tb+27*tc)/54,
				tT;
		if(tR*tR<tQ*tQ*tQ)
			tT=acos(tR/sqrt(tQ*tQ*tQ));
		else if(EQU(tR,0)&&EQU(tQ,0))
			tT=acos(1.);
		else
			return;
		eigenVal.setData(-2*sqrt(tQ)*cos(tT/3)-ta/3,0);
		eigenVal.setData(-2*sqrt(tQ)*cos((tT+2*M_PI)/3)-ta/3,1);
		eigenVal.setData(-2*sqrt(tQ)*cos((tT-2*M_PI)/3)-ta/3,2);
		for(int i=0;i<col;i++){
			Mat nullMat=(identity(col)*eigenVal.getData(i)-(*this)).nullspace();
			Vec eigenVec(col);
			for(int j=0;j<col;j++)
				for(int k=0;k<col;k++)
					eigenVec.setData(eigenVec.getData(j)+nullMat.data[j][k],j);
			eigenVec.normalize();
			for(int j=0;j<col;j++)
				rm.data[j][i]=eigenVec.getData(j);
		}
		vecs=rm;
		vals=eigenVal;
	}
	else if(row==2){
		Vec eigenValues(row);
		vals=eigenValues;
		double poly[2];//x^2+b*x+c=0
		poly[0]=det();
		poly[1]=-data[0][0]-data[1][1];
#define _b poly[1]
#define _c poly[0]
#define quadric0 (-_b+pow(pow(_b,2)-4*_c,0.5))/2
#define quadric1 (-_b-pow(pow(_b,2)-4*_c,0.5))/2
		vals.setData(quadric0,0);
		vals.setData(quadric1,1);

		Mat a_l,eigenVectors(row,col);
		vecs=eigenVectors;
		for(int i=0;i<row;i++){
			a_l=*this;
			for(int j=0;j<row;j++)
				a_l.data[j][j]-=vals.getData(i);
			a_l.GaussJordanRowReduction();
			for(int j=row-1;j>=0;j--){
				double sum=0;
				if(a_l.data[j][j]==0){
					vecs.data[j][i]=1;
					continue;
				}
				for(int k=j+1;k<col;k++)
					sum+=a_l.data[j][k]*vecs.data[k][i];
				vecs.data[j][i]=(0-sum);
			}
		}
	}
}
int Mat::Rank(){
	Mat m(*this);
	for(int i=0,nr=0;i<(m.row<m.col?m.row:m.col);i++){
		int maxV=nr;
		for(int j=nr+1;j<m.row;j++)
			if(fabs(m.data[j][i])>fabs(m.data[maxV][i]))
				maxV=j;
		if(maxV!=nr)
			for(int j=i;j<m.col;j++){
				double t=m.data[nr][j];
				m.data[nr][j]=m.data[maxV][j];
				m.data[maxV][j]=t;
			}
		if(EQU(m.data[nr][i],0))
			continue;
		for(int j=nr+1;j<m.row;j++)
			for(int k=m.col-1;k>=i;k--)
				m.data[j][k]-=m.data[nr][k]*m.data[j][i]/m.data[nr][i];
		nr++;
	}
	int r=0;
	for(int i=0;i<m.row;i++){
		int j;
		for(j=0;j<m.col;j++)
			if(!EQU(m.data[i][j],0))
				break;
		r+=j!=m.col;
	}
	return r;
}
Mat Mat::LS(Mat& v){
	return (trans()*(*this)).Inverse()*trans()*v;
}
double Mat::PowerMethod(Vec& xn){
	if(row!=col) throw "PowerMethod失敗，not square matrix!";
	double lambda;
	Vec initVec(row);
	xn=initVec;
	xn.setI();
	for(int i=0;i<19999;i++)
		xn=(*this*xn).getColData(0).normal();
	lambda=(*this*xn).getColData(0)*xn/(xn*xn);
	return lambda;
}
Mat Mat::nullspace(){
	Mat m(*this);
	Mat rm=identity(m.col);
	for(int i=0;i<m.row;i++){
		int maxV=i;
		for(int j=i+1;j<m.row;j++)
			if(fabs(m.data[j][i])>fabs(m.data[maxV][i]))
				maxV=j;
		if(maxV!=i)
			for(int j=i;j<m.col;j++){
				double t=m.data[i][j];
				m.data[i][j]=m.data[maxV][j];
				m.data[maxV][j]=t;
			}
		if(EQU(m.data[i][i],0))
			continue;
		for(int j=i+1;j<m.row;j++)
			for(int k=m.col-1;k>=i;k--)
				m.data[j][k]-=m.data[i][k]*m.data[j][i]/m.data[i][i];
	}
	for(int i=0;i<m.row;i++){
		int r=0;
		while(r<col){
			if(!EQU(m.data[i][r],0))
				break;
			r++;
		}
		if(r==col)
			continue;
		if(!EQU(m.data[i][r],0))
			for(int j=m.col-1;j>=r;j--)
				m.data[i][j]/=m.data[i][r];
	}
	for(int i=m.row-1;i>=0;i--){
		int r=0;
		while(r<col){
			if(!EQU(m.data[i][r],0))
				break;
			r++;
		}
		if(r==col)
			continue;
		if(!EQU(m.data[i][r],0))
			for(int j=i-1;j>=0;j--)
				for(int k=m.col-1;k>=r;k--)
					m.data[j][k]-=m.data[i][k]*m.data[j][r];
	}
	for(int i=m.row-1;i>=0;i--){
		int r=0;
		while(r<col){
			if(!EQU(m.data[i][r],0))
				break;
			r++;
		}
		if(r==col)
			continue;
		if(!EQU(m.data[i][r],0))
			for(int j=0;j<col;j++)
					rm.data[r][j]-=m.data[i][j];
	}
	return rm;
}
std::string Mat::toString(){
	std::ostringstream out;
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++)
			out<<data[i][j]<<" ";
		out<<"\n";
	}
	return out.str();
}
