#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_lineEdit_returnPressed();

	void on_pushButton_Golden_clicked();

	void on_pushButton_Powell_clicked();

	void on_pushButton_Newton_clicked();

	void on_pushButton_Quasi_clicked();

	void on_pushButton_Steep_clicked();

	void on_pushButton_Conjugate_clicked();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
