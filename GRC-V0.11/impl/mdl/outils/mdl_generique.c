#include "mdl.h"

#define DER (DAR+KER+EXI-1)

static void ajout_couche(
	Mdl_t * mdl, uint * couche, uint elements,
	uint prendre_de,
	uint depart)
{
	for (uint i=0; i < elements; i++) {
		mdl->neurone_inst[depart + i] = couche[i];
		//
		for (uint j=0; j < INST[ couche[i] ]->dar; j++) {
			mdl->conn[depart + i][j] = prendre_de++;
			mdl->conn_temps[depart + i][j] = 0;//rand() % 2;
			mdl->conn_inst[depart + i][j] = rand() % INSTS_1DAR;
		}
	};
};

Mdl_t * generique() {
	Mdl_t * mdl = cree_mdl();
	
	//	Sortie
	uint structure[4][9] = {
		{AXBXCXD, AXBXCXD, AXBXCXD, AXBXCXD, AXBXCXD, AXBXCXD, AXBXCXD, AXBXCXD, AXBXCXD},
		{AXBXCXD, AXBXCXD, AXBXCXD, 0, 0, 0, 0, 0, 0},
		{AXBXCXD, 0, 0, 0, 0, 0, 0, 0, 0},
		{AXBXCXD, 0, 0, 0, 0, 0, 0, 0, 0}
	};

	//ajout_couche(mdl, structure[0], 9, 0, 				total-EXI-3-9);
	//ajout_couche(mdl, structure[1], 3, total-EXI-3-9, 	total-EXI-3);
	//ajout_couche(mdl, structure[2], 1, total-EXI-3, 	total-EXI);
	//ajout_couche(mdl, structure[3], 1, total-EXI-3, 	total-EXI+1);

	ajout_couche(mdl, structure[0], 9, 0, 				total-EXI-3-9);
	ajout_couche(mdl, structure[1], 3, 50,			 	total-EXI-3);
	ajout_couche(mdl, structure[2], 1, total-EXI-3, 	total-EXI);
	ajout_couche(mdl, structure[3], 1, total-EXI-3, 	total-EXI+1);

	// + verifier que charger_les_prix calcule bien les moyennes
	return mdl;
};