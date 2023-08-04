#include "selection.h"

Peuple_t * cree_peuple(Mdl_t * mdl,
	uint tribue_enfants, uint tribue_gagants,
	uint peuple_enfants, uint peuple_gagants)
{
	Peuple_t * ret = malloc(sizeof(Peuple_t));
	//
	ret->N = peuple_gagants * (1+peuple_enfants);
	ret->tribue = malloc(sizeof(Petite_Tribue_t) * ret->N);
	for (uint i=0; i < ret->N; i++)
		ret->tribue[i] = cree_petite_tribue(tribue_enfants, tribue_gagants, mdl);
	//
	ret->score_gain = malloc(sizeof(float) * ret->N);
	ret->score_pertes = malloc(sizeof(float) * ret->N);
	ret->podium = malloc(sizeof(uint) * ret->N);
	ret->points = malloc(sizeof(uint) * ret->N);
	//
	ret->enfants = peuple_enfants;
	ret->gagants = peuple_gagants;
	//
	ret->tribues_enfants = tribue_enfants;
	ret->tribues_gagantes = tribue_gagants;
	//
	return ret;
};

void liberer_peuple(Peuple_t * peuple) {
	for (uint i=0; i < peuple->N; i++)
		liberer_petite_tribue(peuple->tribue[i]);
	free(peuple->tribue);
	//
	free(peuple->score_gain);
	free(peuple->score_pertes);
	//
	free(peuple);
};

//	===========================================================

void peuple_meilleur_score(Peuple_t * peuple) {
	Petite_Tribue_t * tmpt = malloc(sizeof(Petite_Tribue_t));
	tmpt->models = malloc(sizeof(Mdl_t*) * peuple->N);
	for (uint i=0; i < peuple->N; i++) {
		tmpt->models[i] = malloc(sizeof(Mdl_t));
		memcpy(
			tmpt->models[i],
			peuple->tribue[i]->models[peuple->tribue[i]->podium[0]],
			sizeof(Mdl_t));
	}
	//
	tmpt->score_gain = malloc(sizeof(float) * peuple->N);
	tmpt->score_pertes = malloc(sizeof(float) * peuple->N);
	tmpt->points = malloc(sizeof(uint) * peuple->N);
	tmpt->podium = malloc(sizeof(uint) * peuple->N);
	tmpt->N = peuple->N;
	petite_tribue_scores(tmpt);
	//
	memcpy(peuple->score_gain, tmpt->score_gain, sizeof(float) * peuple->N);
	memcpy(peuple->score_pertes, tmpt->score_pertes, sizeof(float) * peuple->N);
	memcpy(peuple->points, tmpt->points, sizeof(float) * peuple->N);
	memcpy(peuple->podium, tmpt->podium, sizeof(float) * peuple->N);
	//
	liberer_petite_tribue(tmpt);
};

void peuple_vie_interne_tribue(Peuple_t * peuple) {
	for (uint i=0; i < peuple->N; i++) {
		petite_tribue_scores(peuple->tribue[i]);
		petite_tribue_mutations(peuple->tribue[i]);
	}
};

void peuple_nouvelles_tribues_mutee(Peuple_t * peuple) {
	Mdl_t * geniteurs[peuple->N];
	//
	for (uint i=0; i < peuple->N; i++) {
		geniteurs[i] = malloc(sizeof(Mdl_t));
		memcpy(
			geniteurs[i],
			peuple->tribue[i]->models[peuple->tribue[i]->podium[0]],
			sizeof(Mdl_t));
		liberer_petite_tribue(peuple->tribue[i]);
	}
	//
	for (uint i=0; i < peuple->gagants; i++) {
		uint gagant = peuple->podium[i];
		//
		for (uint j=0; j < peuple->enfants; j++) {
			for (uint k=DAR; k < total; k++) {
				uint enfant = peuple->podium[
					peuple->gagants + i*peuple->enfants + k];
				//
				if (rnd() > 0.5) {
					geniteurs[enfant]->neurone_inst[k] = geniteurs[gagant]->neurone_inst[k];
					memcpy(
						geniteurs[enfant]->conn[k],
						geniteurs[gagant]->conn[k],
						sizeof(uint) * max_dar);
					memcpy(
						geniteurs[enfant]->conn_inst[k],
						geniteurs[gagant]->conn_inst[k],
						sizeof(uint) * max_dar);
					memcpy(
						geniteurs[enfant]->conn_temps[k],
						geniteurs[gagant]->conn_temps[k],
						sizeof(uint) * max_dar);
				}
			}
		};
	};
	for (uint i=0; i < peuple->N; i++) {
		peuple->tribue[i] = cree_petite_tribue(
			peuple->tribues_enfants,
			peuple->tribues_gagantes,
			geniteurs[i]);
		free(geniteurs[i]);
	};
};