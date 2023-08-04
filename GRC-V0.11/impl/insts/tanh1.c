#include "mdl.h"

float tanh1_f(float * dar, float * poid) {return tanh(dar[0]);};
void tanh1_df(float * dar, float * poid, float grad, float * d_dar, float * d_poid) {
	d_dar[0] += grad * d_tanh(dar[0]);
};
Inst_t tanh1 = {
	tanh1_f, tanh1_df,
	1, //dar
	0, //poids
	"tanh1"
};
