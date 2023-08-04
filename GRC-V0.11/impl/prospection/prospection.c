#include "prospection.h"

static float* mdl_gain_prospection(Mdl_t * mdl, uint depart) {
	float usdt = DEPART_SESSION;
	float pourcent, levier, p0, p1;
	float changement;
	//
	float * pl = malloc(sizeof(float) * TAILLE_SESSION);
	//
	for (uint i=0; i < TAILLE_SESSION; i++)
	{
		mdl_nouveau_jour(mdl, i, depart);
		mdl_f(mdl, i);
		//
		pourcent = tanh(socle->_var[(i+1)*total - 2]);
		levier = 1+(LEVIER_MAX-1)/(1.0 + expf(-socle->_var[(i+1)*total - 1]));
		p0 = prixs[depart + i];
		p1 = prixs[depart + i+1];
		//
		changement = usdt*pourcent*levier*(p1/p0 - 1);
		usdt += changement;
		//
		pl[i] = pourcent * levier;
	}
	//
	return pl;
};

static void ecrire_toutes_variables_1_jour(Mdl_t * mdl, uint depart) {
	uint t = TAILLE_SESSION;
	if (t > 10) t = 10;
	//
	float _variables_influentes_socle[t][socle->influents];
	//
	for (uint i=0; i < t; i++)
	{
		mdl_nouveau_jour(mdl, i, depart);
		mdl_f(mdl, i);
		//
		for (uint j=0; j < socle->influents; j++) {
			_variables_influentes_socle[i][j] = socle->_var[i*total + socle->influent[j]];
		}
	}
	//
	printf("Depart a %i de t=0 a t=%i.\n   ", depart, t);
	for (uint i=0; i < t; i++) printf("   t=%i    |", i);
	printf("\n");
	printf("DARI:\n");
	for (uint i=0; i < socle->influents; i++) {
		if (i == socle->dar_influents) printf("KERDI:\n");
		if (i == socle->dar_influents+socle->ker_influents) printf("EXI:\n");
		//
		printf("%3.i| ", socle->influent[i]);
		for (uint _t=0; _t < t; _t++) {
			if (_variables_influentes_socle[_t][i] >= 0) printf(" ");
			printf("%f,  ", //"%10.5g, ",
				_variables_influentes_socle[_t][i]);
		}
		printf("\n");
	}
};

void prospection(Mdl_t * mdl) {
	float * a[5] = {
		mdl_gain_prospection(mdl, rand() % MAX_DEPART),
		mdl_gain_prospection(mdl, rand() % MAX_DEPART),
		mdl_gain_prospection(mdl, rand() % MAX_DEPART),
		mdl_gain_prospection(mdl, rand() % MAX_DEPART),
		mdl_gain_prospection(mdl, rand() % MAX_DEPART)
	};
	gnuplots_titre(
		5,
		TAILLE_SESSION,
		a,
		"Des sessions a departs aleatoires. Courbes : pourcent*lever");
	

	//	========================
	ecrire_toutes_variables_1_jour(mdl, rand() % MAX_DEPART);
};

//le generique bouge pas et bougera pas
//toujoura du .99 ou .01 eventuellement