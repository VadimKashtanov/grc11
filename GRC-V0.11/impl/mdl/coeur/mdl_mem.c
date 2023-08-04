#include "mdl.h"

//	Mem
Mdl_t * cree_mdl() {
	return malloc(sizeof(Mdl_t));
};

Mdl_t * copier_mdl(Mdl_t * mdl) {
	Mdl_t * ret = cree_mdl();
	memcpy(ret, mdl, sizeof(Mdl_t)); 
	return ret;
};