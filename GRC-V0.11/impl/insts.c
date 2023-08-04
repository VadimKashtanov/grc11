#include "insts.h"

Inst_t * INST[INSTS] = {
	&logistic1,
	&tanh1,
	&gauss1,
	&logabs1,
	&axb1,
	&axbxc2,
	&axbxcxd3
};

//	Ceux qui ont 1 seul dar et autant de poid qu'on veut
Inst_t * INST_1DAR[INSTS] = {};

void generer_les_instructions() {
	INSTS_1DAR = 0;
	//
	for (uint i=0; i < INSTS; i++) {
		if (INST[i]->dar == 1) {
			INST_1DAR[INSTS_1DAR] = INST[i];
			INSTS_1DAR++;
		}
	};
};
