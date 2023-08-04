#include "mdl.h"

float axbxcxd3_f(float * dar, float * poid) {return dar[0]*poid[0] + dar[1]*poid[1] + dar[2]*poid[2] + poid[3];};
void axbxcxd3_df(float * dar, float * poid, float dy, float * d_dar, float * d_poid) {
	d_dar[0] += dy * poid[0];
	d_poid[0] += dy * dar[0] / TAILLE_SESSION;
	//
	d_dar[1] += dy * poid[1];
	d_poid[1] += dy * dar[1] / TAILLE_SESSION;
	//
	d_dar[2] += dy * poid[2];
	d_poid[2] += dy * dar[2] / TAILLE_SESSION;
	//
	d_poid[3] += dy / TAILLE_SESSION;
}

Inst_t axbxcxd3 = {
	axbxcxd3_f, axbxcxd3_df,
	3, //dar
	4, //poids
	"axbxcxd3"
};
