// hw4_BN_QRNG.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
/*
############################################################
#### 2024.12.24. ###########################################
#### EunChan Lee ###########################################
#### eunchan_supernova@gm.gist.ac.kr #######################
############################################################
*/
///////////////////////////////////////////////////////////////////////
///////////Buffon's Needle Hw with Quasi-RN generation/////////////////
///////////////////////////////////////////////////////////////////////
//

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <cmath>
#include <array>
#include <string>
#include <iterator>
#include <time.h>
#include <algorithm>

//#include "floating_point_stub.hpp"
//#include "sprng.h"  // SPRNG 헤더 파일 포함
#include "quasirand.hpp"  // Quasi-Random Generator 헤더 파일 포함

using namespace quasirand;

// 뷔퐁의 바늘 문제 해결 함수
void buffon_needle_simulation(int n, double k, FILE* file) {
    QuasiRandom<2> qr;  // 2차원 Halton 시퀀스 생성기
    qr.reset(0.13);          // Quasi-Random 시퀀스 초기화

    double d = 1.0;      // 평행선 간격
    double l = k * d;    // 바늘 길이
    int count = 0;       // 교차한 바늘 수
    //array<double, 2> point = qr(n);

    for (int i = 0; i < n; i++) {
        // 2차원 Quasi-Random 값 생성
//        std::vector<double> point = qr();
        std::array<double, 2> point = qr();  // [0, 1) 범위의 2차원 Halton 시퀀스
//        auto point = qr();

        double y0 = point[0] * d + 1;           // 중점의 y 좌표
        double theta = point[1] * 2 * M_PI; // 0 ~ 2π 사이의 각도

        // 바늘 끝점 계산
        double y1 = y0 + (l / 2) * std::sin(theta);
        double y2 = y0 - (l / 2) * std::sin(theta);

        // 끝점들이 다른 평행선 위에 있는지 확인
        if ((int)y1 != (int)y2) {
            count++;  // 교차한 경우 count 증가
        }
    }

    // 확률 계산
    double P = static_cast<double>(count) / n;  // 교차 확률
    double pi_estimate = (P > 0) ? (2 * l) / (P * d) : INFINITY;  // 원주율 추정값 계산

    fprintf(file, "%d %.8f %.8f\n", n, pi_estimate, fabs(pi_estimate - M_PI));
}

// 데이터 파일 생성 및 Gnuplot 사용
void run_simulation_and_plot() {
    const char* data_file = "buffon_data.txt";
    const char* gnuplot_file = "buffon_plot.gnu";

    int n_values[] = { 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };
    int num_values = sizeof(n_values) / sizeof(n_values[0]);

    FILE* data = fopen(data_file, "w");
    if (!data) {
        perror("Failed to open data file");
        return;
    }

    for (int i = 0; i < num_values; i++) {
        buffon_needle_simulation(n_values[i], 1.0, data);
    }

    fclose(data);

    // Gnuplot 스크립트 작성
    FILE* gnuplot = fopen(gnuplot_file, "w");
    if (!gnuplot) {
        perror("Failed to open gnuplot file");
        return;
    }

    fprintf(gnuplot, "set terminal pngcairo size 1200,600\n"); //(1)
    //(2)    fprintf(gnuplot, "set terminal postscript eps color\n"); 
    fprintf(gnuplot, "set output 'buffon_results.png'\n"); ////(1) 저장 명령 주석 처리 가능
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
    system("gnuplot buffon_plot.gnu"); //(1)
}

int main() {
    run_simulation_and_plot();
    return 0;
}