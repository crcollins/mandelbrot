#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327
#endif

struct Screen {
    int width;
    int height;
    double minx;
    double maxx;
    double miny;
    double maxy;
    int screensize;
    double rangex;
    double rangey;
    double dx;
    double dy;
};


double random(double a, double b);
void writePPM(const char *filename, const int *array, int width, int height, int max);

int tallyPath(const double *zrArray, const double *ziArray, int count,  int *hist, const struct Screen s);
void mutate(double cr, double ci, double *crnew, double *cinew, double zoom);

int quickCheck(double x, double y);
void iterateMandelbrot(double *zr, double *zi, double cr, double ci);
int insideMandelbrot(double cr, double ci, int maxIter);
int getMandelbrotPath(double *zrArray, double *ziArray, double cr, double ci, int maxIter);

#endif
