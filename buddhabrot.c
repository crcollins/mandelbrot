#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>
#include "util.h"


int main(int argc, char** argv)
{

    int i;
    int count, countnew, tally, tallynew, max;
    int world_rank, world_size;
    double cr, ci, crnew, cinew;
    
    struct Screen s;
    s.height = 800;
    s.width = 1200;
    double zoom = 1.0;
    s.minx = -2.0/zoom;
    s.maxx = 1.0/zoom;
    s.miny = -1.0/zoom;
    s.maxy = 1.0/zoom;
    s.screensize = s.height * s.width;
    s.rangex = s.maxx - s.minx;
    s.rangey = s.maxy - s.miny;
    s.dx = (s.maxx - s.minx) / s.width;
    s.dy = (s.maxy - s.miny) / s.height;

    int samples = 10000000;
    int sampleFreq = 10;
    int maxIter = 255;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    srand(time(NULL)*world_rank);

    double *zrArray = (double *)calloc(maxIter, sizeof(double));
    double *ziArray = (double *)calloc(maxIter, sizeof(double));

    int *hist = (int *)calloc(s.screensize, sizeof(int));
    int *global_hist = (int *)calloc(s.screensize, sizeof(int));

    tally = 0;
    tallynew = 0;

    cr = random(s.minx, s.maxx);
    ci = random(s.miny, s.maxy);
    double *zrArraynew = (double *)calloc(maxIter, sizeof(double));
    double *ziArraynew = (double *)calloc(maxIter, sizeof(double));
    double *tempz;
    for (i=0; i < samples; i++) {
        if ((world_rank == 0) && (i % 10000 == 0)) {
            printf("%d\n", i);
        }

        if (random(0.0, 1.0) > .8) {
            crnew = random(s.minx, s.maxx);
            cinew = random(s.miny, s.maxy);
        } else {
            mutate(cr, ci, &crnew, &cinew, zoom);
        }
        countnew = getMandelbrotPath(zrArraynew, ziArraynew, crnew, cinew, maxIter);
        if (countnew == -1 || countnew == maxIter) continue;
        tallynew = tallyPath(zrArraynew, ziArraynew, countnew, NULL, s);
        if ((tallynew >= tally) || (random(0.0, 1.0) < ((double)tallynew/(double)tally))) {
            tally = tallynew;
            cr = crnew;
            ci = cinew;

            tempz = zrArray;
            zrArray = zrArraynew;
            zrArraynew = tempz;

            tempz = ziArray;
            ziArray = ziArraynew;
            ziArraynew = tempz;

            count = countnew;
        }
        if ((i % sampleFreq) != 0) continue;
        tallyPath(zrArray, ziArray, count, hist, s);
    }
    MPI_Reduce(hist, global_hist, s.screensize, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (world_rank == 0) {
        for (i=0; i < s.screensize; i++) {
            if (global_hist[i] > max) { 
                max = global_hist[i];
            }
        }
        printf("max: %d %d %d", max, hist[10], global_hist[10]);
        writePPM("buddhabrot.ppm", global_hist, s.width, s.height, max);
    }

    free(zrArray);
    free(ziArray);
    free(zrArraynew);
    free(ziArraynew);
    free(global_hist);
    free(hist);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}
        
