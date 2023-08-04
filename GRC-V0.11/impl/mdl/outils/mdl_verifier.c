#include "mdl.h"

void comparer_mdt1e5_retro_propa_gains(Mdl_t * mdl) {
	lier_le_socle(mdl);
	//
	uint depart = rand() % MAX_DEPART;

	//
	memset(socle->_dpoid, 0, sizeof(float) * socle->poids);
	mdl_G_mdt1e5_gradient(mdl, depart);
	float _grad[socle->poids];
	memcpy(_grad, socle->_dpoid, sizeof(float) * socle->poids);
	
	//
	memset(socle->_dpoid, 0, sizeof(float) * socle->poids);
	mdl_G_retropropa_gradient(mdl, depart);

	//
	for (uint i=0; i < socle->poids; i++) {
		uint couleur = 92;
		if (fabs(_grad[i]-socle->_dpoid[i]) > 0.005) couleur = 91;
		//
		printf("%i| %f      %f     [\033[1;%im%f\033[0m]\n",
			i, _grad[i], socle->_dpoid[i],
			couleur, _grad[i]-socle->_dpoid[i]);
	}
	printf("        1e5     retro-propa\n");
};

void comparer_mdt1e5_retro_propa_pertes(Mdl_t * mdl) {
	lier_le_socle(mdl);
	//
	uint depart = rand() % MAX_DEPART;

	//
	memset(socle->_dpoid, 0, sizeof(float) * socle->poids);
	mdl_P_mdt1e5_gradient(mdl, depart);
	float _grad[socle->poids];
	memcpy(_grad, socle->_dpoid, sizeof(float) * socle->poids);
	
	//
	memset(socle->_dpoid, 0, sizeof(float) * socle->poids);
	mdl_P_retropropa_gradient(mdl, depart);

	//
	for (uint i=0; i < socle->poids; i++) {
		uint couleur = 92;
		if (fabs(_grad[i]-socle->_dpoid[i]) > 0.005) couleur = 91;
		//
		printf("%i| %f      %f     [\033[1;%im%f\033[0m]\n",
			i, _grad[i], socle->_dpoid[i],
			couleur, _grad[i]-socle->_dpoid[i]);
	}
	printf("        1e5      retro-propa\n");
};
