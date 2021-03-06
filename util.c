#include "util.h"

double random(double a, double b) {
    return (b-a) * (double)rand()/(double)RAND_MAX + a;
}


struct Screen getScreen(int argc, char** argv)
{
    struct Screen s;
    s.height = 800;
    s.width = 1200;
    s.iterations = 255;
    s.centerx = -0.5;
    s.centery = 0.0;
    s.zoom = 1.0;
    

    switch (argc) {
        case 7:
            s.centery = atof(argv[6]);
            s.centerx = atof(argv[5]);
        case 5:
            s.zoom = atof(argv[4]);
        case 4: 
            s.iterations = atoi(argv[3]);
        case 3: 
            s.width = atoi(argv[2]);
            s.height = atoi(argv[1]);
        case 1:
            break;
        default:
            printf("%s [height] [width] [iterations] [zoom] [centerx] [centery]\n", argv[0]);
            exit(1);
    }
    
    s.aspect = (double)s.height / (double)s.width;
    s.minx = s.centerx - (1.5 / s.zoom);
    s.maxx = s.centerx + (1.5 / s.zoom);
    s.rangex = s.maxx - s.minx;
    
    s.rangey = s.rangex * s.aspect;
    s.miny = s.centery - 0.5 * s.rangey;
    s.maxy = s.centery + 0.5 * s.rangey;
    
    s.screensize = s.height * s.width;
    s.dx = (s.maxx - s.minx) / s.width;
    s.dy = (s.maxy - s.miny) / s.height;
    return s;
}


void writePPM(const char *filename, const int *array, int width, int height, int max)
{
    int i, j;
    FILE *f;
    f = fopen(filename, "w");
    fprintf(f, "P2\n");
    fprintf(f, "%d %d\n", width, height);
    fprintf(f, "255\n");
    for (i=0; i < height; i++) {
        for (j=0; j < width; j++) {
            fprintf(f, "%d ", (int)(255*(array[i*width + j] / (double)max)));
        }
        fprintf(f, "\n");
    }
    fclose(f);
}


int tallyPath(const double *zrArray, const double *ziArray, int count,  int *hist, const struct Screen s)
{
    int i, j, k, tally;
    tally = 0;
    for (k=1; k < count; k++) {
        i = (int)(round((zrArray[k] - s.minx) / s.dx));
        j = (int)(round((ziArray[k] - s.miny) / s.dy));
        
        if (i < 0 || j < 0 || i >= s.width || j >= s.height) continue;
            
        tally += 1;
        if (hist != NULL) {
            hist[j*s.width + i] += 1;
        }
    }
    return tally;
}


void mutate(double cr, double ci, double *crnew, double *cinew, double zoom)
{
    /* http://www.steckles.com/buddha/ */
    double r1 = (1./ zoom) * 0.0001;
    double r2 = (1. / zoom) * 0.01;
    double phi = random(0., 1.) * 2 * M_PI;
    double r = r2 * exp( -log(r2 / r1) * random(0., 1.));
    *crnew = cr + r * cos(phi);
    *cinew = ci + r * sin(phi);
}


int quickCheck(double x, double y)
{
    double p = pow(x - 0.25, 2) + y*y;
    return (x < p - 2 * p*p + 0.25) && (pow(x + 1, 2) + y*y < 0.0625);
}

void iterateMandelbrot(double *zr, double *zi, double cr, double ci)
{
    double temp = (*zr)*(*zr) - (*zi)*(*zi) + cr;
    *zi = 2 * (*zr) * (*zi) + ci;
    *zr = temp;
}

int insideMandelbrot(double cr, double ci, int maxIter)
{
    int i;
    double zr = 0.0;
    double zi = 0.0;
    if (quickCheck(cr, ci)) {
        return maxIter;
    }
    for (i=0; i < maxIter; i++) {
        iterateMandelbrot(&zr, &zi, cr, ci);
        if ((zr*zr + zi*zi) > 4.0) {
            return i;
        }
    }
    return maxIter;
}

int getMandelbrotPath(double *zrArray, double *ziArray, double cr, double ci, int maxIter)
{
    int i;
    double zr = 0.0;
    double zi = 0.0;
    if (quickCheck(cr, ci)) {
        return -1;
    }
    for (i=0; i < maxIter; i++) {
        iterateMandelbrot(&zr, &zi, cr, ci);
        zrArray[i] = zr;
        ziArray[i] = zi;
        if ((zr*zr + zi*zi) > 4.0) {
            return i+1;
        }
    }
    return maxIter;


}

