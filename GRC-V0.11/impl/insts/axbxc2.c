#include "mdl.h"

float axbxc2_f(float * dar, float * poid) {return dar[0]*poid[0] + dar[1]*poid[1] + poid[2];};
void axbxc2_df(float * dar, float * poid, float dy, float * d_dar, float * d_poid) {
	d_dar[0] += dy * poid[0];
	d_poid[0] += dy * dar[0] / TAILLE_SESSION;
	d_dar[1] += dy * poid[1];
	d_poid[1] += dy * dar[1] / TAILLE_SESSION;
	d_poid[2] += dy / TAILLE_SESSION;
}

Inst_t axbxc2 = {
	axbxc2_f, axbxc2_df,
	2, //dar
	3, //poids
	"axbxc2"
};
