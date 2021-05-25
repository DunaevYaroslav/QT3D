#pragma once
#include <stdio.h>
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QAction>
#include <QMenuBar>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSlider>
#include <QtDataVisualization>
#include <QtDataVisualization/Q3DSurface>
#include <iostream>

using namespace QtDataVisualization;

class Window : public QObject
{
	Q_OBJECT

private:
        double a, b, c, d;
        int m, n, id;
        int flag;
        int delt;

        double *x;
        double *y;
        double xleft;
        double xright;
        double yleft;
        double yright;
        double *coeffs;
        double (*f)(double, double);

	Q3DSurface *m_graph;
    QSurfaceDataProxy *m_funcProxy;
	QSurfaceDataProxy *m_methodProxy;
	QSurfaceDataProxy *m_residProxy;

    QSurface3DSeries *m_funcSeries;
	QSurface3DSeries *m_methodSeries;
	QSurface3DSeries *m_residSeries;

        float min_meth();
        float max_meth();
        void create_arrays();
        void delete_arrays();

public:
    QLabel *text1;
    explicit Window(Q3DSurface *surface,QLabel *text);
	~Window();


    float max_func();
    float min_func();
    double giveA();
    double giveB();
    double giveC();
    double giveD();
    int giveN();
    int giveM();

    void enablefuncModel(bool enable);
    void enableResidual(bool enable);
    void enableMethod(bool enable);
    void setGreenToRedGradient();

    int parse_command_line(int argc, char *argv[]);

    void fillfuncProxy();
    void fillMethod();
    void fillResidual();

    void method();
    double poly(double x_0, double y_0);

    char* text();

public slots:
        void incr_n();
        void incr_m();
        void decr_n();
        void decr_m();
        void incr_a_b();
        void incr_c_d();
        void decr_a_b();
        void decr_c_d();

};

double A(double x, int k, int l);
double Pow(int k, double x);
double min(double x, double y);
double max(double x, double y);
