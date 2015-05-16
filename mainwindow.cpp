#include "mainwindow.h"
#include "ui_mainwindow.h"

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
		double x=0;
		for(int j=0;j<func[i].degrees.size();++j)
			x*=pow(point.getData(j), func[i].degrees[j]);
		result +=x*func[i].coefficient;
	}
	return result;
}

void MainWindow::on_lineEdit_returnPressed()
{
	QStringList args = ui->lineEdit->text().split(' ');
	if(args[0] == "interval"){
		//read interval(假設用到interval的都是一維)
		interval.push_back(0);
		interval.push_back(2);

		//output
		QString intervalStr = QString::number(interval[0]);
		for(int i=1;i<interval.size();++i)
			intervalStr += ',' + QString::number(interval[i]);
		ui->textBrowser->append(QString("interval [%1]").arg(intervalStr));
	}
	else if(args[0] == "initial" && args[1] == "point"){
		//read initial point
		initialPoint = Vec(2);
		initialPoint.setData(-3, 0);
		initialPoint.setData(1, 1);

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

//QVector<term> MainWindow::toPostfix(QString& inputStr,int varCount){
//	QVector<term> result;
//	QStringList opList;
//	for(int i=0;i<inputStr.size();i++){
//		if(inputStr[i].isDigit()){//coefficient
//			double num=inputStr[i].toLatin1()-48,fraction=0;
//			while(inputStr[i+1].isDigit())
//				num=num*10+inputStr[++i].toLatin1()-48;
//			if(inputStr[i+1]=='.'){
//				i++;
//				int count=1;
//				while(inputStr[i+1].isDigit())
//					fraction+=(inputStr[++i].toLatin1()-48)/pow(10,count++);
//				num+=fraction;
//			}

//			term t;
//			t.coefficient=num;
//			if(++i>=inputStr.size()){//end
//				for(int j=0;j<varCount;j++)
//					t.degrees.push_back(0);
//				return result;
//			}
//			//////////x
//			else if(inputStr[i]=='x'){
//				if(++i>=inputStr.size()){//end
//					for(int j=0;j<varCount;j++){
//						if(j==0)
//							t.degrees.push_back(1);
//						t.degrees.push_back(0);
//					}
//					return result;
//				}
//				else if(inputStr[i]=='^'){//次方後的數字
//					double degreeNum=inputStr[i].toLatin1()-48,degreeFraction=0;
//					if(inputStr[++i].isDigit()){//一般數字直接寫法
//						while(inputStr[i+1].isDigit())
//							degreeNum=degreeNum*10+inputStr[++i].toLatin1()-48;
//						if(inputStr[i+1]=='.'){
//							i++;
//							int degreeNumCount=1;
//							while(inputStr[i+1].isDigit())
//								degreeFraction+=(inputStr[++i].toLatin1()-48)/pow(10,degreeNumCount++);
//							degreeNum+=degreeFraction;
//						}
//					}
//					else if(inputStr[i]=='('){//(0.5)之類的寫法
//						if(inputStr[++i].isDigit()){
//							while(inputStr[i+1].isDigit())
//								degreeNum=degreeNum*10+inputStr[++i].toLatin1()-48;
//							if(inputStr[i+1]=='.'){
//								i++;
//								int degreeNumCount=1;
//								while(inputStr[i+1].isDigit())
//									degreeFraction+=(inputStr[++i].toLatin1()-48)/pow(10,degreeNumCount++);
//								degreeNum+=degreeFraction;
//							}
//						}
//						i++;//inputStr[i]==')'
//					}
//					t.degrees.push_back(degreeNum);
//				}
//			}
//			//////////y
//			else if(inputStr[i]=='y'){
//				if(++i>=inputStr.size()){//end
//					if(t.degrees.count()==0)//no x
//						t.degrees.push_back(0);
//					for(int j=1;j<varCount;j++){
//						if(j==1)
//							t.degrees.push_back(1);
//						t.degrees.push_back(0);
//					}
//					return result;
//				}
//				else if(inputStr[i]=='^'){//次方後的數字
//					double degreeNum=inputStr[i].toLatin1()-48,degreeFraction=0;
//					if(inputStr[++i].isDigit()){//一般數字直接寫法
//						while(inputStr[i+1].isDigit())
//							degreeNum=degreeNum*10+inputStr[++i].toLatin1()-48;
//						if(inputStr[i+1]=='.'){
//							i++;
//							int degreeNumCount=1;
//							while(inputStr[i+1].isDigit())
//								degreeFraction+=(inputStr[++i].toLatin1()-48)/pow(10,degreeNumCount++);
//							degreeNum+=degreeFraction;
//						}
//					}
//					else if(inputStr[i]=='('){//(0.5)之類的寫法
//						if(inputStr[++i].isDigit()){
//							while(inputStr[i+1].isDigit())
//								degreeNum=degreeNum*10+inputStr[++i].toLatin1()-48;
//							if(inputStr[i+1]=='.'){
//								i++;
//								int degreeNumCount=1;
//								while(inputStr[i+1].isDigit())
//									degreeFraction+=(inputStr[++i].toLatin1()-48)/pow(10,degreeNumCount++);
//								degreeNum+=degreeFraction;
//							}
//						}
//						i++;//inputStr[i]==')'
//					}
//					t.degrees.push_back(degreeNum);
//				}
//			}
//			result.push_back(t);
//		}
//		else if(inputStr[i]=='.'){
//			double fraction=0;
//			int count=1;
//			while(inputStr[i+1].isDigit())
//				fraction+=(inputStr[++i].toLatin1()-48)/pow(10,count++);

//			for(int i=0;i<varCount;i++)
//				t.degrees.push_back(0);
//			t.coefficient=fraction;
//			result.push_back(t);
//		}
//		else if(inputStr[i]=='x'){
//			for(int i=0;i<varCount;i++)
//				t.degrees.push_back(0);
//			t.coefficient=fraction;
//			result.push_back(t);
//		}
//		else if(inputStr[i]=='y'){
//			int indexNum,indexOffset=-97;
//			while(inputStr[++i]=='z') //mza=m[25]
//				indexOffset+=25;
//			if(!inputStr[i].isLetter()) throw "Input Error!";
//			indexNum=inputStr[i].toLatin1()+indexOffset;
//			if(indexNum>=m.size()) throw "V Out Of Range!";
//			result.push(m[indexNum]);
//		}
//		else if(inputStr[i]=='+'){
//			if(opList.isEmpty()) opList+="+";
//			else if(opList.last()=="(") opList+="+";
//			else{
//				QString inst=opList.last();
//				opList.removeLast();
//				calc(inst,result);
//				opList+="+";
//			}
//		}
//		else if(inputStr[i]=='-'){
//			if(opList.isEmpty()) opList+="-";
//			else if(opList.last()=="(") opList+="-";
//			else{
//				QString inst=opList.last();
//				opList.removeLast();
//				calc(inst,result);
//				opList+="-";
//			}
//		}
//		else if(inputStr[i]=='*'){
//			if(opList.isEmpty()) opList+="*";
//			else if(opList.last()=="+"||opList.last()=="-"||opList.last()=="(") opList+="*";
//			else{
//				QString inst=opList.last();
//				opList.removeLast();
//				calc(inst,result);
//				opList+="*";
//			}
//		}
//		else if(inputStr[i]=='/'){
//			if(opList.isEmpty()) opList+="/";
//			else if(opList.last()=="+"||opList.last()=="-"||opList.last()=="(") opList+="/";
//			else{
//				QString inst=opList.last();
//				opList.removeLast();
//				calc(inst,result);
//				opList+="/";
//			}
//		}
//		else if(inputStr[i]=='('){
//			opList+="(";
//		}
//	}
//}

void MainWindow::on_pushButton_Golden_clicked()
{

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

Vec MainWindow::deltaf(QVector<QVector<term>>& df,Vec& xi){
	Vec v(f[0].degrees.size());
	for(int i=0;i<f.size();i++)
		v.setData(calculateFunction(df[i],xi),i);
	return v;
}

QVector<term> MainWindow::variableInFuncToNewFunc(QVector<term> func,QVector<QString>& x){//代入另一變數
	QVector<term> newf=func;
	QString s;
	for(int i=0;i<newf.size();i++){
		s=;
	}
	return newf;
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
		//jump //compute length[i]
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
