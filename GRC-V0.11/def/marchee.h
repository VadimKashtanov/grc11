#pragma once

#include "etc.h"

#define TAILLE_SESSION 50		//50 heures
#define DEPART_SESSION 100.0 	//depart a 100$

#define LEVIER_MAX 20.0

//<Estimation Score>
//	//	Apprend avant une certaine Date
//	jusque = % PRIXS-1;
//	dernier_depart = jusque - TAILLE_SESSION
//	boucle n dans 0..N
//		optimiser(dernier_depart - (%dernier_depart))
//	//	On estime un score apres la certaine date
//	retour estimer_score(apres)
//	//	De cette maniere le modele ne c'est jamais
//	//	entrainnee sur la zone sur laquelle il est teste

uint PRIXS;
float * prixs;
uint PRIXS_VALIDES;	//dans les infos il y a moyenn-100jours
uint INFOS_FOURNIE;
uint INFOS;	//=PRIXS_VALIDES*INFOS_FOURNIE
float * valeurs;	//normaliser les prixs

//uint DAR, EXI; typedef socle utilise macro DAR
#define DAR 60
#define EXI 2

void charger_les_prixs();
void liberer_les_prixs();

#define MAX_DEPART (PRIXS_VALIDES - 1 - TAILLE_SESSION)