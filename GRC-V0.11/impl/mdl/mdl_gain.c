#include "mdl.h"

float mdl_gain(Mdl_t * mdl, uint depart) {
	float usdt = DEPART_SESSION;
	float pourcent, levier, p0, p1;
	float changement;
	//
	for (uint i=0; i < TAILLE_SESSION; i++)
	{
		mdl_nouveau_jour(mdl, i, depart);
		mdl_f(mdl, i);
		//
		pourcent = tanh(socle->_var[(i+1)*total - 2]);
		levier = 1+(LEVIER_MAX-1)/(1.0 + expf(-socle->_var[(i+1)*total - 1]));
		p0 = prixs[depart + i];
		p1 = prixs[depart + i+1];
		//
		changement = usdt*pourcent*levier*(p1/p0 - 1);
		usdt += changement;
	}
	//
	return usdt - DEPART_SESSION;
};

float mdl_gain_sessions(Mdl_t * mdl, uint S) {
	float gain = 0;
	for (uint i=0; i < S; i++) {
		gain += mdl_gain(mdl, rand() % MAX_DEPART);
	}
	return gain;
};

float mdl_gain_sessions_a_partire(Mdl_t * mdl, uint S, uint depart) {
	float gain = 0;
	for (uint i=0; i < S; i++) {
		gain += mdl_gain(mdl, depart + rand() % (MAX_DEPART-depart));
	}
	return gain;
};

float mdl_gain_sessions_jusqua(Mdl_t * mdl, uint S, uint jusqua) {
	float gain = 0;
	for (uint i=0; i < S; i++) {
		gain += mdl_gain(mdl, rand() % (MAX_DEPART-jusqua));
	}
	return gain;
};