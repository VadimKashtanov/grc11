#include "mdl.h"

float moyenne_seconds(Mdl_t * mdl, uint depart, uint N) {
	float _f, f_x, f_y, f_xy;

	float _1E5 = 1e-3;

	_f = mdl_gain(mdl, depart);
	float _somme = 0;
	//
	uint i,j;
	for (uint k=0; k < N; k++) {
		i = rand() % socle->poids;
		j = rand() % socle->poids;
		//
		//f'(x+,y+)-f(y+)-f(y+)+f()
		socle->_poid[i] += _1E5;
		f_x = mdl_gain(mdl, depart);
		socle->_poid[j] += _1E5;
		f_xy = mdl_gain(mdl, depart);
		socle->_poid[i] -= _1E5;
		f_y = mdl_gain(mdl, depart);
		socle->_poid[j] -= _1E5;
		//
		_somme += fabs((f_xy-f_x-f_y+_f)/(_1E5*_1E5)  /  TAILLE_SESSION);
	}
	return _somme / N;
};

float moyenne_diag(Mdl_t * mdl, uint depart, uint N) {
	float _f, f_x, f_xx;

	float _1E5 = 1e-3;

	_f = mdl_gain(mdl, depart);
	float _somme = 0;
	//
	uint i;
	for (uint k=0; k < N; k++) {
		i = rand() % socle->poids;
		//
		//f'(x+,y+)-f(y+)-f(y+)+f()
		socle->_poid[i] += _1E5;
		f_x = mdl_gain(mdl, depart);
		socle->_poid[i] += _1E5;
		f_xx = mdl_gain(mdl, depart);
		socle->_poid[i] -= 2*_1E5;
		//
		_somme += fabs((f_xx-2*f_x+_f)/(_1E5*_1E5)  /  TAILLE_SESSION);
	}
	return _somme / N;
};

float* tableau_second(Mdl_t * mdl, uint depart) {
	uint N = socle->poids;
	float * tableau = malloc(sizeof(float) * N*N);

	float _f, f_x, f_y, f_xy;

	float _1E5 = 1e-3;
	float _1E10 = _1E5*_1E5;

	_f = mdl_gain(mdl, depart);
	//
	for (uint i=0; i < socle->poids; i++) {
		for (uint j=0; j < socle->poids; j++) {
			//f'(x+,y+)-f(y+)-f(y+)+f()
			socle->_poid[i] += _1E5;
			f_x = mdl_gain(mdl, depart);
			socle->_poid[j] += _1E5;
			f_xy = mdl_gain(mdl, depart);
			socle->_poid[i] -= _1E5;
			f_y = mdl_gain(mdl, depart);
			socle->_poid[j] -= _1E5;
			//
			tableau[i*N + j] = (f_xy-f_x-f_y+_f)/(_1E10 * TAILLE_SESSION);
		}
	}

	return tableau;
};