/*
############################################################
#### 2024.12.24. ###########################################
#### EunChan Lee ###########################################
#### eunchan_supernova@gm.gist.ac.kr #######################
############################################################
*/

#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "throw_needle.h"

#define NEEDLE_LENGTH 1.0
#define LINE_SPACING 2.0

double throw_needle(int num_throws) {
    int hits = 0;
    srand(time(NULL));

    for (int i = 0; i < num_throws; i++) {
        double distance = ((double)rand() / RAND_MAX) * (LINE_SPACING / 2.0);
        double angle = ((double)rand() / RAND_MAX) * M_PI;

        if (distance <= (NEEDLE_LENGTH / 2.0) * sin(angle)) {
            hits++;
        }
    }
    return (2.0 * NEEDLE_LENGTH * num_throws) / (LINE_SPACING * hits);
}
