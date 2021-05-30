#include "window.h"
#include <stdio.h>
#include <math.h>
#include <QPainter>
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define SGN(a) ((a) > (0) ? (1) : (-1))
void myGLWidget::InverseMatrix(double h, double matrix[4][4]){
    double h_1 = 1 / h, h_2 = 1 / (h * h), h_3 = h_2 / h;
    matrix[0][0] = 1;       matrix[0][1] = 0;       matrix[0][2] = 0;       matrix[0][3] = 0;
    matrix[1][0] = 0;       matrix[1][1] = 1;       matrix[1][2] = 0;       matrix[1][3] = 0;
    matrix[2][0] = -3 * h_2;matrix[2][1] = -2 * h_1;matrix[2][2] = 3 * h_2; matrix[2][3] = -h_1;
    matrix[3][0] = 2 * h_3; matrix[3][1] = h_2;     matrix[3][2] = -2 * h_3;matrix[3][3] = h_2;
}
void myGLWidget::TransposeMatrix(double h, double matrix[4][4]){
    double h_1 = 1 / h, h_2 = 1 / (h * h), h_3 = h_2 / h;
    matrix[0][0] = 1;       matrix[1][0] = 0;       matrix[2][0] = 0;       matrix[3][0] = 0;
    matrix[0][1] = 0;       matrix[1][1] = 1;       matrix[2][1] = 0;       matrix[3][1] = 0;
    matrix[0][2] = -3 * h_2;matrix[1][2] = -2 * h_1;matrix[2][2] = 3 * h_2; matrix[3][2] = -h_1;
    matrix[0][3] = 2 * h_3; matrix[1][3] = h_2;     matrix[2][3] = -2 * h_3;matrix[3][3] = h_2;
}
void MultiplicateMatrices(double X[4][4], double Y[4][4], double Z[4][4]){
    int i, j, k;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++){
            Z[i][j] = 0;
            for (k = 0; k < 4; k++)
                Z[i][j] += X[i][k] * Y[k][j];
        }
}
void myGLWidget::MatrixF(double x_i, double x_i_1, double y_i, double y_i_1, double matrix[4][4]){
    matrix[0][0] = FUNCTION(x_i, y_i);  matrix[0][1] = d_fd_y(x_i, y_i);      matrix[0][2] = FUNCTION(x_i, y_i_1);  matrix[0][3] = d_fd_y(x_i, y_i_1);
    matrix[1][0] = d_fd_x(x_i, y_i);    matrix[1][1] = dd_fd_xd_y(x_i, y_i);  matrix[1][2] = d_fd_x(x_i, y_i_1);    matrix[1][3] = dd_fd_xd_y(x_i, y_i_1);
    matrix[2][0] = FUNCTION(x_i_1, y_i);matrix[2][1] = d_fd_y(x_i_1, y_i);    matrix[2][2] = FUNCTION(x_i_1, y_i_1);matrix[2][3] = d_fd_y(x_i_1, y_i_1);
    matrix[3][0] = d_fd_x(x_i_1, y_i);  matrix[3][1] = dd_fd_xd_y(x_i_1, y_i);matrix[3][2] = d_fd_x(x_i_1, y_i_1);  matrix[3][3] = dd_fd_xd_y(x_i_1, y_i_1);
}
double myGLWidget::d_fd_x(double x, double y){
    return 0.5 * (FUNCTION(x + 1e-10, y) - FUNCTION(x - 1e-10, y))/1e-10;
}
double myGLWidget::d_fd_y(double x, double y){
    return 0.5 * (FUNCTION(x, y + 1e-10) - FUNCTION(x, y - 1e-10))/1e-10;
}
double myGLWidget::dd_fd_xd_y(double x, double y){
    switch(func_id){
    case 0: return sin(x)/(-cos(y));
    case 1: return 0;
    case 2: return 0;
    case 3: return 2/((x+y)*(x+y)*(x+y));
    }
    return 0;
}
double myGLWidget::d_fd_x_2(double x, double y, double d_x, double d_y){
    double f_2, f_3;
    d_y += 0;
    f_2 = (FUNCTION(x, y) - FUNCTION(x - d_x, y)) / d_x;
    f_3 = (FUNCTION(x + d_x, y) - FUNCTION(x, y)) / d_x;
    return ((SGN(f_3) != SGN(f_2)) ? 0 : SGN(f_2)) * fmin(fabs(f_3), fabs(f_2));
}
double myGLWidget::d_fd_y_2(double x, double y, double d_x, double d_y){
    double f_2, f_3;
    d_x += 0;
    f_2 = (FUNCTION(x, y) - FUNCTION(x, y - d_y)) / d_y;
    f_3 = (FUNCTION(x, y + d_y) - FUNCTION(x, y)) / d_y;
    return ((SGN(f_3) != SGN(f_2)) ? 0 : SGN(f_2)) * fmin(fabs(f_3), fabs(f_2));
}
double myGLWidget::dd_fd_xd_y_2(double x, double y, double d_x, double d_y){
    return (FUNCTION(x + d_x, y + d_y) -  FUNCTION(x, y + d_y) - FUNCTION(x + d_x, y) + FUNCTION(x, y)) / d_x / d_y;
}
void myGLWidget::MatrixF_2(double x_i, double x_i_1, double y_i, double y_i_1, double matrix[4][4]){
    matrix[0][0] = FUNCTION(x_i, y_i);                                  matrix[0][1] = d_fd_y_2(x_i, y_i, x_i_1 - x_i, y_i_1 - y_i);        matrix[0][2] = FUNCTION(x_i, y_i_1);                                matrix[0][3] = d_fd_y_2(x_i, y_i_1, x_i_1 - x_i, y_i_1 - y_i);
    matrix[1][0] = d_fd_x_2(x_i, y_i, x_i_1 - x_i, y_i_1 - y_i);    matrix[1][1] = dd_fd_xd_y_2(x_i, y_i, x_i_1 - x_i, y_i_1 - y_i);    matrix[1][2] = d_fd_x_2(x_i, y_i_1, x_i_1 - x_i, y_i_1 - y_i);  matrix[1][3] = dd_fd_xd_y_2(x_i, y_i_1, x_i_1 - x_i, y_i_1 - y_i);
    matrix[2][0] = FUNCTION(x_i_1, y_i);                                matrix[2][1] = d_fd_y_2(x_i_1, y_i, x_i_1 - x_i, y_i_1 - y_i);      matrix[2][2] = FUNCTION(x_i_1, y_i_1);                              matrix[2][3] = d_fd_y_2(x_i_1, y_i_1, x_i_1 - x_i, y_i_1 - y_i);
    matrix[3][0] = d_fd_x_2(x_i_1, y_i, x_i_1 - x_i, y_i_1 - y_i);  matrix[3][1] = dd_fd_xd_y_2(x_i_1, y_i, x_i_1 - x_i, y_i_1 - y_i);  matrix[3][2] = d_fd_x_2(x_i_1, y_i_1, x_i_1 - x_i, y_i_1 - y_i);matrix[3][3] = dd_fd_xd_y_2(x_i_1, y_i_1, x_i_1 - x_i, y_i_1 - y_i);
}
double myGLWidget::calculation(double x, double y, double matrix[4][4]){
    double a = 1, b, s = 0;
    int i, j;
    for(i = 0; i < 4; i++){
        b = 1;
        for(j = 0; j < 4; j++){
            s += matrix[i][j] * a * b;
            b *= y;
        }
        a *= x;
    }
    return s;
}

