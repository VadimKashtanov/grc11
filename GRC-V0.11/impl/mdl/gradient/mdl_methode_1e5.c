#include "mdl.h"

void mdl_G_mdt1e5_gradient(Mdl_t * mdl, uint depart) {
	float _fx, _f;
	_f = mdl_gain(mdl, depart);
	//
	float _1E5 = 1e-3;
	//
	for (uint i=0; i < socle->poids; i++) {
		socle->_poid[i] += _1E5;
		_fx = mdl_gain(mdl, depart);
		socle->_poid[i] -= _1E5;
		//
		socle->_dpoid[i] += (_fx-_f)/_1E5    /  (TAILLE_SESSION);
	}
};

void mdl_G_mdt1e5_gradient_session(Mdl_t * mdl, uint S, uint graine) {
	memset(socle->_dpoid, 0, sizeof(float) * socle->poids);
	//
	for (uint i=0; i < S; i++) {
		srand(graine + i + 1);
		mdl_G_mdt1e5_gradient(mdl, rand() % MAX_DEPART);
	}
	for (uint i=0; i < socle->poids; i++) socle->_dpoid[i] /= S;
};

//==============================================================

void mdl_P_mdt1e5_gradient(Mdl_t * mdl, uint depart) {
	float _fx, _f;
	_f = mdl_pertes(mdl, depart);
	//
	float _1E5 = 1e-3;
	//
	for (uint i=0; i < socle->poids; i++) {
		socle->_poid[i] += _1E5;
		_fx = mdl_pertes(mdl, depart);
		socle->_poid[i] -= _1E5;
		//
		socle->_dpoid[i] += (_fx-_f)/_1E5    /  (TAILLE_SESSION);
	}
};

void mdl_P_mdt1e5_gradient_session(Mdl_t * mdl, uint S, uint graine) {
	memset(socle->_dpoid, 0, sizeof(float) * socle->poids);
	//
	for (uint i=0; i < S; i++) {
		srand(graine + i + 1);
		mdl_P_mdt1e5_gradient(mdl, rand() % MAX_DEPART);
	}
	for (uint i=0; i < socle->poids; i++) socle->_dpoid[i] /= S;
};