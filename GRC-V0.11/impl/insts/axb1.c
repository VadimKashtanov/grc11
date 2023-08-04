#include "mdl.h"

float axb1_f(float * dar, float * poid) {return dar[0]*poid[0] + poid[1];};
void axb1_df(float * dar, float * poid, float dy, float * d_dar, float * d_poid) {
	d_dar[0] += dy*poid[0];
	d_poid[0] += dy*dar[0] / TAILLE_SESSION;
	d_poid[1] += dy / TAILLE_SESSION;
}
Inst_t axb1 = {
	axb1_f, axb1_df,
	1, //dar
	2, //poids
	"axb1"
};