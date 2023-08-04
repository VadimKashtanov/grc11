#pragma once

#include "optimisation.h"

typedef struct {
	uint enfants;
	uint gagants;
	//
	uint N;	//=(enfants + 1)*gagnants
	//
	float * score_gain;
	float * score_pertes;
	//
	uint * points;
	uint * podium;
	//
	Mdl_t ** models;
} Petite_Tribue_t;

Petite_Tribue_t * cree_petite_tribue(
	uint enfants, uint gagants, Mdl_t * depart);
void liberer_petite_tribue(Petite_Tribue_t * petite_tribue);
//
void petite_tribue_scores(Petite_Tribue_t * petite_tribue);
void petite_tribue_mutations(Petite_Tribue_t * petite_tribue);

Mdl_t * petite_tribue(Mdl_t * depart);

//	===========================================================

typedef struct {
	uint N;
	Petite_Tribue_t ** tribue;
	//
	float * score_gain;
	float * score_pertes;
	uint * points;
	uint * podium;
	//
	uint enfants;
	uint gagants;
	//
	uint tribues_enfants;
	uint tribues_gagantes;
} Peuple_t;

Peuple_t * cree_peuple(
	Mdl_t * mdl,
	uint tribue_enfants, uint tribue_gagants,
	uint peuple_enfants, uint peuple_gagants);
void liberer_peuple(Peuple_t * peuple);

void peuple_meilleur_score(Peuple_t * peuple);
void peuple_vie_interne_tribue(Peuple_t * peuple);
void peuple_nouvelles_tribues_mutee(Peuple_t * peuple);