#include <QPainter>
#include <stdio.h>
#include <cmath>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMessageBox>
#include <QtWidgets>
#include "window.h"

using namespace QtDataVisualization;

#define DEFAULT_A -10
#define DEFAULT_B 10
#define DEFAULT_N 60


double func(double x, double y)
{
   return qSqrt(x*x+y*y);
}

const float Average = 150;

Window::Window(Q3DSurface *surface,QLabel * text12)
	: m_graph(surface)
{
	m_graph->setAxisX(new QValue3DAxis);
	m_graph->setAxisY(new QValue3DAxis);
	m_graph->setAxisZ(new QValue3DAxis);

        m_funcProxy= new QSurfaceDataProxy();
        m_funcSeries = new QSurface3DSeries(m_funcProxy);

	m_methodProxy = new QSurfaceDataProxy();
	m_methodSeries = new QSurface3DSeries(m_methodProxy);

	m_residProxy = new QSurfaceDataProxy();
	m_residSeries = new QSurface3DSeries(m_residProxy);

	a = DEFAULT_A;
	b = DEFAULT_B;
	c = DEFAULT_A;
	d = DEFAULT_B;
	m = DEFAULT_N;
	n = DEFAULT_N;
	id = 0;
    flag=0;
    text1=text12;

    f = func;

	x = NULL;
	y = NULL;
	coeffs = NULL;
}

Window::~Window() {
	if (x) delete[] x;
	if (y) delete[] y;
	if (coeffs) delete[] coeffs;
	delete m_graph;
}

void Window::delete_arrays()
{
    if (x) delete[] x;
    if (y) delete[] y;
    if (coeffs) delete[] coeffs;
}

void Window::create_arrays()
{
    x = new double[n];
    y = new double[m];
    coeffs = new double[16 * n*m];
}

void Window::fillMethod()
{
    char u[200];
    sprintf(u,"a = %.2f\nb = %.2f\nc = %.2f\nd = %.2f\nm = %d\nn = %d\nMax = %.2f\nMin = %.2f",a,b,c,d,m,n,max_func(),min_func());
    text1->setText(QString(u));
    flag=1;

    delete_arrays();
    create_arrays();

	float stepX = (b - a) / float(n);
	float stepY = (d - c) / float(m);

    xleft=a-stepX;
    for (int i = 0; i < n; i++)
		x[i] = i * stepX + a;
    xright=b+stepX;

    yleft=c-stepY;
    for (int i = 0; i < m; i++)
		y[i] = i * stepY + c;
    yright=d+stepY;

	method();

	stepX = (b - a) / float(Average);
	float stepZ = (d - c) / float(Average);

	QSurfaceDataArray *dataArray = new QSurfaceDataArray;
	dataArray->reserve(Average);
    for (int i = 0; i < Average; i++) {
		QSurfaceDataRow *newRow = new QSurfaceDataRow(Average);
		float _z = i * stepZ + c;
		int index = 0;
        for (int j = 0; j < Average; j++) {
			float _x = j * stepX + a;
			float _y = poly(_x, _z);
			(*newRow)[index++].setPosition(QVector3D(_x, _y, _z));
		}
		*dataArray << newRow;
	}

	m_methodProxy->resetArray(dataArray);
}

double Window::giveA()
{
    return a;
}

double Window::giveC()
{
    return c;
}

double Window::giveB()
{
    return b;
}

double Window::giveD()
{
    return d;
}

int Window::giveN()
{
    return n;
}

int Window::giveM()
{
    return m;
}


void Window::fillfuncProxy()
{
    char u[200];
    sprintf(u,"a = %.2f\nb = %.2f\nc = %.2f\nd = %.2f\nm = %d\nn = %d\nMax = %.2f\nMin = %.2f",a,b,c,d,m,n,max_func(),min_func());
    text1->setText(QString(u));
    flag=2;
	float stepX = (b - a) / float(Average);
	float stepZ = (d - c) / float(Average);

	QSurfaceDataArray *dataArray = new QSurfaceDataArray;
	dataArray->reserve(Average);
	for (int i = 0; i < Average; i++) {
		QSurfaceDataRow *newRow = new QSurfaceDataRow(Average);
		float _z = i * stepZ + c;
		int index = 0;
		for (int j = 0; j < Average; j++) {
			float _x = j * stepX + a;
			(*newRow)[index++].setPosition(QVector3D(_x, f(_x, _z), _z));
		}
		*dataArray << newRow;
	}

    m_funcProxy->resetArray(dataArray);
}

void Window::fillResidual()
{
    char u[200];
    sprintf(u,"a = %.2f\nb = %.2f\nc = %.2f\nd = %.2f\nm = %d\nn = %d\nMax = %.2f\nMin = %.2f",a,b,c,d,m,n,max_func(),min_func());
    text1->setText(QString(u));
    flag=3;
	float stepX = (b - a) / float(Average);
	float stepZ = (d - c) / float(Average);

	QSurfaceDataArray *dataArray = new QSurfaceDataArray;
	dataArray->reserve(Average);
	for (int i = 0; i < Average; i++) {
		QSurfaceDataRow *newRow = new QSurfaceDataRow(Average);
		float _z = i * stepZ + c;
		int index = 0;
		for (int j = 0; j < Average; j++) {
			float _x = j * stepX + a;
			float _y = f(_x, _z) - poly(_x, _z);
			(*newRow)[index++].setPosition(QVector3D(_x, _y, _z));
		}
		*dataArray << newRow;
	}

	m_residProxy->resetArray(dataArray);
}

void Window::enableResidual(bool enable)
{
    char u[200];
    sprintf(u,"a = %.2f\nb = %.2f\nc = %.2f\nd = %.2f\nm = %d\nn = %d\nMax = %.2f\nMin = %.2f",a,b,c,d,m,n,max_func(),min_func());
    this->text1->setText(QString(u));
	if (enable) {
		m_residSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
		m_residSeries->setFlatShadingEnabled(true);

		m_graph->axisX()->setLabelFormat("%.2f");
		m_graph->axisZ()->setLabelFormat("%.2f");
		m_graph->axisX()->setRange(a, b);
		m_graph->axisY()->setRange(-2, 2);
		m_graph->axisZ()->setRange(c, d);
		m_graph->axisX()->setLabelAutoRotation(30);
		m_graph->axisY()->setLabelAutoRotation(90);
		m_graph->axisZ()->setLabelAutoRotation(30);

		if (id == 0)
            m_graph->removeSeries(m_funcSeries);
		if (id == 1)
			m_graph->removeSeries(m_methodSeries);
		id = 2;
		m_graph->addSeries(m_residSeries);

	}
}

void Window::enablefuncModel(bool enable)
{
    char u[200];
    sprintf(u,"a = %.2f\nb = %.2f\nc = %.2f\nd = %.2f\nm = %d\nn = %d\nMax = %.2f\nMin = %.2f",a,b,c,d,m,n,max_func(),min_func());
    text1->setText(QString(u));

	if (enable) {
        m_funcSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        m_funcSeries->setFlatShadingEnabled(true);

		m_graph->axisX()->setLabelFormat("%.2f");
		m_graph->axisZ()->setLabelFormat("%.2f");
		m_graph->axisX()->setRange(a, b);
		m_graph->axisY()->setRange(min_func(), max_func());
		m_graph->axisZ()->setRange(c, d);
		m_graph->axisX()->setLabelAutoRotation(30);
		m_graph->axisY()->setLabelAutoRotation(90);
		m_graph->axisZ()->setLabelAutoRotation(30);

		if (id == 2)
			m_graph->removeSeries(m_residSeries);
		if (id == 1)
			m_graph->removeSeries(m_methodSeries);
		id = 0;
        m_graph->addSeries(m_funcSeries);
	}
}

void Window::enableMethod(bool enable)
{
    char u[200];
    sprintf(u,"a = %.2f\nb = %.2f\nc = %.2f\nd = %.2f\nm = %d\nn = %d\nMax = %.2f\nMin = %.2f",a,b,c,d,m,n,max_func(),min_func());
    text1->setText(QString(u));
	if (enable) {
		m_methodSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
		m_methodSeries->setFlatShadingEnabled(true);

		m_graph->axisX()->setLabelFormat("%.2f");
		m_graph->axisZ()->setLabelFormat("%.2f");
		m_graph->axisX()->setRange(a, b);
		m_graph->axisY()->setRange(min_meth(), max_meth());
		m_graph->axisZ()->setRange(c, d);
		m_graph->axisX()->setLabelAutoRotation(30);
		m_graph->axisY()->setLabelAutoRotation(90);
		m_graph->axisZ()->setLabelAutoRotation(30);

		if (id == 2)
			m_graph->removeSeries(m_residSeries);
		if (id == 0)
            m_graph->removeSeries(m_funcSeries);
		id = 1;
		m_graph->addSeries(m_methodSeries);
	}
}

void Window::setGreenToRedGradient()
{
	QLinearGradient gr;
	gr.setColorAt(0.0, Qt::darkGreen);
	gr.setColorAt(0.5, Qt::yellow);
	gr.setColorAt(0.8, Qt::red);
	gr.setColorAt(1.0, Qt::darkRed);

	m_graph->seriesList().at(0)->setBaseGradient(gr);
	m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

int Window::parse_command_line(int argc, char *argv[])
{
	QMessageBox msg;
	if (argc == 1) {

		fillMethod();
                fillfuncProxy();
                fillResidual();

		msg.setText("Default running\n");
		msg.exec();
		return 0;
	}
	if (argc == 7) {
		if (sscanf(argv[1], "%lf", &a) != 1
			|| sscanf(argv[2], "%lf", &b) != 1
			|| b - a < 1.e-6
			|| sscanf(argv[3], "%lf", &c) != 1
			|| sscanf(argv[4], "%lf", &d) != 1
			|| d - c < 1.e-6
			|| sscanf(argv[5], "%d", &n) != 1
			|| n <= 0
			|| sscanf(argv[6], "%d", &m) != 1
			|| m <= 0)
			return -1;
		else {
			fillMethod();
            fillfuncProxy();

			msg.setText("Internal function: sqrtSin(x)\n");
			msg.exec();
			return 0;
		}
	}
	else {
		msg.setText("Help:\na b c d n m\na, b, c, d - borders\nn, m - numbers of points\n");
		msg.exec();
		return -1;
	}
	return 0;
}

void Window::decr_n() {
    if (n > 10)
    {
        n = n -4;

		fillMethod();
		fillResidual();
        fillfuncProxy();
	}
    m_graph->axisY()->setRange(min_meth(), max_meth());
}

void Window::decr_m() {
    if (m > 10)
    {

        m = m - 4;

        fillMethod();
        fillResidual();
        fillfuncProxy();
    }
    m_graph->axisY()->setRange(min_meth(), max_meth());
}

void Window::incr_n()
{

    n = n +4;
    fillMethod();
    fillResidual();
    fillfuncProxy();


    m_graph->axisY()->setRange(min_meth(), max_meth());
}


void Window::incr_m()
{

    m = m +4 ;
    fillMethod();
    fillResidual();
    fillfuncProxy();

    m_graph->axisY()->setRange(min_meth(), max_meth());
}


void Window::decr_a_b()
{
        if (b - a > 15)
        {
            b -= 10;
            a += 10;
        }

	fillMethod();
	fillResidual();
    fillfuncProxy();
	m_graph->axisX()->setRange(a, b);
    m_graph->axisY()->setRange(min(min_meth(),30),max(max_meth(),30));
	m_graph->axisZ()->setRange(c, d);
}

void Window::decr_c_d()
{
    if (d-c > 15)
    {

        d -=10;
        c += 10;

    }

    fillMethod();
    fillResidual();
    fillfuncProxy();
    m_graph->axisX()->setRange(a, b);
    m_graph->axisY()->setRange(min(min_meth(),30),max(max_meth(),30));
    m_graph->axisZ()->setRange(c, d);
}

void Window::incr_a_b()
{

        b += 10;
        a -= 10;


        fillMethod();
        fillResidual();
        fillfuncProxy();
        m_graph->axisX()->setRange(a, b);
        m_graph->axisY()->setRange(min(min_meth(),30),max(max_meth(),30));
        m_graph->axisZ()->setRange(c, d);
}

void Window::incr_c_d()
{
    d += 10;
    c -= 10;


    fillMethod();
    fillResidual();
    fillfuncProxy();
    m_graph->axisX()->setRange(a, b);
    m_graph->axisY()->setRange(min(min_meth(),30),max(max_meth(),30));
    m_graph->axisZ()->setRange(c, d);
}


float Window::max_func() {
	float stepX = (b - a) / float(Average);
	float stepZ = (d - c) / float(Average);

	float max = f(a, c);

	for (int i = 0; i < Average; i++) {
		float _z = qMin(d, (i * stepZ + c));
		for (int j = 0; j < Average; j++) {
			float _x = qMin(b, (j * stepX + a));
			if (max < f(_x, _z))
				max = f(_x, _z);
		}
	}
	return max;
}

float Window::min_func() {
	float stepX = (b - a) / float(Average);
	float stepZ = (d - c) / float(Average);

	float min = f(a, c);

	for (int i = 0; i < Average; i++) {
		float _z = qMin(d, (i * stepZ + c));
		for (int j = 0; j < Average; j++) {
			float _x = qMin(b, (j * stepX + a));
			if (min > f(_x, _z))
				min = f(_x, _z);
		}
	}
	return min;
}

float Window::max_meth() {
	float stepX = (b - a) / float(Average);
	float stepZ = (d - c) / float(Average);

	float max = poly(a, c);

    for (int i = 0; i < Average; i++) {
		float _z = qMin(d, (i * stepZ + c));
		for (int j = 0; j < Average; j++) {
			float _x = qMin(b, (j * stepX + a));
			if (max < poly(_x, _z))
				max = poly(_x, _z);
		}
	}
	return max;
}

float Window::min_meth() {
	float stepX = (b - a) / float(Average);
	float stepZ = (d - c) / float(Average);

	float min = poly(a, c);

    for (int i = 0; i < Average; i++) {
		float _z = qMin(d, (i * stepZ + c));
		for (int j = 0; j < Average; j++) {
			float _x = qMin(b, (j * stepX + a));
			if (min > poly(_x, _z))
				min = poly(_x, _z);
		}
	}
	return min;
}

char* Window::text()
{
	char buf1[30], buf2[30];
	strcpy(buf1, "a = ");
	sprintf(buf2, "%.3f", a);
	strcat(buf1, buf2);
	return buf1;
}
