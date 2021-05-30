#include <qapplication.h>
#include <QOpenGLWidget>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMainWindow>
#include <QLabel>
#include "window.h"
int main(int argc, char** argv){
	QApplication app(argc, argv);
    QMainWindow *window = new QMainWindow;
    QMenuBar *tool_bar = new QMenuBar (window);
    myGLWidget *myglw = new myGLWidget;
    QAction *action;
    action = tool_bar->addAction ("Смена функции", myglw, SLOT (change_func()));
    action->setShortcut (QString ("0"));

    action = tool_bar->addAction ("Вид", myglw, SLOT (change_view()));
    action->setShortcut (QString ("1"));

    action = tool_bar->addAction ("Приблизить", myglw, SLOT (scale_up()));
    action->setShortcut (QString ("2"));

    action = tool_bar->addAction ("Отдалить", myglw, SLOT (scale_down()));
    action->setShortcut (QString ("3"));

    action = tool_bar->addAction("Влево", myglw, SLOT(left()));
    action->setShortcut(QString("Left"));

    action = tool_bar->addAction("Вправо", myglw, SLOT(right()));
    action->setShortcut(QString("Right"));

    action = tool_bar->addAction("Вверх", myglw, SLOT(up()));
    action->setShortcut(QString("Up"));

    action = tool_bar->addAction("Вниз", myglw, SLOT(down()));
    action->setShortcut(QString("Down"));

    action = tool_bar->addAction ("Добавить n", myglw, SLOT (points_up()));
    action->setShortcut (QString ("4"));

    action = tool_bar->addAction ("Убрать n", myglw, SLOT (points_down()));
    action->setShortcut (QString ("5"));

    action = tool_bar->addAction ("Добавить m", myglw, SLOT (points_up_m()));
    action->setShortcut (QString ("6"));

    action = tool_bar->addAction ("Убрать m", myglw, SLOT (points_down_m()));
    action->setShortcut (QString ("7"));

    action = tool_bar->addAction ("Выход", window, SLOT (close()));
    action->setShortcut (QString ("Ctrl+X"));

    window->setCentralWidget(myglw);
    window->setMenuBar (tool_bar);
    window->setWindowTitle ("Dunaev");
    window->show();
	return app.exec();
}
