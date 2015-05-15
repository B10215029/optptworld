#include "mainwindow.h"
#include "ui_mainwindow.h"

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
		result += pow(point.getData(0), func[i].degreeX) *
				pow(point.getData(1), func[i].degreeY) * func[i].coefficient;
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
		term t1={0,1,2};
		term t2={2,1,2};
		term t3={2,2,0};
		f.push_back(t1);
		f.push_back(t2);
		f.push_back(t3);

		//output
		QString fStr("f(x) = ");
		int varCount = 0;//計算用到幾個變數，用來決定f(x)或f(x,y)(但投影片好像只有f(x))
		for(int i=0;i<f.size();++i){
			QString termStr;
			if(f[i].coefficient >= 0 && i != 0)
				termStr += '+';
			termStr += QString::number(f[i].coefficient);
			if(f[i].degreeX != 0)
				termStr += QString(" * X^%1").arg(f[i].degreeX);
			if(f[i].degreeY != 0)
				termStr += QString(" * Y^%1").arg(f[i].degreeY);
			fStr += ' ' + termStr;
		}
		ui->textBrowser->append(fStr);
	}
}

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

void MainWindow::on_pushButton_Conjugate_clicked()
{

}
