#ifndef _my_widget
#define _my_widget
#include <QOpenGLWidget>
#include <QKeyEvent>
#include <qnamespace.h>
void MultiplicateMatrices(double A[4][4], double B[4][4], double C[4][4]);
class myGLWidget:public QOpenGLWidget{
    Q_OBJECT
public slots:
      void change_func();
      void change_view();
      void left();
      void right();
      void up();
      void down();
      void scale_up();
      void scale_down();
      void points_up();
      void points_down();
      void points_up_m();
      void points_down_m();
protected:
      void ProjectionMatrix();
      void CameraDefault();
      int parse_command_line (int argc, char *argv[]);
      double d_fd_x(double x, double y);
      double d_fd_y(double x, double y);
      double dd_fd_xd_y(double x, double y);
      double d_fd_x_2(double x, double y, double d_x, double d_y);
      double d_fd_y_2(double x, double y, double d_x, double d_y);
      double dd_fd_xd_y_2(double x, double y, double d_x, double d_y);
      void MatrixF(double x_i, double x_i_1, double y_i, double y_i_1, double F[4][4]);
      void MatrixF_2(double x_i, double x_i_1, double y_i, double y_i_1, double F[4][4]);
      void InverseMatrix(double h, double matrix[4][4]);
      void TransposeMatrix(double h, double matrix[4][4]);
      double calculation(double x, double y, double F[4][4]);
      virtual void paintGL();
      virtual void initializeGL();
      virtual void resizeGL(int nWidth, int nHeight);
      virtual void drawAxis();
private:
      double (*FUNCTION) (double, double);
      const char *f_name;
      float ANGLEH, ANGLEV, CAMERA_POS, ASPECT;
      int view_id = 0, func_id = 0, n = 4, m = 4;
      double x_min = -1, x_max = 1, y_min = -1, y_max = 1, z_max = 1, z_min = 1, diff = 0;
public:
      QSize minimumSizeHint () const;
      QSize sizeHint () const;
      myGLWidget();
};

#endif
