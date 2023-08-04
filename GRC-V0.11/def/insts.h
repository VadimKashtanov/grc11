#pragma once

#include "etc.h"

typedef float (*f_t)(
	float * dar, float * poid);
typedef void (*df_t)(
	float * dar, float * poids,
	float dy, float * d_dar, float * d_poid);

typedef struct {
	f_t f;
	df_t df;
	//
	uint dar;	//	Chaque instruction a `dar` entrees
	uint poids;	//	Chaque instruction possede N poids
	//
	char * nom;
} Inst_t;

//	Nb max Entree et Poids
#define max_dar 3
#define max_poid 4

//	Un neurone peut avoire >1 entrees
//	Une connection peut avoire 1 entree
#define INSTS 7
uint INSTS_1DAR;
Inst_t * INST[INSTS];
Inst_t * INST_1DAR[INSTS];

void generer_les_instructions();

//
Inst_t logistic1;
Inst_t tanh1;
Inst_t gauss1;
Inst_t logabs1;
Inst_t axb1;
Inst_t axbxc2;
Inst_t axbxcxd3;

#define LOGISTIQUE 0
#define TANH 1
#define GAUSS 2
#define LOGABS 3
#define AXB 4
#define AXBXC 5
#define AXBXCXD 6

#define NEU1 AXB
#define NEU2 AXBXC
#define NEU3 AXBXCXD 
