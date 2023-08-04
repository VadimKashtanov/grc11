#include "etc.h"

void FREE(uint len, ...) {
	va_list valist;
	va_start(valist, len);
	for (uint i=0; i < len; i++)
		free(va_arg(valist, void*));
	va_end(valist);
};

float fastExp3(register float x)  // cubic spline approximation
{
    union { float f; int i; } reinterpreter;

    reinterpreter.i = (int)(12102203.0f*x) + 127*(1 << 23);
    int m = (reinterpreter.i >> 7) & 0xFFFF;  // copy mantissa
    // empirical values for small maximum relative error (8.34e-5):
    reinterpreter.i +=
         ((((((((1277*m) >> 14) + 14825)*m) >> 14) - 79749)*m) >> 11) - 626;
    return reinterpreter.f;
}

float rnd() {return (float)(rand()%100)/99;};

float gauss(register float x) {return fastExp3(-x*x);}
float d_gauss(register float x) {return -2*x*gauss(x);}
float dd_gauss(register float x) {return 4*x*x*gauss(x) - 2*gauss(x);}

float logistique(float x) {return 1 / (1 + fastExp3(-x));}
float d_logistique(float x) {return logistique(x)*(1 - logistique(x));}
float dd_logistique(float x) {return d_logistique(x)*(1+2*logistique(x));}

//inline float tanh(float x) {return exp(-x*x);}
float d_tanh(float x) {return 1 - pow(tanh(x), 2);}
float dd_tanh(float x) {return -2 * tanh(x) * d_tanh(x);}

// ==========================================================================
// ==========================================================================
// ==========================================================================

void gnuplot(float * arr, uint len) {
	char buff[100];
	//
	FILE * fp = fopen("gnuplot_dat.dat", "w");
	//
	for (uint i=0; i < len; i++) {
		if (arr[i] == 123456.0) {
			snprintf(buff, 100, "\n");
			fputs(buff, fp);
		} else {
			snprintf(buff, 100, "%i ", i);
			fputs(buff, fp);
			//
			snprintf(buff, 100, "%f\n", arr[i]);
			fputs(buff, fp);
		}
	}
	fclose(fp);
	//
	system("gnuplot -p -e \"plot 'gnuplot_dat.dat' with lp\"");
	//
	system("rm gnuplot_dat.dat");
};

static void gnuplots_fonction(
	uint MODE,
	uint N, uint taille,
	float ** lsts, char * titre)
{
	//
	float __listes[N][taille];
	//
	if (MODE == 0) {
		for (uint n=0; n < N; n++) {
			for (uint i=0; i < taille; i++) {
				__listes[n][i] = lsts[n][i];
			}
		}
	} else {
		for (uint n=0; n < N; n++) {
			float _max=lsts[n][0], _min=lsts[n][0];
			uint un_nan = 0;
			for (uint i=1; i < taille; i++) {
				if (_max < lsts[n][i]) _max = lsts[n][i];
				if (_min > lsts[n][i]) _min = lsts[n][i];
				if (lsts[n][i] != lsts[n][i]) {
					un_nan = 1;
					break;
				}
			}
			if (un_nan == 0 && _max != _min) {
				for (uint i=0; i < taille; i++) {
					__listes[n][i] = (lsts[n][i]-_min)/(_max-_min);
				}
			} else {
				memset(__listes + n*taille, 0, sizeof(float) * taille);
				if (un_nan == 1) puts("Nan detectee");
				if (_max == _min) puts("Liste uniforme, _max == _min");
			}
		};
	}
	//
	char buff[100];
	//
	FILE * fp = fopen("gnuplot_dat.dat", "w");
	//
	for (uint i=0; i < taille; i++) {
		snprintf(buff, 100, "%i ", i);
		fputs(buff, fp);
		//
		for (uint n=0; n < N; n++) {
			snprintf(buff, 100, "%f ", __listes[n][i]);
			fputs(buff, fp);
		}
		//
		snprintf(buff, 100, "\n");
		fputs(buff, fp);
	}
	fclose(fp);
	//
	char commande[1000 + N*50];
	//
	snprintf(
		commande,
		1000,
		"gnuplot -p -e \"set title \'%s\'; plot 'gnuplot_dat.dat' u 1:2 w l t 'l0'",
		titre);
	//
	for (uint n=1; n < N; n++) {
		snprintf(
			commande+strlen(commande),
			200,
			", 'gnuplot_dat.dat' u 1:%i w l t 'l%i'",
			n+2,
			n);
	}
	snprintf(commande+strlen(commande), 200, "\"");
	//puts(commande);
	//
	system(commande);
	system("rm gnuplot_dat.dat");
};

void gnuplots(uint N, uint taille, float ** arrs) {
	gnuplots_fonction(0, N, taille, arrs, "Courbes");
};

void gnuplots_normaliser(uint N, uint taille, float ** arrs) {
	gnuplots_fonction(1, N, taille, arrs, "Courbes");
};

void gnuplots_titre(uint N, uint taille, float ** arrs, char * titre) {
	gnuplots_fonction(0, N, taille, arrs, titre);
};