#include "mdl.h"

float gauss1_f(float * dar, float * poid) {
	return gauss(dar[0]);
};
void gauss1_df(float * dar, float * poid, float dy, float * d_dar, float * d_poid) {
	d_dar[0] += dy * d_gauss(dar[0]);
}

Inst_t gauss1 = {
	gauss1_f, gauss1_df,
	1, //dar
	0, //poids
	"gauss1"
};
