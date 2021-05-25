#include "window.h"

void Window::method()
{
    double Gxleft,Gxright,Gyleft,Gyright;
    double *F, *Gx, *Gy, *Fx, *Fy, *Fxy,*Fxleft,*Fxright;
    Gx = new double[n*n];
    Gy = new double[m*m];
    Fx = new double[n*m];
    Fxleft= new double[n];
    Fxright= new double[n];
    Fy = new double[n*m];
    Fxy= new double[n*m];
    F= new double[16 * n*m];

    int i, j, k, l;

    for (i = 0; i < n*n; i++)
        Gx[i] = 0;

    for (j = 0; j < m*m; j++)
        Gy[j] = 0;

    for (i = 0; i < m*n; i++)
    {
        Fx[i] = 0;
        Fy[i] = 0;
        Fxy[i] = 0;
    }
    for(i=0;i<n;i++)
    {
        Fxleft[i]=0;
        Fxright[i]=0;
    }



    i=0;
    Gxleft = -(x[i + 1] - x[i]) / ((x[i + 1] - xleft) * (x[i] - xleft));
    Gx[i*n + i] = (x[i + 1] - x[i]) / ((x[i + 1] - xleft) * (x[i] - xleft)) - (x[i] - xleft) / ((x[i + 1] - xleft) * (x[i + 1] - x[i]));
    Gx[i*n + i + 1] = (x[i] - xleft) / ((x[i + 1] - xleft) * (x[i + 1] - x[i]));
    for (i = 1; i < n - 1; i++)
    {
        Gx[i*n + i - 1] = -(x[i + 1] - x[i]) / ((x[i + 1] - x[i - 1]) * (x[i] - x[i - 1]));
        Gx[i*n + i] = (x[i + 1] - x[i]) / ((x[i + 1] - x[i - 1]) * (x[i] - x[i - 1])) - (x[i] - x[i - 1]) / ((x[i + 1] - x[i - 1]) * (x[i + 1] - x[i]));
        Gx[i*n + i + 1] = (x[i] - x[i - 1]) / ((x[i + 1] - x[i - 1]) * (x[i + 1] - x[i]));
    }
    i=n-1;
    Gx[i*n + i - 1] = -(xright - x[i]) / ((xright - x[i - 1]) * (x[i] - x[i - 1]));
    Gx[i*n + i] = (xright - x[i]) / ((xright - x[i - 1]) * (x[i] - x[i - 1])) - (x[i] - x[i - 1]) / ((xright - x[i - 1]) * (xright - x[i]));
    Gxright = (x[i] - x[i - 1]) / ((xright - x[i - 1]) * (xright - x[i]));




    j=0;
    Gyleft = -(y[j + 1] - y[j]) / ((y[j + 1] - yleft) * (y[j] - yleft));
    Gy[j*m + j] = (y[j + 1] - y[j]) / ((y[j + 1] - yleft) * (y[j] - yleft)) - (y[j] - yleft) / ((y[j + 1] - yleft) * (y[j + 1] - y[j]));
    Gy[j*m + j + 1] = (y[j] - yleft) / ((y[j + 1] - yleft) * (y[j + 1] - y[j]));
    for (j = 1; j < m - 1; j++)
    {
        Gy[j*m + j - 1] = -(y[j + 1] - y[j]) / ((y[j + 1] - y[j - 1]) * (y[j] - y[j - 1]));
        Gy[j*m + j] = (y[j + 1] - y[j]) / ((y[j + 1] - y[j - 1]) * (y[j] - y[j - 1])) - (y[j] - y[j - 1]) / ((y[j + 1] - y[j - 1]) * (y[j + 1] - y[j]));
        Gy[j*m + j + 1] = (y[j] - y[j - 1]) / ((y[j + 1] - y[j - 1]) * (y[j + 1] - y[j]));
    }
    j=m-1;
    Gy[j*m + j - 1] = -(yright - y[j]) / ((yright - y[j - 1]) * (y[j] - y[j - 1]));
    Gy[j*m + j] = (yright - y[j]) / ((yright - y[j - 1]) * (y[j] - y[j - 1])) - (y[j] - y[j - 1]) / ((yright - y[j - 1]) * (yright - y[j]));
    Gyright = (y[j] - y[j - 1]) / ((yright - y[j - 1]) * (yright - y[j]));


    i=0;
    for (j = 0; j < m; j++)
    {
            Fx[i*m + j] +=Gxleft * f(xleft, y[j]);
            for (k = 0; k < n; k++)
                Fx[i*m + j] += Gx[i*n + k] * f(x[k], y[j]);


    }

    for (i = 1; i < n-1; i++)
        for (j = 0; j < m; j++)
            for (k = 0; k < n; k++)
                Fx[i*m + j] += Gx[i*n + k] * f(x[k], y[j]);


    i=n-1;
    for (j = 0; j < m; j++)
    {
            for (k = 0; k < n; k++)
                Fx[i*m + j] += Gx[i*n + k] * f(x[k], y[j]);

            Fx[i*m + j] +=Gxright * f(xright, y[j]);
    }


    j=0;
    for (i = 0; i < n; i++)
    {
        Fy[i*m + j] += f(x[i], yleft)*Gyleft;
        for (k = 0; k < m; k++)
            Fy[i*m + j] += f(x[i], y[k])*Gy[j*m + k];

    }


    for (j = 1; j < m-1; j++)
     for (i = 0; i < n; i++)
       for (k = 0; k < m; k++)
         Fy[i*m + j] += f(x[i], y[k])*Gy[j*m + k];


    j=m-1;
    for (i = 0; i < n; i++)
    {
        for (k = 0; k < m; k++)
         Fy[i*m + j] += f(x[i], y[k])*Gy[j*m + k];

        Fy[i*m + j] += f(x[i], yright)*Gyright;

    }






        i=0;
        Fxleft[i] +=Gxleft * f(xleft, yleft);
        for (k = 0; k < n; k++)
        {
                Fxleft[i] += Gx[i*n + k] * f(x[k], yleft);
        }

        for (i = 1; i < n-1; i++)
            for (k = 0; k < n; k++)
                Fxleft[i] += Gx[i*n + k] * f(x[k], yleft);


        i=n-1;
        for (k = 0; k < n; k++)
            Fxleft[i] += Gx[i*n + k] * f(x[k], yleft);
        Fxleft[i] += Gxright * f(xright, yleft);



        i=0;
        Fxright[i] +=Gxleft * f(xleft, yright);
        for (k = 0; k < n; k++)
            Fxright[i] += Gx[i*n + k] * f(x[k], yright);


        for (i = 1; i < n-1; i++)
         for (k = 0; k < n; k++)
            Fxright[i] += Gx[i*n + k] * f(x[k], yright);


        i=n-1;
        for (k = 0; k < n; k++)
            Fxright[i] += Gx[i*n + k] * f(x[k], yright);

        Fxright[i] += Gxright * f(xright, yright);



    for (i = 0; i < n; i++)
    {
            j=0;
            Fxy[i*m + j] += Fxleft[i] * Gyleft;
            for (k = 0; k < m; k++)
                Fxy[i*m + j] += Fx[i*m + k] * Gy[j*m + k];

            for (j = 1; j < m-1; j++)
                for (k = 0; k < m; k++)
                    Fxy[i*m + j] += Fx[i*m + k] * Gy[j*m + k];


            j=m-1;
            for (k = 0; k < m; k++)
                Fxy[i*m + j] += Fx[i*m + k] * Gy[j*m + k];

            Fxy[i*m + j] += Fxright[i] * Gyright;
    }

    for (i = 0; i < n ; i++)
    {
        for (j = 0; j < m ; j++)
        {
            for (k = 0; k < 4; k++)
            {
                for (l = 0; l < 4; l++)
                {
                    if ((k == 0) && (l == 0))
                    {
                        F[i*m * 16 + j * 16 + k * 4 + l] = f(x[i], y[j]);
                    }
                    else if ((k == 0) && (l == 2))
                    {
                        F[i*m * 16 + j * 16 + k * 4 + l] = f(x[i], y[j + 1]);
                    }
                    else if ((k == 2) && (l == 0))
                    {
                        F[i*m * 16 + j * 16 + k * 4 + l] = f(x[i + 1], y[j]);
                    }
                    else if ((k == 2) && (l == 2))
                    {
                        F[i*m * 16 + j * 16 + k * 4 + l] = f(x[i + 1], y[j + 1]);
                    }
                    else if ((k == 1) && (l == 0))
                    {
                        F[i*m * 16 + j * 16 + k * 4 + l] = Fx[i*m + j];
                    }
                    else if ((k == 1) && (l == 2))
                    {
                        F[i*m * 16 + j * 16 + k * 4 + l] = Fx[i*m + j + 1];
                    }
                    else if ((k == 3) && (l == 0))
                    {
                        F[i*m * 16 + j * 16 + k * 4 + l] = Fx[(i + 1)*m + j];
                    }
                    else if ((k == 3) && (l == 2))
                    {
                        F[i*m * 16 + j * 16 + k * 4 + l] = Fx[(i + 1)*m + j + 1];
                    }
                    else if ((k == 0) && (l == 1))
                    {
                        F[i*m * 16 + j * 16 + k * 4 + l] = Fy[i*m + j];
                    }
                    else if ((k == 0) && (l == 3))
                    {
                        F[i*m * 16 + j * 16 + k * 4 + l] = Fy[i*m + j + 1];
                    }
                    else if ((k == 2) && (l == 1))
                    {
                        F[i*m * 16 + j * 16 + k * 4 + l] = Fy[(i + 1)*m + j];
                    }
                    else if ((k == 2) && (l == 3))
                    {
                        F[i*m * 16 + j * 16 + k * 4 + l] = Fy[(i + 1)*m + j + 1];
                    }
                    else if ((k == 1) && (l == 1))
                    {
                        F[i*m * 16 + j * 16 + k * 4 + l] = Fxy[i*m + j];
                    }
                    else if ((k == 1) && (l == 3))
                    {
                        F[i*m * 16 + j * 16 + k * 4 + l] = Fxy[i*m + j + 1];
                    }
                    else if ((k == 3) && (l == 1))
                    {
                        F[i*m * 16 + j * 16 + k * 4 + l] = Fxy[(i + 1)*m + j];
                    }
                    else if ((k == 3) && (l == 3))
                    {
                        F[i*m * 16 + j * 16 + k * 4 + l] = Fxy[(i + 1)*m + j + 1];
                    }
                }
            }
        }
    }

    for (i = 0; i < n ; i++)
    {
        for (j = 0; j < m ; j++)
        {
            for (k = 0; k < 4; k++)
            {
                for (l = 0; l < 4; l++)
                {
                    coeffs[i*m * 16 + j * 16 + k * 4 + l] = 0;
                    for (int p = 0; p < 4; p++)
                    {
                        for (int q = 0; q < 4; q++)
                        {
                            coeffs[i*m * 16 + j * 16 + k * 4 + l] += A(x[i + 1] - x[i], k, p) *
                                F[i*m * 16 + j * 16 + p * 4 + q] * A(y[j + 1] - y[j], l, q);
                        }
                    }

                }
            }
        }
    }

    if (Gx) delete[] Gx;
    if (Gy) delete[] Gy;
    if (Fx) delete[] Fx;
    if (Fxleft) delete[] Fxleft;
    if (Fxright) delete[] Fxright;
    if (Fy) delete[] Fy;
    if (Fxy) delete[] Fxy;
    if (F) delete[] F;

}


double A(double x, int k, int l)
{
    double s;
    if ((k == l) && ((k == 1) || (k == 0)))
        return 1.;

    if(k==2)
    {
        switch(l)
        {
            case 0:
                return (-3.) / (x*x*x);

            case 1:
                return (-2.) / (x*x);

            case 2:
                return (3.) / (x*x*x);

            case 3:
                return  (-1.) / (x*x);

        }
    }

    if(k==3)
    {
        switch(l)
        {
            case 0:
                return (2.) / (x*x*x);

            case 1:
                return 1. / (x*x);

            case 2:
                return (-2.) / (x*x*x);

            case 3:
                return  1. / (x*x);

        }

    }

    return 0.;
}

double Pow(int k, double x)
{
    double s = 1;
    if (k == 0)
        return s;
    for (int i = 0; i < k; i++)
    {
        s *= x;
    }
    return s;
}


double Window::poly(double x_0, double y_0) {
    int i, j;
    double s = 0;
    double delta=1e-4;


    for (i = 0; i < n-1 ; i++)
        if (x_0 <= x[i + 1]) break;



    for (j = 0; j < m-1; j++)
        if (y_0 <= y[j + 1]) break;



    for (int k = 0; k < 4; k++)
    {
        for (int l = 0; l < 4; l++)
        {
            s += coeffs[i*m * 16 + j * 16 + k * 4 + l] * Pow(k, (x_0-x[i])) * Pow(l, y_0-y[j]);
        }
    }

    return s;
}

double min(double x, double y)
{
    if(x<y)
        return x;

    return y;
}

double max(double x, double y)
{
    if(x<y)
        return y;

    return x;
}
