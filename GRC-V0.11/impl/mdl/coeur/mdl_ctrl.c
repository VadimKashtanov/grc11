#include "mdl.h"

void aleatoire_poids() {
	for (uint i=0; i < socle->poids; i++) {
		socle->_poid[i] = (2.0*rnd()-1.0);
	}
};
