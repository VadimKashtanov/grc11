#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <time.h>
#include <stdarg.h>

#define OK(str, ...) printf("[\033[35;1m*\033[0m]:\033[96m%s:(%d)\033[32m: " str "\033[0m\n", __FILE__, __LINE__, ##__VA_ARGS__);
#define MSG(str, ...) printf("\033[35;1m -> \033[0m \033[96m%s:(%d)\033[35m: " str "\033[0m\n", __FILE__, __LINE__, ##__VA_ARGS__);
#define ERR(str, ...) do {printf("[\033[30;101mError\033[0m]:\033[96m%s:(%d)\033[30m: " str "\033[0m\n", __FILE__, __LINE__, ##__VA_ARGS__);raise(SIGINT);} while (0);

void FREE(uint len, ...);

float rnd();

float gauss(float x);
float d_gauss(float x);
float dd_gauss(float x);

float logistique(float x);
float d_logistique(float x);
float dd_logistique(float x);

//inline float tanh(float x);
float d_tanh(float x);
float dd_tanh(float x);

//	=== Couleures ===
#define FONT_VERT(str, ...) printf("\033[42m" str "\033[0m", ##__VA_ARGS__);
#define FONT_ROUGE(str, ...) printf("\033[41m" str "\033[0m", ##__VA_ARGS__);
#define FONT_JAUNE(str, ...) printf("\033[43m" str "\033[0m", ##__VA_ARGS__);

#define ROUGE(str, ...) printf("\033[91m" str "\033[0m", ##__VA_ARGS__);
#define VERT(str, ...) printf("\033[92m" str "\033[0m", ##__VA_ARGS__);
#define JAUNE(str, ...) printf("\033[93m" str "\033[0m", ##__VA_ARGS__);

#define JAUNE_GRAS(str, ...) printf("\033[1;93m" str "\033[0m", ##__VA_ARGS__);

//	=================

void gnuplot(float * arr, uint len);

void gnuplots(uint N, uint taille, float ** arrs);
void gnuplots_normaliser(uint N, uint taille, float ** arrs);

void gnuplots_titre(uint N, uint taille, float ** arrs, char * titre);

//	==================

#define INIT_CHRONO() time_t chrono;
#define DEPART_CHRONOS() chrono = clock();
#define VALEUR_CHRONO() (((float)(clock()-chrono))/CLOCKS_PER_SEC)