#pragma once

#include "marchee.h"
#include "insts.h"

#define KER 12
#define total (DAR+KER+EXI)

typedef struct {//Socle_t {
	float _var[total * TAILLE_SESSION];
	float _grad[total * TAILLE_SESSION];
	//
	float _poid[(KER+EXI)*(max_poid + max_dar*max_poid)];
	float _dpoid[(KER+EXI)*(max_poid + max_poid*max_poid)];
	
    //==============================

	//	Il peut y avoire plusieurs poids par instruction
	//	Depart des poids pour chaque neurone
	uint neurone_poid_depart[total];		//pos dans _poid
	uint conn_poid_depart[total][max_dar];	//pos dans _poid
	//
	uint actifs;		//	Ceux qui ne sont pas neutre
	uint actif[total];	//
	//
	uint influents;			//	Ceux qui sont sur le chemin des EXI
	uint dar_influents;		//	Cmb de dar sont influents
	uint ker_influents;		//	Cmb de ker sont influents
	//
	uint influent_oui_non[total];	//	0;1
	uint influent[total];			//	pos dans neurone_inst

	/*	Poids  */
	uint poids;
} Socle_t;

Socle_t __socle;
Socle_t * socle;

typedef struct mdl_t {
	uint neurone_inst[total];
	uint conn[total][max_dar];
	uint conn_inst[total][max_dar];
	uint conn_temps[total][max_dar];
} Mdl_t;

//	Mem
Mdl_t * cree_mdl();
Mdl_t * copier_mdl(Mdl_t * mdl);

//	Associe le model au socle. Model pret a calculer.
void lier_le_socle(Mdl_t * mdl);

//	Controle
void aleatoire_poids();

// ====================== Reseaux Instruction ========================= 

void mdl_nouveau_jour(Mdl_t * mdl, uint jour, uint depart);
void mdl_f(Mdl_t * mdl, uint jour);
void mdl_df(Mdl_t * mdl, uint jour);

// ====================================================================

//  G(x) : gain = positif + negatif
float mdl_gain(Mdl_t * mdl, uint depart);
float mdl_gain_sessions(Mdl_t * mdl, uint S);
float mdl_gain_sessions_a_partire(Mdl_t * mdl, uint S, uint depart);
float mdl_gain_sessions_jusqua(Mdl_t * mdl, uint S, uint jusqua);

//  P(x) : perte = negatif 
float mdl_pertes(Mdl_t * mdl, uint depart);
float mdl_pertes_sessions(Mdl_t * mdl, uint S);
float mdl_pertes_sessions_a_partire(Mdl_t * mdl, uint S, uint depart);
float mdl_pertes_sessions_jusqua(Mdl_t * mdl, uint S, uint jusqua);

// =============== Gradient des Fonction de Score ====================

//	[Methode 1e-5]::G'(x)
void mdl_G_mdt1e5_gradient(Mdl_t * mdl, uint depart);
void mdl_G_mdt1e5_gradient_session(Mdl_t * mdl, uint S, uint graine);
//  [Retro-Propagation]::G'(x)
void mdl_G_retropropa_gradient(Mdl_t * mdl, uint depart);
void mdl_G_retropropa_gradient_session(Mdl_t * mdl, uint S, uint graine);

//	[Methode 1e-5]::P'(x)
void mdl_P_mdt1e5_gradient(Mdl_t * mdl, uint depart);
void mdl_P_mdt1e5_gradient_session(Mdl_t * mdl, uint S, uint graine);
//  [Retro-Propagation]::P'(x)
void mdl_P_retropropa_gradient(Mdl_t * mdl, uint depart);
void mdl_P_retropropa_gradient_session(Mdl_t * mdl, uint S, uint graine);

//  ====================== Outils =============================

//	G''(x)
float moyenne_seconds(Mdl_t * mdl, uint depart, uint N);
float moyenne_diag(Mdl_t * mdl, uint depart, uint N);
float* tableau_second(Mdl_t * mdl, uint depart);

//	Outils d'Analyse Manuelle
Mdl_t * generique();
void plum_mdl_socle(Mdl_t * mdl); 

//	Verification des instructions
void comparer_mdt1e5_retro_propa_pertes(Mdl_t * mdl);
void comparer_mdt1e5_retro_propa_gains(Mdl_t * mdl);