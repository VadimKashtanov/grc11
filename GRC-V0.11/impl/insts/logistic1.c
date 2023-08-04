#include "mdl.h"

float logistic1_f(float * dar, float * poid) {return logistique(dar[0]);};
void logistic1_df(float * dar, float * poid, float dy, float * d_dar, float * d_poid) {
	d_dar[0] += dy * d_logistique(dar[0]);
};
Inst_t logistic1 = {
	logistic1_f, logistic1_df,
	1, //dar
	0, //poids
	"logistique1"
};
