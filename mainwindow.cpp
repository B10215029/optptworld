#include "mainwindow.h"
#include "ui_mainwindow.h"

#define GOLDEN_RATE 0.3819660112501051
#define number_of_iterations 99
#define epslon1 0.0000001
#define epslon2 0.0000001
#define epslon3 0.0000001

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

void MainWindow::on_lineEdit_returnPressed()
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
		term currentTerm;
		currentTerm.degrees.push_back(0);
		currentTerm.degrees.push_back(0);
		currentTerm.coefficient = 1;
		for(int i=0;i<str.size();++i){
			if(QString(" (*)").indexOf(str[i])!=-1)
				continue;
			else if(str[i].isDigit() || str[i]=='+' || str[i]=='-'){
				double sign = 1;
				double num = 0;
				int dot = 0;
				if(str[i]=='+' || str[i]=='-'){
					if(!readExp){
						f.push_back(currentTerm);
						currentTerm.degrees[0] = 0;
						currentTerm.degrees[1] = 0;
						currentTerm.coefficient = 1;
						currentVar = 0;
					}
					sign = str[i++]=='+'?1:-1;
				}
				for(;i<str.size();++i){
					if(str[i] == '.')
						dot = 1;
					else if(str[i].isDigit()){
						if(dot == 0){
							num = num*10 + ((int)str[i].toLatin1() - (int)'0');
						}
						else{
							num += pow(10.0, -dot)*((int)str[i].toLatin1() - (int)'0');
							dot++;
						}
					}
					else{
						i--;
						break;
					}
				}
				num *= sign;
				//ui->textBrowser->append(QString::number(num));
				if(currentVar == 0){
					currentTerm.coefficient = num;
					if(num == 0)
						currentTerm.coefficient = sign;
				}
				if(readExp){
					if(currentVar == 1)
						currentTerm.degrees[0] = num;
					else if(currentVar == 2)
						currentTerm.degrees[1] = num;
					readExp = false;
				}
			}
			else if(str[i].toLower() == 'x'){
				currentVar = 1;
				currentTerm.degrees[0] = 1;
			}
			else if(str[i].toLower() == 'y'){
				currentVar = 2;
				currentTerm.degrees[1] = 1;
			}
			else if(str[i] == '^'){
				readExp = true;
			}
		}
		f.push_back(currentTerm);

		//output
		QString fStr("f(x) = ");
		//int varCount = 0;//計算用到幾個變數，用來決定f(x)或f(x,y)(但投影片好像只有f(x))
		for(int i=0;i<f.size();++i){
			QString termStr;
			if(f[i].coefficient >= 0 && i != 0)
				termStr += '+';
			termStr += QString::number(f[i].coefficient);
			if(f[i].degrees[0] != 0)
				termStr += QString(" * X^%1").arg(f[i].degrees[0]);
			if(f[i].degrees[1] != 0)
				termStr += QString(" * Y^%1").arg(f[i].degrees[1]);
			fStr += ' ' + termStr;
		}
		ui->textBrowser->append(fStr);
	}
}

void MainWindow::on_pushButton_Golden_clicked()
{
	double a = interval[0];
	double b = interval[1];
	double c1;
	double c2;
	double fc1;
	double fc2;
	double ba;
	while(1){
		QString ostr("%1\t%2\t%3\t%4\t%5\t%6\t%7 = C%8\t%9");
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

}

void MainWindow::on_pushButton_Newton_clicked()
{

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

QVector<term> MainWindow::variableInFuncToNewFunc(QVector<term>& func,QVector<QString>& x){
	QVector<term> newf;
	return newf;
}

Vec MainWindow::deltaf(QVector<QVector<term>>& df,Vec& xi){
	Vec v(f[0].degrees.size());
	for(int i=0;i<f.size();i++)
		v.setData(calculateFunction(df[i],xi),i);
	return v;
}

void MainWindow::on_pushButton_Conjugate_clicked()
{
	QVector<Vec> X,direction;
	X.push_back(initialPoint);

	QVector<QVector<term>> df;
	for(int i=0;i<f.size();i++)
		df.push_back(diff(f,i));

	QVector<double> length;

	for(int i=0;i<number_of_iterations;i++){
		if(i==0)
			direction[0]=-1*deltaf(df,X[i]);
		else{
			double beta=deltaf(df,X[i])*deltaf(df,X[i])/(deltaf(df,X[i-1])*deltaf(df,X[i-1]));
			direction[i]=-1*deltaf(df,X[i])+beta*direction[i-1];
		}
		//jump

		X.push_back(X[i]+length[i]*direction[i]);
		//check
		if(calculateFunction(f,X[i+1])-calculateFunction(f,X[i])<=epslon1){
			ui->textBrowser->append("epslon1");
			return;
		}
		else if((X[i+1]-X[i]).norm()<=epslon1){
			ui->textBrowser->append("epslon2");
			return;
		}
		else if(deltaf(df,X[i+1])*deltaf(df,X[i+1])<=epslon3){
			ui->textBrowser->append("epslon3");
			return;
		}
	}
}














