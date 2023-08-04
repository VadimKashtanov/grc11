#include "mdl.h"

float mdl_pertes(Mdl_t * mdl, uint depart) {
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
		if (changement < 0)
			usdt += changement;
	}
	//
	return usdt - DEPART_SESSION;
};

float mdl_pertes_sessions(Mdl_t * mdl, uint S) {
	float pertes = 0;
	for (uint i=0; i < S; i++) {
		pertes += mdl_pertes(mdl, rand() % (PRIXS_VALIDES-TAILLE_SESSION-1));
	}
	return pertes;
};

float mdl_pertes_sessions_a_partire(Mdl_t * mdl, uint S, uint depart) {
	float pertes = 0;
	for (uint i=0; i < S; i++) {
		pertes += mdl_pertes(mdl, depart + rand() % (MAX_DEPART-depart));
	}
	return pertes;
};

float mdl_pertes_sessions_jusqua(Mdl_t * mdl, uint S, uint jusqua) {
	float pertes = 0;
	for (uint i=0; i < S; i++) {
		pertes += mdl_pertes(mdl, rand() % (MAX_DEPART-jusqua));
	}
	return pertes;
};