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

void MainWindow::on_lineEdit_returnPressed()
{
	ui->textBrowser->append(ui->lineEdit->text());
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
