// hw5_Makefile.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
/*
############################################################
#### 2024.12.24. ###########################################
#### EunChan Lee ###########################################
#### eunchan_supernova@gm.gist.ac.kr #######################
############################################################
*/
//

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "throw_needle.h"


int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    if (argc != 2) {
        if (MPI_COMM_WORLD == 0) {
            printf("Usage: %s <num_throws>\\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    int num_throws = atoi(argv[1]);
    if (num_throws <= 0) {
        if (MPI_COMM_WORLD == 0) {
            printf("Number of throws must be positive.\\n");
        }
        MPI_Finalize();
        return 1;
    }

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_throws = num_throws / size;
    double local_pi = throw_needle(local_throws);

    double global_pi;
    MPI_Reduce(&local_pi, &global_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        global_pi /= size;
        printf("Estimated value of pi: %f\\n", global_pi);
    }

    MPI_Finalize();
    return 0;
}