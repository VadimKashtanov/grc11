#include "optimisation.h"

void optimiser_gains(Mdl_t * mdl, uint N) {
	float sec = fabs(moyenne_seconds(mdl, rand() % MAX_DEPART, 5));
	float alpha = (sec > 0 ? 1.0/sec : 1.0);
	//
	uint S = 1;

	for (uint p=0; p < N; p++) {
		mdl_G_retropropa_gradient_session(mdl, S, rand() % 1000);
		//
		for (uint i=0; i < socle->poids; i++)
			socle->_poid[i] += alpha * socle->_dpoid[i];
	}
};

void optimiser_pertes(Mdl_t * mdl, uint N) {
	float sec = fabs(moyenne_seconds(mdl, rand() % MAX_DEPART, 5));
	float alpha = (sec > 0 ? 1.0/sec : 1.0);
	//
	uint S = 1;

	for (uint p=0; p < N; p++) {
		mdl_P_retropropa_gradient_session(mdl, S, rand() % 1000);
		//
		for (uint i=0; i < socle->poids; i++)
			socle->_poid[i] += alpha * socle->_dpoid[i];
	}
};