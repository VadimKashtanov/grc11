#pragma once

#include "mdl.h"

#define GAIN_MOYEN 0
#define ANTI_PERTE 1

float mdl_moyenne_score_aveugle(
	Mdl_t * mdl,    // 
	uint S,         //
	uint N,         //
	uint T,         //
	float alpha,    //
	uint tours,		// Cmb de frontieres differentes
    uint mode       // gain_moyen / anti_perte
);

float estimer_score(Mdl_t * mdl, uint mode);

//	=====================================

void optimiser_gains(Mdl_t * mdl, uint N);
void optimiser_pertes(Mdl_t * mdl, uint N);