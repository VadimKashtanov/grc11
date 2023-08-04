#include "mdl.h"

float logabs1_f(float * dar, float * poid) {
	return logf(0.0001 + fabs(dar[0]));
};
void logabs1_df(float * dar, float * poid, float dy, float * d_dar, float * d_poid) {
	d_dar[0] += dy * 1.0 / (0.0001 + fabs(dar[0])) * ((dar[0] >= 0) ? 1.0 : -1.0);
}

Inst_t logabs1 = {
	logabs1_f, logabs1_df,
	1, //dar
	0, //poids
	"logabs1"
};