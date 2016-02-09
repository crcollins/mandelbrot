#include <stdio.h>
#include <stdlib.h>
#include "util.h"


int main(int argc, char** argv)
{

    
    int i, j;

    struct Screen s;
    s = getScreen(argc, argv);
    double cr, ci;
    int *image = (int *)malloc(s.screensize * sizeof(int));

    for (i=0; i < s.height; i++) {
        for (j=0; j < s.width; j++) {
            cr = j * s.dx + s.minx;
            ci = i * s.dy + s.miny;
            image[i*s.width + j] = insideMandelbrot(cr, ci, s.iterations);
        }
    }
   
    writePPM("mandelbrot.ppm", image, s.width, s.height, s.iterations);
    free(image);

    return 0;
}
        
