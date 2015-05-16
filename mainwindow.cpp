#include "mainwindow.h"
#include "ui_mainwindow.h"

#define numberOfVar 2
#define GOLDEN_RATE 0.3819660112501051
#define number_of_iterations 99
#define epslon1 0.00000000001
#define epslon2 0.00000000001
#define epslon3 0.00000000001

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_lineEdit_returnPressed()//input
{
	QStringList args = ui->lineEdit->text().split(' ');
	if(args.size() == 2 && args[0] == "interval"){
		//read interval(假設用到interval的都是一維)
		interval.clear();
		QStringList intargs = args[1].split('[')[1].split(']')[0].split(',');
		for(int i=0;i<intargs.size();++i)
			interval.push_back(intargs[i].toDouble());

		//output
		QString intervalStr = QString::number(interval[0]);
		for(int i=1;i<interval.size();++i)
			intervalStr += ',' + QString::number(interval[i]);
		ui->textBrowser->append(QString("interval [%1]").arg(intervalStr));
	}
	else if(args.size() == 3 && args[0] == "initial" && args[1] == "point"){
		//read initial point
		QStringList iniargs = args[2].split('(')[1].split(')')[0].split(',');
		initialPoint = Vec(iniargs.size());
		for(int i=0;i<iniargs.size();++i)
			initialPoint.setData(iniargs[i].toDouble(), i);

		//output
		ui->textBrowser->append(QString("initial point (%1)").arg(QString::fromStdString(initialPoint.toString())));
	}
	else{
		//read function
		if(ui->lineEdit->text().split('=').size()!=2){
			ui->textBrowser->append("unknow input");
			return;
		}
		f.clear();
		QString str = ui->lineEdit->text().split('=')[1];
		int currentVar = 0;
		bool readExp = false;
		bool readedNum = false;
		term currentTerm;
		for(int vn=0;vn<numberOfVar;++vn)
			currentTerm.degrees.push_back(0);
		currentTerm.coefficient = 1;

		for(int i=0;i<str.size();++i){
			if(QString(" (*)").indexOf(str[i])!=-1)
				continue;
			else if(str[i].isDigit() || str[i]=='+' || str[i]=='-'){
				double sign = 1,num = 0;
				int dot = 0;
				if(str[i]=='+' || str[i]=='-'){
					if(!readExp && (f.size()!=0 || readedNum)){
						f.push_back(currentTerm);
						for(int vn=0;vn<numberOfVar;++vn)
							currentTerm.degrees[vn] = 0;
						currentTerm.coefficient = 1;
						currentVar = 0;
					}
					sign = str[i++]=='+'?1:-1;
				}
				readedNum = true;
				for(;i<str.size();++i){//number
					if(str[i] == '.')
						dot = 1;
					else if(str[i].isDigit()){
						if(dot == 0)
							num = num*10 + (str[i].toLatin1() - (int)'0');
						else{
							num += pow(10.0, -dot)*(str[i].toLatin1() - (int)'0');
							dot++;
						}
					}
					else{
						i--;
						break;
					}
				}
				num *= sign;
				if(currentVar == 0){
					if(str[i]=='+' || str[i]=='-')
						currentTerm.coefficient = sign;
					else
						currentTerm.coefficient = num;
				}
				if(readExp){
					currentTerm.degrees[currentVar-1] = num;
					readExp = false;
				}
			}
			else if(str[i].toLower() == 'x'){
				currentVar = 1;
				currentTerm.degrees[currentVar-1] = 1;
			}
			else if(str[i].toLower() == 'y'){
				currentVar = 2;
				currentTerm.degrees[currentVar-1] = 1;
			}
			else if(str[i] == '^')
				readExp = true;
		}
		f.push_back(currentTerm);

		//output
		QString fStr("f(x) = ");
		for(int i=0;i<f.size();++i){
			QString termStr;
			if(f[i].coefficient >= 0 && i != 0)
				termStr += '+';
			if(f[i].coefficient != 1)
				termStr += QString::number(f[i].coefficient);
			if(f[i].degrees[0] != 0){
				if(f[i].degrees[0] == 1)
					termStr += QString("X");
				else
					termStr += QString("X^%1").arg(f[i].degrees[0]);
			}
			if(f[i].degrees[1] != 0){
				if(f[i].degrees[1] == 1)
					termStr += QString("Y");
				else
					termStr += QString("Y^%1").arg(f[i].degrees[1]);
			}
			fStr += ' ' + termStr;
		}
		ui->textBrowser->append(fStr);
	}
}

double MainWindow::calculateFunction(QVector<term> func,Vec point)
{
	double result = 0;
	for(int i=0;i<func.size();++i){
		double x=1;
		for(int j=0;j<func[i].degrees.size();++j)
			x*=pow(point.getData(j), func[i].degrees[j]);
		result +=x*func[i].coefficient;
	}
	return result;
}

double MainWindow::goldenSection(QVector<term> func,double intervalA,double intervalB){
	double a = intervalA;
	double b = intervalB;
	double c1,c2,fc1,fc2;
	while(1){
		c1 = a + (b-a)*GOLDEN_RATE;
		c2 = a + (b-a)*(1-GOLDEN_RATE);
		Vec c1v(2);
		c1v.setData(c1, 0);
		Vec c2v(2);
		c2v.setData(c2, 0);
		fc1 = calculateFunction(func, c1v);
		fc2 = calculateFunction(func, c2v);
		if(fc1 > fc2)
			a=c1;
		else
			b=c2;
		if(b-a<epslon1)
			break;
	}
	return c1;
}

QVector<term> MainWindow::polMul(QVector<term>& funcA,QVector<term>& funcB){
	QVector<term> funcR;
	for(int i=0;i<funcA.size();++i){
		for(int j=0;j<funcB.size();++j){
			term t;
			for(int vn=0;vn<numberOfVar;++vn)
				t.degrees.push_back(funcA[i].degrees[vn]+funcB[j].degrees[vn]);
			t.coefficient = funcA[i].coefficient * funcB[j].coefficient;
			funcR.push_back(t);
		}
	}
	return funcR;
}

QVector<term> MainWindow::polPow(QVector<term>& func,int e){
	QVector<term> funcR;
	term t;
	for(int vn=0;vn<numberOfVar;++vn)
		t.degrees.push_back(0);
	t.coefficient = 1;
	funcR.push_back(t);
	for(int i=0;i<e;i++)
		funcR = polMul(funcR, func);
	return funcR;
}

QVector<term> MainWindow::polAdd(QVector<term>& funcA,QVector<term>& funcB){//combine
	QVector<term> funcR;
	for(int i=0;i<funcA.size();++i)
		funcR.push_back(funcA[i]);
	for(int i=0;i<funcB.size();++i)
		funcR.push_back(funcB[i]);
	return funcR;
}

QVector<term> MainWindow::pIntoF(QVector<term>& func,Vec& pos,Vec& dir){
	QVector<term> funcR;
	for(int i=0;i<func.size();++i){
		QVector<term> fc;//coefficient function
		QVector<term> fx;//x function
		QVector<term> fy;//y function
		term tt;//temp term
		for(int vn=0;vn<numberOfVar;++vn)
			tt.degrees.push_back(0);
		tt.coefficient = func[i].coefficient;
		fc.push_back(tt);

		tt.degrees[0] = 1;
		tt.coefficient = dir.getData(0);
		fx.push_back(tt);
		tt.degrees[0] = 0;
		tt.coefficient = pos.getData(0);
		fx.push_back(tt);
		fx = polPow(fx, (int)func[i].degrees[0]);

		tt.degrees[0] = 1;
		tt.coefficient = dir.getData(1);
		fy.push_back(tt);
		tt.degrees[0] = 0;
		tt.coefficient = pos.getData(1);
		fy.push_back(tt);
		fy = polPow(fy, (int)func[i].degrees[1]);

		fc = polMul(fc, fx);
		fc = polMul(fc, fy);
		funcR = polAdd(funcR, fc);
	}
	return funcR;
}

void MainWindow::on_pushButton_Golden_clicked()
{
	double a = interval[0];
	double b = interval[1];
	double c1,c2,fc1,fc2,ba;
	ui->textBrowser->append("a\tb\tc1\tc2\tf(c1)\tf(c2)\tupdate\tb - a");
	while(1){
		QString ostr("%1\t%2\t%3\t%4\t%5\t%6\t%7 = c%8\t%9");
		ostr = ostr.arg(a).arg(b);
		c1 = a + (b-a)*GOLDEN_RATE;
		c2 = a + (b-a)*(1-GOLDEN_RATE);
		ostr = ostr.arg(c1).arg(c2);
		Vec c1v(2);
		c1v.setData(c1, 0);
		Vec c2v(2);
		c2v.setData(c2, 0);
		fc1 = calculateFunction(f, c1v);
		fc2 = calculateFunction(f, c2v);
		ostr = ostr.arg(fc1).arg(fc2);
		if(fc1 > fc2){
			a=c1;
			ostr = ostr.arg('a').arg('1');
		}
		else{
			b=c2;
			ostr = ostr.arg('b').arg('2');
		}
		ba = b-a;
		ostr = ostr.arg(ba);
		ui->textBrowser->append(ostr);
		if(ba<epslon1)
			break;
	}
}

void MainWindow::on_pushButton_Powell_clicked()
{
	Vec point(initialPoint);
	QVector<Vec> s;
	s.push_back(Vec(2));
	s.push_back(Vec(2));
	s[0].setData(1.0, 0);
	s[0].setData(0.0, 1);
	s[1].setData(0.0, 0);
	s[1].setData(1.0, 1);
	while(1){
		Vec x1 = point;
		double fx1 = calculateFunction(f, x1);
		QVector<term> fa1 = pIntoF(f, x1, s[0]);
		double a1 = goldenSection(fa1, interval[0], interval[1]);
		ui->textBrowser->append("X1="+QString::fromStdString(x1.toString())+"\tf(X1)="+QString::number(fx1));

		Vec x2(2);
		x2.setData(x1.getData(0)+a1*s[0].getData(0),0);
		x2.setData(x1.getData(1)+a1*s[0].getData(1),1);
		double fx2 = calculateFunction(f, x2);
		QVector<term> fa2 = pIntoF(f, x2, s[1]);
		double a2 = goldenSection(fa2, interval[0], interval[1]);
		ui->textBrowser->append("X2="+QString::fromStdString(x2.toString())+"\tf(X2)="+QString::number(fx2));

		Vec x3(2);
		x3.setData(x2.getData(0)+a2*s[1].getData(0),0);
		x3.setData(x2.getData(1)+a2*s[1].getData(1),1);
		double fx3 = calculateFunction(f, x3);
		ui->textBrowser->append("X3="+QString::fromStdString(x3.toString())+"\tf(X3)="+QString::number(fx3));

		Vec s3(2);//new direction
		s3.setData(a1*s[0].getData(0)+a2*s[1].getData(0),0);
		s3.setData(a1*s[0].getData(1)+a2*s[1].getData(1),1);
		s.push_back(s3);
		s.remove(0);

		QVector<term> fa3 = pIntoF(f, x3, s[1]);
		double a3 = goldenSection(fa3, interval[0], interval[1]);
		Vec x4(2);
		x4.setData(x3.getData(0)+a3*s[1].getData(0),0);
		x4.setData(x3.getData(1)+a3*s[1].getData(1),1);
		double fx4 = calculateFunction(f, x4);
		point = x4;
		ui->textBrowser->append("X4="+QString::fromStdString(x4.toString())+"\tf(X4)="+QString::number(fx4));

		if(abs(fx1-fx4)<epslon1 || abs(x1.norm()-x4.norm())<epslon2)
			break;
	}
}

void MainWindow::on_pushButton_Newton_clicked()
{
	//只是測試一下pintof
	//結果是好像沒問題
	QVector<Vec> s;
	s.push_back(Vec(2));
	s.push_back(Vec(2));
	s[0].setData(1.0, 0);
	s[0].setData(0.0, 1);
	s[1].setData(0.0, 0);
	s[1].setData(1.0, 1);
	Vec x1 = initialPoint;
	double fx1 = calculateFunction(f, x1);
	QVector<term> fa1 = pIntoF(f, x1, s[0]);
	double a1 = goldenSection(fa1, interval[0], interval[1]);
	Vec x2(2);
	x2.setData(x1.getData(0)+a1*s[0].getData(0),0);
	x2.setData(x1.getData(1)+a1*s[0].getData(1),1);
	double fx2 = calculateFunction(f, x2);
	ui->textBrowser->append(QString::number(a1));
	ui->textBrowser->append(QString::number(fx1));
	ui->textBrowser->append(QString::number(fx2));
}

void MainWindow::on_pushButton_Quasi_clicked()
{

}

void MainWindow::on_pushButton_Steep_clicked()
{

}

QVector<term> diff(QVector<term> func,int xi){
	QVector<term> fdxi=func;
	for(int i=0;i<func.size();i++){
		fdxi[i].coefficient*=fdxi[i].degrees[xi];
		fdxi[i].degrees[xi]--;
	}
	return fdxi;
}

Vec MainWindow::deltaf(QVector< QVector<term> >& df,Vec& xi){
	Vec v(numberOfVar);
	for(int i=0;i<numberOfVar;i++)
		v.setData(calculateFunction(df[i],xi),i);
	return v;
}

void MainWindow::on_pushButton_Conjugate_clicked()
{
	QVector<Vec> X,direction;
	X.push_back(initialPoint);

	QVector< QVector<term> > df;
	for(int i=0;i<numberOfVar;i++)
		df.push_back(diff(f,i));

	QVector<double> length;

	for(int i=0;i<number_of_iterations;i++){
		if(i==0)
			direction.push_back(-1*deltaf(df,X[i]));
		else{
			double beta=deltaf(df,X[i])*deltaf(df,X[i])/(deltaf(df,X[i-1])*deltaf(df,X[i-1]));
			direction.push_back(-1*deltaf(df,X[i])+beta*direction[i-1]);
		}
		//jump //compute length[i]
		length.push_back(goldenSection(pIntoF(f,X[i],direction[i]),interval[0],interval[1]));
		X.push_back(X[i]+length[i]*direction[i]);
		//check
		if(calculateFunction(f,X[i+1])-calculateFunction(f,X[i])<=epslon1){
			ui->textBrowser->append("epslon1");
			ui->textBrowser->append("X="+QString::fromStdString(X.last().toString())+"\tf(X)="+QString::number(calculateFunction(f,X.last())));
			return;
		}
		else if((X[i+1]-X[i]).norm()<=epslon2){
			ui->textBrowser->append("epslon2");
			ui->textBrowser->append("X="+QString::fromStdString(X.last().toString())+"\tf(X)="+QString::number(calculateFunction(f,X.last())));
			return;
		}
		else if(deltaf(df,X[i+1])*deltaf(df,X[i+1])<=epslon3){
			ui->textBrowser->append("epslon3");
			ui->textBrowser->append("X="+QString::fromStdString(X.last().toString())+"\tf(X)="+QString::number(calculateFunction(f,X.last())));
			return;
		}
	}
	ui->textBrowser->append("X="+QString::fromStdString(X.last().toString())+"\tf(X)="+QString::number(calculateFunction(f,X.last())));
}

void MainWindow::on_actionClear_triggered()
{
	ui->textBrowser->clear();
}
