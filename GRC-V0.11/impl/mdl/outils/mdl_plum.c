#include "mdl.h"

static void plume_connection(uint inst, float * depart_p, uint dar, uint temps) {
	printf(" \033[45;93m");
	printf("%s(%i[%i])", INST_1DAR[inst]->nom, dar, -temps);
	//
	uint poids = INST_1DAR[inst]->poids;
	if (poids > 0) {
		printf("[");
		for (uint i=0; i < poids; i++) printf("p%i=%.3g%s", i, *(depart_p + i), (i==poids-1 ? "" : ", "));
		printf("]");
	}
	printf("\033[0m ");
};

static void plume_neurone(uint inst, float * depart_p) {
	JAUNE_GRAS("%s", INST[inst]->nom);
	//
	uint poids = INST[inst]->poids;
	if (poids > 0) {
		printf("[");
		for (uint i=0; i < poids; i++) printf("p%i=%.3g%s", i, *(depart_p + i), (i==poids-1 ? "" : ","));
		printf("]");
	}
};

static void plume_neurone_et_args(uint inst, uint i, Mdl_t * mdl) {
	printf("%i| ", i);
	inst = mdl->neurone_inst[i];
	//
	plume_neurone(inst, socle->_poid + socle->neurone_poid_depart[i]);
	printf(" {");
	for (uint j=0; j < INST[inst]->dar; j++)
		plume_connection(
			mdl->conn_inst[i][j],
			socle->_poid + socle->conn_poid_depart[i][j],
			mdl->conn[i][j], mdl->conn_temps[i][j]);
	printf("}\n");
};

static void plume_mdl(Mdl_t * mdl) {
	printf("========== Mdl ==========\n");

	//
	printf("DARI: ");
	for (uint i=0; i < DAR; i++)
		if (socle->influent_oui_non[i])
			printf("%i| ", i);
	printf("\n");

	//
	printf("KERDI:\n");
	for (uint i=DAR; i < DAR+KER; i++)
		if (socle->influent_oui_non[i])
			plume_neurone_et_args(mdl->neurone_inst[i], i, mdl);
	
	//
	printf("EXII:\n");
	for (uint i=DAR+KER; i < total; i++)
		plume_neurone_et_args(mdl->neurone_inst[i], i, mdl);

	//
	printf("=========================\n");
};

void plum_mdl_socle(Mdl_t * mdl) {
	lier_le_socle(mdl);
	//
	plume_mdl(mdl);
};