#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMessageBox>
#include <QtWidgets>
#include <QtDataVisualization>
#include "window.h"

using namespace QtDataVisualization;

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	Q3DSurface *graph = new Q3DSurface();
	QWidget *container = QWidget::createWindowContainer(graph);

	QWidget *widget = new QWidget;
	QMenuBar *tool_1 = new QMenuBar(widget);
	QMenuBar *tool_2 = new QMenuBar(widget);
    QMenuBar *tool_3 = new QMenuBar(widget);
    QMenuBar *tool_4 = new QMenuBar(widget);
    QMenuBar *tool_5 = new QMenuBar(widget);
	QAction *action;
        QLabel *ALab = new QLabel(widget);
        ALab->setText(QStringLiteral("Mamba"));


    Window *graph_area = new Window(graph,ALab);

	if (graph_area->parse_command_line(argc, argv)) {
		QMessageBox::warning(0, "Wrong input arguments!",
			"Wrong input arguments!");
		return -1;
	}

	QSize screenSize = graph->screen()->size();
	container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
	container->setMaximumSize(screenSize);
	container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	container->setFocusPolicy(Qt::StrongFocus);

	QHBoxLayout *hLayout = new QHBoxLayout(widget);
	QVBoxLayout *vLayout = new QVBoxLayout();
	hLayout->addWidget(container, 1);
	hLayout->addLayout(vLayout);
	vLayout->setAlignment(Qt::AlignTop);

	QGroupBox *modelGroupBox = new QGroupBox(QStringLiteral("Choose:"));

	QRadioButton *sqrtSinModelRB = new QRadioButton(widget);
    sqrtSinModelRB->setText(QStringLiteral("Function"));
	sqrtSinModelRB->setChecked(false);

	QRadioButton *method = new QRadioButton(widget);
	method->setText(QStringLiteral("Method"));
	method->setChecked(false);

	QRadioButton *res = new QRadioButton(widget);
	res->setText(QStringLiteral("Residual"));
	res->setChecked(false);

	QVBoxLayout *modelVBox = new QVBoxLayout;
	modelVBox->addWidget(sqrtSinModelRB);
	modelVBox->addWidget(method); 
	modelVBox->addWidget(res);
	modelGroupBox->setLayout(modelVBox);

	QLinearGradient grGtoR(0, 0, 1, 100);
	grGtoR.setColorAt(1.0, Qt::darkGreen);
	grGtoR.setColorAt(0.5, Qt::yellow);
	grGtoR.setColorAt(0.2, Qt::red);
	grGtoR.setColorAt(0.0, Qt::darkRed);
	QPixmap pm(24, 100);
	QPainter pmp(&pm);
	pmp.setBrush(QBrush(grGtoR));
	pmp.drawRect(0, 0, 24, 100);
	QPushButton *gradientGtoRPB = new QPushButton(widget);
	gradientGtoRPB->setIcon(QIcon(pm));
	gradientGtoRPB->setIconSize(QSize(24, 100));

	QGroupBox *colorGroupBox = new QGroupBox(QStringLiteral("Custom gradient"));
	QHBoxLayout *colorHBox = new QHBoxLayout;
	colorHBox->addWidget(gradientGtoRPB);
	colorGroupBox->setLayout(colorHBox);


        action = tool_1->addAction("Increase n", graph_area, SLOT(incr_n()));
	action->setShortcut(QString("Ctrl+A"));

        action = tool_1->addAction("Increse m", graph_area, SLOT(incr_m()));
        action->setShortcut(QString("Ctrl+B"));

        action = tool_2->addAction("Decrease n", graph_area, SLOT(decr_n()));
        action->setShortcut(QString("Ctrl+C"));

        action = tool_2->addAction("Decrese m", graph_area, SLOT(decr_m()));
        action->setShortcut(QString("Ctrl+D"));

        action = tool_3->addAction("Increase a_b", graph_area, SLOT(incr_a_b()));
        action->setShortcut(QString("Ctrl+E"));

        action = tool_3->addAction("Decrease c_d", graph_area, SLOT(decr_a_b()));
        action->setShortcut(QString("Ctrl+F"));

        /*action = tool_4->addAction("Increase c_d", graph_area, SLOT(incr_c_d()));
        action->setShortcut(QString("Ctrl+E"));*/

        /*action = tool_4->addAction("Decrease c_d", graph_area, SLOT(decr_c_d()));
        action->setShortcut(QString("Ctrl+F"));*/


        action = tool_5->addAction("E&xit", widget, SLOT(close()));
	action->setShortcut(QString("Ctrl+X"));

	tool_1->setMaximumHeight(30);
	tool_2->setMaximumHeight(30);
        tool_3->setMaximumHeight(30);
        tool_4->setMaximumHeight(30);
        tool_5->setMaximumHeight(30);

	QGroupBox *labels= new QGroupBox(QStringLiteral("Values"));
	QLabel *val = new QLabel();
	val->setText(graph_area->text());


	vLayout->addWidget(modelGroupBox);
        vLayout->addWidget(ALab);
	vLayout->addWidget(tool_1);
	vLayout->addWidget(tool_2);
        vLayout->addWidget(tool_3);
       // vLayout->addWidget(tool_4);
        vLayout->addWidget(tool_5);

        vLayout->addWidget(val);
	vLayout->addWidget(colorGroupBox);

	QObject::connect(gradientGtoRPB, &QPushButton::pressed,
		graph_area, &Window::setGreenToRedGradient);
	QObject::connect(sqrtSinModelRB, &QRadioButton::toggled,
                graph_area, &Window::enablefuncModel);
    //QObject::connect(sqrtSinModelRB, &QRadioButton::toggled,
     //   ALab, &QLabel::setText(QStringLiteral("Method")));
	QObject::connect(method, &QRadioButton::toggled,
		graph_area, &Window::enableMethod);
	QObject::connect(res, &QRadioButton::toggled,
		graph_area, &Window::enableResidual);

	sqrtSinModelRB->setChecked(true);
	widget->setWindowTitle(QStringLiteral("Interpolation 3D"));
	widget->show();

	return app.exec();
}
