// hw3_BN_SPRNG.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
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
#include <math.h>
#include <time.h>
#include "sprng.h" // SPRNG 헤더 포함

#define DEFAULT_GTYPE 1
#define DEFAULT_SEED time(0)

// SPRNG 기반 난수 생성 클래스
class MySprng : public Sprng {
private:
    int seed;
    int gtype;

public:
    MySprng(int gtype = DEFAULT_GTYPE, int seed = DEFAULT_SEED) : gtype(gtype), seed(seed) {}

    int init_rng(int gtype, int seed, int param, int stream) override {
        this->gtype = gtype;
        this->seed = seed;
        srand(seed);
        return 0;
    }

    double get_rn_dbl() override {
        return (double)rand() / RAND_MAX;
    }

    int get_rn_int() override { return 0; }
    float get_rn_flt() override { return 0.0f; }
    int spawn_rng(int nspawned, Sprng*** newgens) override { return 0; }
    int get_seed_rng() override { return seed; }
    int free_rng() override { return 0; }
    int pack_rng(char** buffer) override { return 0; }
    int print_rng() override { return 0; }
    int unpack_rng(char* packed) override { return 0; }
};

// Buffon 바늘 시뮬레이션
void buffon_needle_simulation(int n, double k, int gtype, int seed, FILE* file) {
    MySprng rng(gtype, seed);

    double d = 1.0;
    double l = k * d;
    int count = 0;

    for (int i = 0; i < n; i++) {
        double y0 = rng.get_rn_dbl() + 1;
        double theta = rng.get_rn_dbl() * 2 * M_PI;

        double y1 = y0 + (l / 2) * sin(theta);
        double y2 = y0 - (l / 2) * sin(theta);

        if ((int)y1 != (int)y2) {
            count++;
        }
    }

    double P = (double)count / n;
    double pi_estimate = (P > 0) ? (2 * l) / (P * d) : INFINITY;

    fprintf(file, "%d %.8f %.8f\n", n, pi_estimate, fabs(pi_estimate - M_PI));
}

// 데이터 파일 생성 및 Gnuplot 사용
void run_simulation_and_plot() {
    const char* data_file = "buffon_data.txt";
    const char* gnuplot_file = "buffon_plot.gnu";

    int n_values[] = { 100, 1000, 10000, 100000, 1000000, 10000000 };
    int num_values = sizeof(n_values) / sizeof(n_values[0]);

    FILE* data = fopen(data_file, "w");
    if (!data) {
        perror("Failed to open data file");
        return;
    }

    for (int i = 0; i < num_values; i++) {
        buffon_needle_simulation(n_values[i], 1.0, DEFAULT_GTYPE, DEFAULT_SEED, data);
    }

    fclose(data);

    // Gnuplot 스크립트 작성
    FILE* gnuplot = fopen(gnuplot_file, "w");
    if (!gnuplot) {
        perror("Failed to open gnuplot file");
        return;
    }

    fprintf(gnuplot, "set terminal pngcairo size 1200,600\n"); // (1)
//(2)    fprintf(gnuplot, "set terminal postscript eps color\n");
    fprintf(gnuplot, "set output 'buffon_results.png'\n"); //// (1) 저장 명령 주석 처리 가능
//(2)    fprintf(gnuplot, "set output 'buffon_results.eps'\n"); // 저장 명령 주석 처리 가능
    fprintf(gnuplot, "set multiplot layout 1,2\n");

    // 첫 번째 그래프: pi 값
    fprintf(gnuplot, "set logscale x\n");
    fprintf(gnuplot, "set xlabel 'Number of needles (log scale)'\n");
    fprintf(gnuplot, "set ylabel 'Estimated pi'\n");
    fprintf(gnuplot, "set title 'Estimated pi as n values'\n");
    fprintf(gnuplot, "plot '%s' using 1:2 with linespoints title 'Estimated pi', \\\n", data_file);
    fprintf(gnuplot, "     %.15f title 'Real pi value (π)' with lines lw 2\n", M_PI);

    // 두 번째 그래프: 오차
    fprintf(gnuplot, "set logscale x\n");
    fprintf(gnuplot, "set logscale y\n");
    fprintf(gnuplot, "set xlabel 'Number of needles (log scale)'\n");
    fprintf(gnuplot, "set ylabel 'Error (log scale)'\n");
    fprintf(gnuplot, "set title 'Error as n values'\n");
    fprintf(gnuplot, "f(x) = a * x + b\n");
    fprintf(gnuplot, "fit f(x) '%s' using (log10($1)):(log10($3)) via a, b\n", data_file);
    fprintf(gnuplot, "plot '%s' using 1:3 with linespoints title 'Error', \\\n", data_file);
    fprintf(gnuplot, "     10**(f(log10(x))) title sprintf('Fit: Slope = %%0.4f', a) with lines lw 2\n");

    fprintf(gnuplot, "unset multiplot\n");

    fclose(gnuplot);

    // Gnuplot 실행
    system("gnuplot buffon_plot.gnu"); // (1)
}

int main() {
    run_simulation_and_plot();
    return 0;
}