#include "marchee.h"

float * moyenne(float * prix, uint PRIXS, uint MOY) {
	float * val = malloc(sizeof(float) * PRIXS);
	//
	memset(val, 0, sizeof(float) * MOY);
	//
	val[MOY] = 0;
	for (uint i=0; i < MOY; i++) val[MOY] += prixs[i];
	val[MOY] /= MOY;
	//
	for (uint i=1; i < PRIXS-MOY; i++) {
		val[MOY+i] = (val[MOY+i-1]*MOY + prixs[MOY + i] - prixs[i-1])/MOY;
	};
	//
	return val;
};

float * ema(float * prix, uint PRIXS, uint PERIODE) {
	float * val = malloc(sizeof(float) * PRIXS);
	//
	float k = 1.0 / (1 + (float)PERIODE);
	//
	val[0] = prixs[0]*k + 0.0;
	for (uint i=1; i < PRIXS; i++) val[i] = prixs[i]*k + val[i-1]*(1.0-k);
	//
	return val;
};

/*Infos :
0|	moy1000 5 valeurs d'intervalle 1000 (ce pourait etre d'intervalle 6189)
5|	moy100 5 valeurs d'intervalle 100
10|	prixs 10 valeurs d'intervalle 1
20|	ema9 10 d'intervalle 1
30|	ema12 10 d'intervalle 1
40|	ema26 10 d'intervalle 1
50|	macd-macd 10 d'intervalle 1 = ema12-ma26 (pas de signal car c'est le ema9)
Depart = 5*1000 (le maximum)
*/

static void les_valeurs() {
	uint DEPART = 5000;
	PRIXS_VALIDES = PRIXS - DEPART -1;
	//
	INFOS_FOURNIE = 5+5+10+ 10+10+10+ 0+10;
	INFOS = INFOS_FOURNIE * PRIXS_VALIDES;
	//
	valeurs = malloc(sizeof(float) * INFOS);
	//
	float * prixs_norme = malloc(sizeof(float) * PRIXS);
	float _min=prixs[0], _max=prixs[0];
	for (uint i=0; i < PRIXS; i++) {
		if (_min > prixs[i]) _min = prixs[i];
		if (_max < prixs[i]) _max = prixs[i];
	}
	const float _min_max = _min - _max;
	for (uint i=0; i < PRIXS; i++) {
		prixs_norme[i] = (prixs[i]-_min)/_min_max;
	}
	//
	float * moy1000 = moyenne(prixs_norme, PRIXS, 1000);
	float * moy100 = moyenne(prixs_norme, PRIXS, 100);
	float * ema9 = ema(prixs_norme, PRIXS, 9);
	float * ema12 = ema(prixs_norme, PRIXS, 12);
	float * ema26 = ema(prixs_norme, PRIXS, 26);
	//
	for (uint i=0; i < PRIXS_VALIDES; i++) {
		valeurs[i*INFOS_FOURNIE + 0] = moy1000[DEPART+i-4000];
		valeurs[i*INFOS_FOURNIE + 1] = moy1000[DEPART+i-3000];
		valeurs[i*INFOS_FOURNIE + 2] = moy1000[DEPART+i-2000];
		valeurs[i*INFOS_FOURNIE + 3] = moy1000[DEPART+i-1000];
		valeurs[i*INFOS_FOURNIE + 4] = moy1000[DEPART+i];
		//
		valeurs[i*INFOS_FOURNIE + 5] = moy100[DEPART+i-400];
		valeurs[i*INFOS_FOURNIE + 6] = moy100[DEPART+i-300];
		valeurs[i*INFOS_FOURNIE + 7] = moy100[DEPART+i-200];
		valeurs[i*INFOS_FOURNIE + 8] = moy100[DEPART+i-100];
		valeurs[i*INFOS_FOURNIE + 9] = moy100[DEPART+i];
		//
		valeurs[i*INFOS_FOURNIE + 10] = prixs_norme[DEPART+i-9];
		valeurs[i*INFOS_FOURNIE + 11] = prixs_norme[DEPART+i-8];
		valeurs[i*INFOS_FOURNIE + 12] = prixs_norme[DEPART+i-7];
		valeurs[i*INFOS_FOURNIE + 13] = prixs_norme[DEPART+i-6];
		valeurs[i*INFOS_FOURNIE + 14] = prixs_norme[DEPART+i-5];
		valeurs[i*INFOS_FOURNIE + 15] = prixs_norme[DEPART+i-4];
		valeurs[i*INFOS_FOURNIE + 16] = prixs_norme[DEPART+i-3];
		valeurs[i*INFOS_FOURNIE + 17] = prixs_norme[DEPART+i-2];
		valeurs[i*INFOS_FOURNIE + 18] = prixs_norme[DEPART+i-1];
		valeurs[i*INFOS_FOURNIE + 19] = prixs_norme[DEPART+i+0];
		//
		valeurs[i*INFOS_FOURNIE + 20] = ema9[DEPART+i-9];
		valeurs[i*INFOS_FOURNIE + 21] = ema9[DEPART+i-8];
		valeurs[i*INFOS_FOURNIE + 22] = ema9[DEPART+i-7];
		valeurs[i*INFOS_FOURNIE + 23] = ema9[DEPART+i-6];
		valeurs[i*INFOS_FOURNIE + 24] = ema9[DEPART+i-5];
		valeurs[i*INFOS_FOURNIE + 25] = ema9[DEPART+i-4];
		valeurs[i*INFOS_FOURNIE + 26] = ema9[DEPART+i-3];
		valeurs[i*INFOS_FOURNIE + 27] = ema9[DEPART+i-2];
		valeurs[i*INFOS_FOURNIE + 28] = ema9[DEPART+i-1];
		valeurs[i*INFOS_FOURNIE + 29] = ema9[DEPART+i];
		//
		valeurs[i*INFOS_FOURNIE + 30] = ema12[DEPART+i-9];
		valeurs[i*INFOS_FOURNIE + 31] = ema12[DEPART+i-8];
		valeurs[i*INFOS_FOURNIE + 32] = ema12[DEPART+i-7];
		valeurs[i*INFOS_FOURNIE + 33] = ema12[DEPART+i-6];
		valeurs[i*INFOS_FOURNIE + 34] = ema12[DEPART+i-5];
		valeurs[i*INFOS_FOURNIE + 35] = ema12[DEPART+i-4];
		valeurs[i*INFOS_FOURNIE + 36] = ema12[DEPART+i-3];
		valeurs[i*INFOS_FOURNIE + 37] = ema12[DEPART+i-2];
		valeurs[i*INFOS_FOURNIE + 38] = ema12[DEPART+i-1];
		valeurs[i*INFOS_FOURNIE + 39] = ema12[DEPART+i];
		//
		valeurs[i*INFOS_FOURNIE + 40] = ema26[DEPART+i-9];
		valeurs[i*INFOS_FOURNIE + 41] = ema26[DEPART+i-8];
		valeurs[i*INFOS_FOURNIE + 42] = ema26[DEPART+i-7];
		valeurs[i*INFOS_FOURNIE + 43] = ema26[DEPART+i-6];
		valeurs[i*INFOS_FOURNIE + 44] = ema26[DEPART+i-5];
		valeurs[i*INFOS_FOURNIE + 45] = ema26[DEPART+i-4];
		valeurs[i*INFOS_FOURNIE + 46] = ema26[DEPART+i-3];
		valeurs[i*INFOS_FOURNIE + 47] = ema26[DEPART+i-2];
		valeurs[i*INFOS_FOURNIE + 48] = ema26[DEPART+i-1];
		valeurs[i*INFOS_FOURNIE + 49] = ema26[DEPART+i];
		//
		valeurs[i*INFOS_FOURNIE + 50] = ema12[DEPART+i-9] - ema26[DEPART+i-9];
		valeurs[i*INFOS_FOURNIE + 51] = ema12[DEPART+i-8] - ema26[DEPART+i-8];
		valeurs[i*INFOS_FOURNIE + 52] = ema12[DEPART+i-7] - ema26[DEPART+i-7];
		valeurs[i*INFOS_FOURNIE + 53] = ema12[DEPART+i-6] - ema26[DEPART+i-6];
		valeurs[i*INFOS_FOURNIE + 54] = ema12[DEPART+i-5] - ema26[DEPART+i-5];
		valeurs[i*INFOS_FOURNIE + 55] = ema12[DEPART+i-4] - ema26[DEPART+i-4];
		valeurs[i*INFOS_FOURNIE + 56] = ema12[DEPART+i-3] - ema26[DEPART+i-3];
		valeurs[i*INFOS_FOURNIE + 57] = ema12[DEPART+i-2] - ema26[DEPART+i-2];
		valeurs[i*INFOS_FOURNIE + 58] = ema12[DEPART+i-1] - ema26[DEPART+i-1];
		valeurs[i*INFOS_FOURNIE + 59] = ema12[DEPART+i] - ema26[DEPART+i];
	}
	//
	FREE(6, prixs_norme, moy1000, moy100, ema9, ema12, ema26);
	//
	float * _prixs = malloc(sizeof(float) * PRIXS_VALIDES);
	memcpy(_prixs, prixs + DEPART+1, sizeof(float) * PRIXS_VALIDES);
	free(prixs);
	prixs = _prixs;
};

void charger_les_prixs() {
	FILE * fp = fopen("prix/prixs.bin", "rb");
	fread(&PRIXS, sizeof(float), 1, fp);
	prixs = (float*)malloc(sizeof(float) * PRIXS);
	fread(prixs, sizeof(float), PRIXS, fp);
	fclose(fp);
	//
	les_valeurs();
};

void liberer_les_prixs() {
	free(valeurs);
	free(prixs);
}