#include "window.h"
#include <stdio.h>
#include <math.h>
#include <QPainter>
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

double f_0(double x, double y){
    return 1;
}
double f_1(double x, double y){
    return x;
}
double f_2(double x, double y){
    return  y;
}
double f_3 (double x, double y){
    return (x+y);
}
double f_4 (double x, double y){
    return sqrt(x*x+y*y);
}
double f_5 (double x, double y){
    return (x*x+y*y);
}
double f_6 (double x, double y){
    return exp(x*x-y*y);
}
double f_7 (double x, double y){
    return 1/(25*(x*x+y*y)+1);
}
myGLWidget::myGLWidget(){
    FUNCTION = f_0;
    func_id = 0;
    f_name = "sin(x)/cos(y)";
}
int myGLWidget::parse_command_line (int argc, char *argv[]){
    if (argc == 1)
        return 0;

    if (argc == 2)
        return -1;

    if (   sscanf (argv[1], "%lf", &x_min) != 1
           || sscanf (argv[2], "%lf", &x_max) != 1
           || x_max - x_min < 1.e-6
           || sscanf (argv[3], "%lf", &y_min) != 1
           || sscanf (argv[4], "%lf", &y_max) != 1
           || y_max - y_min < 1.e-6
           || (argc > 5 && sscanf (argv[3], "%d", &n) != 1)
           || (argc > 6 && sscanf (argv[4], "%d", &m) != 1)
           || n <= 0
           || m <= 0)
        return -2;

    return 0;
}

QSize myGLWidget::minimumSizeHint () const{
    return QSize (100, 100);
}

QSize myGLWidget::sizeHint () const{
    return QSize (1000, 1000);
}

void myGLWidget::initializeGL(){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    CameraDefault();
}

///change current function for drawing
void myGLWidget::change_func(){
    func_id = (func_id + 1) % 8;
    switch (func_id){
    case 0:
        f_name = "1";
        FUNCTION = f_0;
        break;
    case 1:
        f_name = "x";
        FUNCTION = f_1;
        break;
    case 2:
        f_name = "y";
        FUNCTION = f_2;
        break;
    case 3:
        f_name = "x+y";
        FUNCTION = f_3;
        break;
    case 4:
        f_name = "sqrt(x*x+y*y)";
        FUNCTION = f_4;
        break;
    case 5:
        f_name = "x*x+y*y";
        FUNCTION = f_5;
        break;
    case 6:
        f_name = "exp(x*x-y*y)";
        FUNCTION = f_6;
        break;
    case 7:
        f_name = "1/(25(x*x+y*y)+1)";
        FUNCTION = f_7;
        break;
    }
    update();
}
void myGLWidget::change_view(){
    view_id = (view_id + 1) % 3;
    update();
}
void myGLWidget::left(){
    ANGLEH -= 5;
    update();
}
void myGLWidget::right(){
    ANGLEH += 5;
    update();
}
void myGLWidget::up(){
    ANGLEV = MIN(ANGLEV + 5, 80);
    update();
}
void myGLWidget::down(){
    ANGLEV = MAX(ANGLEV - 5, -80);
    update();
}
void myGLWidget::points_up(){
        n *= 1.5;
    update();
}
void myGLWidget::points_down(){
    if (n > 2)
        n /= 1.5;
    update();
}
void myGLWidget::points_up_m(){
    m *= 1.5;
    update();
}
void myGLWidget::points_down_m(){
   if (m > 2)
       m /= 1.5;
   update();
}
void myGLWidget::scale_up(){
    CAMERA_POS = MAX(CAMERA_POS - 0.1f, 6);
    update();
}
void myGLWidget::scale_down(){
    CAMERA_POS += 0.1f;
    update();
}

void myGLWidget::paintGL(){\
    glClear(GL_COLOR_BUFFER_BIT);
    QPainter painter(this);
    painter.setPen (Qt::black);
    painter.setFont(QFont("Helvetica", 14));
    painter.drawText (5, 15, QString("Функция : "));
    painter.drawText (100, 15, QString(f_name));
    if(view_id == 0)
        painter.drawText (5, 45, QString("Только исходная функция"));
    if(view_id == 1)
        painter.drawText (5, 45, QString("Исходная функция и приближение"));
    if(view_id == 2)
         painter.drawText (5, 45, QString("Погрешность : %2").arg(diff));
    painter.drawText (5, 75, QString("n : %1").arg(n));
    painter.drawText (5, 105, QString("m : %1").arg(m));
    painter.end();
    ProjectionMatrix();
    glEnable(GL_DEPTH_TEST);
    glBegin(GL_QUADS);

    if(view_id == 0){ //рисуем только исходную функцию
        int	i, j, x_n = 300, y_m = 300;
        double z1, delta;
        z_min = FUNCTION(x_min, y_min);
        z_max = FUNCTION(x_max, y_max);
        for(double x1 = x_min; x1 < x_max + 1e-6; x1 += 1e-2){
            for(double y1 = y_min; y1 < y_max + 1e-6; y1 += 1e-2){
                z1 = FUNCTION(x1, y1);
                z_max = MAX(z1, z_max);
                z_min = MIN(z1, z_min);
            }
        }
        delta = 0.01 * (z_max - z_min);
        z_min -= delta;
        z_max += delta;

        glColor3d(0.0,0.0,0.0);
        for (i = 0; i <= x_n-2; i++)
            for (j = 0; j <= y_m-2; j++) {
                double	x, y, z;
                glColor3d(0.5 * (x_n - i) / x_n, 0.5 * j / y_m, 0.2);
                x = (x_max - x_min) * i / (x_n - 1) + x_min;
                y = (y_max - y_min) * j / (y_m - 1) + y_min;
                z = FUNCTION(x, y);
                glVertex3d(x, y, z);
                x = (x_max - x_min) * (i + 1) / (x_n - 1) + x_min;
                y = (y_max - y_min) * j / (y_m - 1) + y_min;
                z = FUNCTION(x, y);
                glVertex3d(x, y, z);
                x = (x_max - x_min) * (i + 1) / (x_n - 1) + x_min;
                y = (y_max - y_min) * (j + 1) / (y_m - 1) + y_min;
                z = FUNCTION(x, y);
                glVertex3d(x, y, z);
                x = (x_max - x_min) * i / (x_n - 1) + x_min;
                y = (y_max - y_min) * (j + 1) / (y_m - 1) + y_min;
                z = FUNCTION(x, y);
                glVertex3d(x, y, z);
            }
        drawAxis();
    }
    if(view_id == 1){       //рисуем исходную вместе с приближением
        int  i, j, k, l, N, M, x_n = 300, y_m = 300;
        double d_x, d_y, dd_x, dd_y, x, y, x_i, y_j, A[4][4], B[4][4], F[4][4], C[4][4];

        double z1, delta;
        z_min = FUNCTION(x_min, y_min);
        z_max = FUNCTION(x_max, y_max);
        for(double x1 = x_min; x1 < x_max + 1e-6; x1 += 1e-2){
            for(double y1 = y_min; y1 < y_max + 1e-6; y1 += 1e-2){
                z1 = FUNCTION(x1, y1);
                z_max = MAX(z1, z_max);
                z_min = MIN(z1, z_min);
            }
        }
        delta = 0.01 * (z_max - z_min);
        z_min -= delta;
        z_max += delta;

        //рисуем исходную функцию
        glColor3d(0.0,0.0,0.0);
        for (i = 0; i < x_n - 1; i++)
            for (j = 0; j < y_m - 1; j++) {
                double	x, y, z;
                glColor3d(0.5 * (x_n - i) / x_n, 0.5 * j / y_m, 0.5);
                x = (x_max - x_min) * i / (x_n - 1) + x_min;
                y = (y_max - y_min) * j / (y_m - 1) + y_min;
                z = FUNCTION(x, y);
                glVertex3d(x, y, z);
                x = (x_max - x_min) * (i + 1) / (x_n - 1) + x_min;
                y = (y_max - y_min) * j / (y_m - 1) + y_min;
                z = FUNCTION(x, y);
                glVertex3d(x, y, z);
                x = (x_max - x_min) * (i + 1) / (x_n - 1) + x_min;
                y = (y_max - y_min) * (j + 1) / (y_m - 1) + y_min;
                z = FUNCTION(x, y);
                glVertex3d(x, y, z);
                x = (x_max - x_min) * i / (x_n - 1) + x_min;
                y = (y_max - y_min) * (j + 1) / (y_m - 1) + y_min;
                z = FUNCTION(x, y);
                glVertex3d(x, y, z);
            }
        //рисуем приближение функции
        glColor3d(1.0,1.0,1.0);
        d_x = (x_max - x_min) / n;
        d_y = (y_max - y_min) / m;
        InverseMatrix(d_x, A);
        TransposeMatrix(d_y, B);
        for (i = 0; i < n; i++){
            x_i = x_min + i * d_x;
            for (j = 0; j < m; j++){
                y_j = y_min + j * d_y;
                MatrixF(x_i, x_i + d_x, y_j, y_j + d_y, F);
                MultiplicateMatrices(A, F, C);
                MultiplicateMatrices(C, B, F);
                N = MAX(n, 5);
                M = MAX(m, 5);
                dd_x = d_x / N;
                dd_y = d_y / M;
                for(k = 0; k < N; k++){
                    for(l = 0; l < M; l++){
                        x = x_i + k * dd_x;
                        y = y_j + l * dd_y;
                        glColor3d(1.0 * (x - x_min) / (x_max - x_min), 0.5 * (y - y_min) / (y_max - y_min),  0.1);
                        glVertex3d(x, y, calculation(k * dd_x, l * dd_y, F));
                        x = x_i + (k + 1) * dd_x;
                        y = y_j + l * dd_y;
                        glVertex3d(x, y, calculation((k + 1) * dd_x, l * dd_y, F));

                        x = x_i + (k + 1) * dd_x;
                        y = y_j + (l + 1) * dd_y;
                        glVertex3d(x, y, calculation((k + 1) * dd_x, (l + 1) * dd_y, F));

                        x = x_i + k * dd_x;
                        y = y_j + (l + 1) * dd_y;
                        glVertex3d(x, y, calculation(k * dd_x, (l + 1) * dd_y, F));

                    }
                }
            }
        }
        drawAxis();
    }

    if((view_id == 2)&&(n <= 1000)&&(m <= 1000)){
        int i, j, N, M;
        double d_x, d_y, dd_x, dd_y, x, y, z, x_i, y_j, z1, f_max = 0.0, f_min = 0.0;
        double A[4][4], B[4][4], F[4][4], C[4][4];
        x = 0;
        y = 0;
        d_x = (x_max - x_min) / n;
        d_y = (y_max - y_min) / m;
        InverseMatrix(d_x, A);
        TransposeMatrix(d_y, B);
        for (i = 0; i < n; i++){
            x_i = x_min + i * d_x;
            for (j = 0; j < m; j++){
                y_j = y_min + j * d_y;
                MatrixF(x_i, x_i + d_x, y_j, y_j + d_y, F);
                MultiplicateMatrices(A,F,C);
                MultiplicateMatrices(C,B,F);
                N = MAX(n, 5);
                M = MAX(m, 5);
                dd_x = d_x / N;
                dd_y = d_y / M;
                //разность функций
                for (int i = 0; i < N; i++)
                    for (int j = 0; j < M; j++){
                        glColor3d(1.0 * (x - x_min) / (x_max - x_min), 0.5 * (y - y_min) / (y_max - y_min),  0.1);
                        x = (x_i + i * dd_x);
                        y = (y_j + j * dd_y);
                        z = FUNCTION((x_i + i * dd_x), (y_j + j * dd_y));
                        z1 = z - calculation(i * dd_x, j * dd_y, F);
                        f_max = MAX(z1, f_max);
                        f_min = MIN(z1, f_min);
                        glVertex3d(x, y, z - calculation(i * dd_x, j * dd_y, F));
                        x = (x_i + (i + 1) * dd_x);
                        y = (y_j + j * dd_y);
                        z = FUNCTION((x_i + (i + 1) * dd_x), (y_j + j * dd_y));
                        z1 = fabs(z - calculation((i + 1) * dd_x, j * dd_y, F));
                        f_max = MAX(z1, f_max);
                        f_min = MIN(z1, f_min);
                        glVertex3d(x, y, z - calculation((i + 1) * dd_x, j * dd_y, F));
                        x = (x_i + (i + 1) * dd_x);
                        y = (y_j + (j + 1) * dd_y);
                        z = FUNCTION((x_i + (i + 1) * dd_x), (y_j + (j + 1) * dd_y));
                        z1 = fabs(z - calculation((i + 1) * dd_x, (j + 1) * dd_y, F));
                        f_max = MAX(z1, f_max);
                        f_min = MIN(z1, f_min);
                        glVertex3d(x, y, z - calculation((i + 1) * dd_x, (j + 1) * dd_y, F));
                        x = (x_i + i * dd_x);
                        y = (y_j + (j + 1) * dd_y);
                        z = FUNCTION((x_i + i * dd_x), (y_j + (j + 1) * dd_y));
                        z1 = fabs(z - calculation(i * dd_x, (j + 1) * dd_y, F));
                        f_max = MAX(z1, f_max);
                        f_min = MIN(z1, f_min);
                        glVertex3d(x, y, z - calculation(i * dd_x, (j + 1) * dd_y, F));
                    }
                }
             }

            if(fabs(f_max)>fabs(f_min)) diff = fabs(f_max);
            else diff = fabs(f_min);

            if(isnan(diff))
                diff = 1e-9;

        double delta;
        z_min = FUNCTION(x_min, y_min);
        z_max = FUNCTION(x_max, y_max);
        for(double x1 = x_min; x1 < x_max + 1e-6; x1 += 1e-2){
            for(double y1 = y_min; y1 < y_max + 1e-6; y1 += 1e-2){
                z1 = FUNCTION(x1, y1);
                z_max = MAX(z1, z_max);
                z_min = MIN(z1, z_min);
            }
        }

        delta = 0.01 * (z_max - z_min);
        z_min -= delta;
        z_max += delta;

        drawAxis();

          }
    glEnd();
    glDisable(GL_DEPTH_TEST);
}

void myGLWidget::resizeGL(int nWidth, int nHeight){
    glViewport(0, 0, nWidth, nHeight);
    ASPECT = nWidth / nHeight;
    update();
}
void myGLWidget::ProjectionMatrix(){
    GLfloat VIEW[16] = {0}, PR[16] = {0}, TMP[16] = {0}, CAM_X, cam_y, cam_z;
    static GLfloat	near = 5, top = 2, bottom, left, right;
    bottom = -top;
    right = top * ASPECT;
    left = -right;
    PR[0] = 2 * near / (right - left);
    PR[2] = (right + left) / (right - left);
    PR[5] = 2 * near / (top - bottom);
    PR[6] = (top + bottom) / (top - bottom);
    PR[10] = - 1;
    PR[11] = - 2 * near;
    PR[14] = -1;
    CAM_X = 0;
    cam_y = 0;
    cam_z = CAMERA_POS;
    VIEW[0] = 1;
    VIEW[6] = -1;
    VIEW[9] = 1;
    VIEW[15] = 1;
    VIEW[12] = -CAM_X;
    VIEW[13] = -cam_y;
    VIEW[14] = -cam_z;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glRotatef(ANGLEH, 0, 0, 1);
    glGetFloatv(GL_PROJECTION_MATRIX, TMP);
    glLoadTransposeMatrixf(PR);
    glMultMatrixf(VIEW);
    glRotatef(ANGLEH, 0, 0, 1);
    glRotatef(ANGLEV, TMP[0], TMP[4], TMP[8]);
}
void myGLWidget::CameraDefault(){
    CAMERA_POS = 7;
    ANGLEH = 45;
    ANGLEV = 30;
    ASPECT = width() / height();
}

void myGLWidget::drawAxis(){
    glLineWidth(4.0);

    glColor4d(1.0, 0.0, 0.0, 1.0);
    glBegin(GL_LINES);
        glVertex3d(x_min - 1.0, 0.0, 0.0);
        glVertex3d(x_max + 1.0, 0.0, 0.0);
    glEnd();

    glColor4d(1.0, 0.0, 0.0, 1.0);
    glBegin(GL_LINES);
        glVertex3d(x_min - 1.0, 0.0, 0.0);
        glVertex3d(x_max + 1.0, 0.0, 0.0);
    glEnd();

    glColor4d(0.0, 1.0, 0.0, 1.0);
    glBegin(GL_LINES);
        glVertex3d(0.0, y_min-1.0, 0.0);
        glVertex3d(0.0, y_max+1.0, 0.0);
    glEnd();

    glColor4d(0.0, 0.0, 1.0, 1.0);
    glBegin(GL_LINES);
        glVertex3d(0.0, 0.0, z_max+1.0);
        glVertex3d(0.0, 0.0, z_min-1.0);
    glEnd();

}
