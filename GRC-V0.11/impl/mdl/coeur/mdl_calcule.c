#include "mdl.h"

void mdl_nouveau_jour(Mdl_t * mdl, uint jour, uint depart) {
	for (uint i=0; i < socle->dar_influents; i++) {
		socle->_var[jour*total + socle->influent[i]] = valeurs[(depart+jour)*INFOS_FOURNIE + socle->influent[i]];
	}
};

void mdl_f(Mdl_t * mdl, uint jour) {
	float null_flt_ptr[] = {0};
	//
	float entree[max_dar];
	//
	uint inst, temps, __i;
	for (uint i=socle->dar_influents; i < socle->influents; i++) {
		__i = socle->influent[i];
		
		inst = mdl->neurone_inst[__i];

		//	Calcule des connections
		for (uint j=0; j < INST[inst]->dar; j++) {
			temps = mdl->conn_temps[__i][j];
			//
			if (jour > 0) {
				entree[j] = INST_1DAR[ mdl->conn_inst[__i][j] ]->f(
					socle->_var + (jour-temps)*total + mdl->conn[__i][j],
					socle->_poid + socle->conn_poid_depart[__i][j]				);
			} else {
				entree[j] = INST_1DAR[ mdl->conn_inst[__i][j] ]->f(
					(temps == 1 ? null_flt_ptr : socle->_var + mdl->conn[__i][j]),
					socle->_poid + socle->conn_poid_depart[__i][j]
				);
			}
		};

		//	Calcule des neurones
		socle->_var[jour*total + __i] = INST[inst]->f(
			entree,
			socle->_poid + socle->neurone_poid_depart[__i]
		);
	};
};

void mdl_df(Mdl_t * mdl, uint jour) {
	float null_flt_ptr[] = {0};
	//
	float entree[max_dar];
	float d_entree[max_dar];
	//
	uint inst, temps, __i;
	for (int i=socle->influents-1; i >=0; i--) {
		__i = socle->influent[i];
		
		inst = mdl->neurone_inst[__i];

		//	Propagation connections
		for (uint j=0; j < INST[inst]->dar; j++) {
			temps = mdl->conn_temps[__i][j];
			//
			if (jour > 0) {
					entree[j] = INST_1DAR[ mdl->conn_inst[__i][j] ]->f(
					socle->_var + (jour-temps)*total + mdl->conn[__i][j],
					socle->_poid + socle->conn_poid_depart[__i][j]
				);
			} else {
				entree[j] = INST_1DAR[ mdl->conn_inst[__i][j] ]->f(
					(temps == 1 ? null_flt_ptr : socle->_var + mdl->conn[__i][j]),
					socle->_poid + socle->conn_poid_depart[__i][j]
				);
			}
		};

		//	Gradient du neurone
		memset(d_entree, 0, sizeof(float) * max_dar);
		INST[inst]->df(
			entree,
			socle->_poid + socle->neurone_poid_depart[__i],
			//
			socle->_grad[jour*total + __i],
			//
			d_entree,
			socle->_dpoid + socle->neurone_poid_depart[__i]
		);

		//	Propagation connections
		for (uint j=0; j < INST[inst]->dar; j++) {
			temps = mdl->conn_temps[__i][j];
			//
			if (jour > 0) {
				INST_1DAR[ mdl->conn_inst[__i][j] ]->df(
					socle->_var + (jour-temps)*total + mdl->conn[__i][j],
					socle->_poid + socle->conn_poid_depart[__i][j],
					//
					d_entree[j],
					//
					socle->_grad + (jour-temps)*total + mdl->conn[__i][j],
					socle->_dpoid + socle->conn_poid_depart[__i][j]
				);
			} else {
				if (temps == 0) {
					INST_1DAR[ mdl->conn_inst[__i][j] ]->df(
						socle->_var + mdl->conn[__i][j],
						socle->_poid + socle->conn_poid_depart[__i][j],
						//
						d_entree[j],
						//
						socle->_grad + mdl->conn[__i][j],
						socle->_dpoid + socle->conn_poid_depart[__i][j]
					);
				}
			}
		};
	};
};