#include "mdl.h"

//=======================================================================

void mdl_G_retropropa_gradient(Mdl_t * mdl, uint depart) {
	float * grad = socle->_grad;
	//
	float usdt_lst[TAILLE_SESSION];
	float usdt = DEPART_SESSION;
	float pourcent, levier, p0, p1;
	//
	//	Propagation
	//
	for (uint j=0; j < TAILLE_SESSION; j++) {
		//
		memset(grad + j*total, 0, sizeof(float) * total);
		//
		usdt_lst[j] = usdt;
		mdl_nouveau_jour(mdl, j, depart);
		mdl_f(mdl, j);
		//
		pourcent = tanh(socle->_var[(j+1)*total - 2]);
		levier = 1+(LEVIER_MAX-1)/(1.0 + expf(-socle->_var[(j+1)*total - 1]));
		p0 = prixs[depart + j];
		p1 = prixs[depart + j+1];
		//
		usdt += usdt*pourcent*levier*(p1/p0 - 1);
	}

	float dscore = ((usdt) > 0) ? 1 : -1;
	
	//
	//	Retro Propagation
	//
	float dusdt, dpourcent, dlevier, dfx;
	//
	dusdt = dscore;
	for (int j=TAILLE_SESSION-1; j >= 0; j--) {
		pourcent = tanh(socle->_var[(j+1)*total - 2]);
		levier = 1+(LEVIER_MAX-1)/(1.0 + expf(-socle->_var[(j+1)*total - 1]));
		p0 = prixs[depart + j];
		p1 = prixs[depart + j+1];
		//
		//usdt = usdt + usdt*f(x)*(p1/p0 - 1)
		//usdt = usdt + usdt*pourcent*levier*(p1/p0 - 1));
		//
		dfx = dusdt * usdt_lst[j]*(p1/p0 - 1.0);
		//
		dpourcent = (1 - pourcent*pourcent) * levier * dfx;
		dlevier = (levier-1) * (LEVIER_MAX-1 - (levier-1)) * pourcent * dfx/(LEVIER_MAX-1);
		dusdt *= (1 + pourcent*levier*(p1/p0 - 1));
		//
		grad[j*total + total - 1] = dlevier;
		grad[j*total + total - 2] = dpourcent;
		//
		//printf("%f\n", dusdt);
		//
		mdl_df(mdl, j);
	};
};

void mdl_G_retropropa_gradient_session(Mdl_t * mdl, uint S, uint graine) {
	memset(socle->_dpoid, 0, sizeof(float) * socle->poids);
	//
	for (uint i=0; i < S; i++) {
		srand(graine + i + 1);
		mdl_G_retropropa_gradient(mdl, rand() % MAX_DEPART);
	}
	for (uint i=0; i < socle->poids; i++) socle->_dpoid[i] /= S;
};

//========================================================================
//========================================================================
//========================================================================

void mdl_P_retropropa_gradient(Mdl_t * mdl, uint depart) {
	float * grad = socle->_grad;
	//
	float usdt_lst[TAILLE_SESSION];
	float usdt = DEPART_SESSION;
	float pourcent, levier, p0, p1, changement;
	//
	//	Propagation
	//
	for (uint j=0; j < TAILLE_SESSION; j++) {
		//
		memset(grad + j*total, 0, sizeof(float) * total);
		//
		usdt_lst[j] = usdt;
		mdl_nouveau_jour(mdl, j, depart);
		mdl_f(mdl, j);
		//
		pourcent = tanh(socle->_var[(j+1)*total - 2]);
		levier = 1+(LEVIER_MAX-1)/(1.0 + expf(-socle->_var[(j+1)*total - 1]));
		p0 = prixs[depart + j];
		p1 = prixs[depart + j+1];
		//
		changement = usdt*pourcent*levier*(p1/p0 - 1);
		if (changement < 0)
			usdt += changement;
	}

	float dscore = ((usdt) > 0) ? 1 : -1;
	
	//
	//	Retro Propagation
	//
	float dusdt, dpourcent, dlevier, dfx;
	//
	uint perte;
	//
	dusdt = dscore;
	for (int j=TAILLE_SESSION-1; j >= 0; j--) {
		pourcent = tanh(socle->_var[(j+1)*total - 2]);
		levier = 1+(LEVIER_MAX-1)/(1.0 + expf(-socle->_var[(j+1)*total - 1]));
		p0 = prixs[depart + j];
		p1 = prixs[depart + j+1];
		//
		perte = (pourcent*levier*(p1/p0 - 1) < 0);
		//if (pourcent == 0) perte = 0;//(p0==p1);
		//else perte = (pourcent / fabs(pourcent)) == ((p1/p0-1)/fabs(p1/p0-1));
		//
		//changement = usdt*f(x)*(p1/p0 - 1)
		//changement = usdt*pourcent*levier*(p1/p0 - 1));
		//
		//if changement < 0: usdt = usdt + changement;
		//else : usdt = usdt + 0
		//
		dfx = dusdt * usdt_lst[j]*(p1/p0 - 1.0) * perte;
		//
		dpourcent = (1 - pourcent*pourcent) * levier * dfx;
		dlevier = (levier-1) * (LEVIER_MAX-1 - (levier-1)) * pourcent * dfx/(LEVIER_MAX-1);
		dusdt *= (1 + pourcent*levier*(p1/p0 - 1)*perte);
		//
		grad[j*total + total - 1] = dlevier;
		grad[j*total + total - 2] = dpourcent;
		//
		if (perte)
			mdl_df(mdl, j);
	};
};

void mdl_P_retropropa_gradient_session(Mdl_t * mdl, uint S, uint graine) {
	memset(socle->_dpoid, 0, sizeof(float) * socle->poids);
	//
	for (uint i=0; i < S; i++) {
		srand(graine + i + 1);
		mdl_P_retropropa_gradient(mdl, rand() % MAX_DEPART);
	}
	for (uint i=0; i < socle->poids; i++) socle->_dpoid[i] /= S;
};