#include "mdl.h"

void lier_le_socle(Mdl_t * mdl) {
	//	Actifs
	socle->actifs = 0;
	for (uint i=0; i < total; i++) if (mdl->neurone_inst[i] >= 0) socle->actifs++;
	//socle->actif = calloc(socle->actifs, sizeof(uint));
	uint tmp = 0;
	for (uint i=0; i < total; i++)
		if (mdl->neurone_inst[i] >= 0)
			socle->actif[tmp++] = i;
	
	//	Influents
	uint liee[total]; //{0:disfluent, 1:influent, 2:? qlq connections non verifiees}
	uint liee_connection[total][max_dar]; //{0:non-verifiee, 1:verifiee}
	memset(liee, 0, sizeof(uint) * total);
	memset(liee_connection, 0, sizeof(uint) * total * max_dar);
	//
	// 	Liee agit comme une liste d'attente.
	//On initialise l'attente avec tous les exi.
	for (uint exi=0; exi < EXI; exi++) {
		liee[DAR + KER + exi] = 2;
	}

	//
	uint fin, inst, pos;
	while (1) {
		//	Verifier que tous les influents sont terminee
		fin = 1;
		for (uint i=0; i < socle->actifs; i++) {
			pos = socle->actif[i];
			inst = mdl->neurone_inst[pos];// - 1;
			if (liee[pos] == 2) {
				fin = 0;
				//
				for (uint j=0; j < INST[inst]->dar; j++) {
					if (!(liee[mdl->conn[pos][j]] > 0)) {
						//printf("%i\n", mdl->neurone_inst[mdl->conn[pos][j]]);
						//if (mdl->neurone_inst[mdl->conn[pos][j]] > 0) {
						liee[mdl->conn[pos][j]] = 2;
						//} else {
						//	liee[mdl->conn[pos][j]] = 1;
						//}
						//printf("%i\n", mdl->conn[pos][j]);
					}
				}
				liee[pos] = 1;
				//
				break;
			}
		}
		if (fin == 1) break;
	}

	socle->influents = 0;
	for (uint i=0; i < total; i++)
		if (liee[i] == 1)
			socle->influents++;
	tmp = 0;
	for (uint i=0; i < total; i++) {
		if (liee[i] == 1) {
			socle->influent[tmp++] = i;
			socle->influent_oui_non[i] = 1;
		} else {
			socle->influent_oui_non[i] = 0;
		}
	}

	//
	socle->dar_influents = 0;
	for (uint i=0; i < socle->influents; i++) {
		if (socle->influent[i] < DAR) socle->dar_influents++;
	}
	socle->ker_influents = socle->influents - socle->dar_influents - EXI;

	// ===
	uint pos_reele;
	socle->poids = 0;
	for (uint i=socle->dar_influents; i < socle->influents; i++) {
		pos_reele = socle->influent[i];
		//
		inst = mdl->neurone_inst[pos_reele];
		tmp = INST[inst]->dar;
		//
		socle->neurone_poid_depart[pos_reele] = socle->poids;
		socle->poids += INST[inst]->poids;
		//
		for (uint j=0; j < tmp; j++) {
			socle->conn_poid_depart[pos_reele][j] = socle->poids;
			socle->poids += INST_1DAR[mdl->conn_inst[pos_reele][j]]->poids;
		};
	};
};