#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <cmath>
#include "Vec.h"
#include "Mat.h"

typedef struct {
	QVector<double> degrees;
	double coefficient;
} term;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	static double calculateFunction(QVector<term> func,Vec point);
	static double goldenSection(QVector<term> func, double intervalA, double intervalB);
	static QVector<term> polMul(QVector<term> funcA, QVector<term> funcB);
	static QVector<term> polPow(QVector<term> func, int e);
	static QVector<term> polAdd(QVector<term> funcA, QVector<term> funcB);
	static QVector<term> pIntoF(QVector<term> func, Vec pos, Vec dir);

private slots:
	void on_lineEdit_returnPressed();

	void on_pushButton_Golden_clicked();

	void on_pushButton_Powell_clicked();

	void on_pushButton_Newton_clicked();

	void on_pushButton_Quasi_clicked();

	void on_pushButton_Steep_clicked();

	void on_pushButton_Conjugate_clicked();

	void on_actionClear_triggered();

private:
	Ui::MainWindow *ui;
	QVector<term> f;
	QVector<double> interval;
	Vec initialPoint;

	Vec deltaf(QVector< QVector<term> >& df,Vec& xi);
	QVector<term> variableInFuncToNewFunc(QVector<term>& func,QVector<QString>& x);
};

#endif // MAINWINDOW_H
