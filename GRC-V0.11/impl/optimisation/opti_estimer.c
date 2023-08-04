#include "optimisation.h"

static float mdl_score_aveugle_GAIN(
	Mdl_t * mdl,
	uint S,     //nb session gradient moyen par entrainnement
	uint N,     //nb boucle entrainnement
	uint T,     //nb Tests resultat final
	float alpha)
{
	uint jusque = TAILLE_SESSION + rand() % (MAX_DEPART-TAILLE_SESSION-1);
	
	//	Le plus proche depart d'apprentissage. Apres c'est le domaine d'ignorance post-apprentissage
	uint dernier_depart = 1 + jusque - TAILLE_SESSION;

	for (uint n=0; n < N; n++) {
		memset(socle->_dpoid, 0, sizeof(float) * socle->poids);
		//
		for (uint i=0; i < S; i++)
			mdl_G_retropropa_gradient(mdl, rand() % dernier_depart);
		//
		for (uint i=0; i < socle->poids; i++)
			socle->_poid[i] += alpha*socle->_dpoid[i] / S;
	}

	//	Verifier que aucun poids n'est nan
	for (uint i=0; i < socle->poids; i++) {
		if (socle->_poid[i] != socle->_poid[i]) {
			aleatoire_poids();
			break;
		}
	}

	//	Score dans une zone non entrainnee
	float _gain = 0;
	for (uint i=0; i < T; i++) {
		_gain += mdl_gain(mdl, jusque + rand() % (MAX_DEPART-jusque-1));
	}
	return _gain / S;
}

static float mdl_score_aveugle_PERTE(
	Mdl_t * mdl,
	uint S,     //nb session gradient moyen par entrainnement
	uint N,     //nb boucle entrainnement
	uint T,     //nb Tests resultat final
	float alpha)
{
	uint jusque = TAILLE_SESSION + rand() % (MAX_DEPART-TAILLE_SESSION-1);
	
	//	Le plus proche depart d'apprentissage.
	//	Apres c'est le domaine d'ignorance post-apprentissage
	uint dernier_depart = 1 + jusque - TAILLE_SESSION;

	for (uint n=0; n < N; n++) {
		memset(socle->_dpoid, 0, sizeof(float) * socle->poids);
		//
		for (uint i=0; i < S; i++)
			mdl_P_retropropa_gradient(mdl, rand() % dernier_depart);
		//
		for (uint i=0; i < socle->poids; i++)
			socle->_poid[i] += alpha*socle->_dpoid[i] / S;
	}

	//	Verifier que aucun poids n'est nan
	for (uint i=0; i < socle->poids; i++) {
		if (socle->_poid[i] != socle->_poid[i]) {
			aleatoire_poids();
			break;
		}
	}

	//	Score dans une zone non entrainnee
	float _gain = 0;
	for (uint i=0; i < T; i++)
		_gain += mdl_pertes(mdl, jusque + rand() % (MAX_DEPART-jusque-1));
	return _gain / S;
}

float mdl_moyenne_score_aveugle(
	Mdl_t * mdl,    // 
	uint S,         //
	uint N,         //
	uint T,         //
	float alpha,    //
	uint tours,     // Cmb de frontieres differentes
	uint mode)       // gain_moyen / anti_perte
{
	float _s = 0;
	if (mode == GAIN_MOYEN) {
		for (uint i=0; i < tours; i++) {
			aleatoire_poids();
			_s += mdl_score_aveugle_GAIN(mdl, S, N, T, alpha);
		}
	} else if (mode == ANTI_PERTE) {
		for (uint i=0; i < tours; i++) {
			aleatoire_poids();
			_s += mdl_score_aveugle_PERTE(mdl, S, N, T, alpha);
		}
	}
	return _s / tours;
}

//  Estimation de Score
float estimer_score(Mdl_t * mdl, uint mode) {
	lier_le_socle(mdl);
	
	if (socle->poids == 0) ERR("Aucun poids, pas d'optimisations possibles");

	//  Approximation de l'hessienne
	uint _N = (uint)roundf(0.10*(float)socle->poids + 0.5);
	float moy_diag = moyenne_diag(mdl, rand() % MAX_DEPART, _N);
	float moy_sec = moyenne_seconds(mdl, rand() % MAX_DEPART, _N);
	float alpha;// = 1.0;
	//if (moy_sec != moy_diag)
	//	alpha = 1.0 / fabs(moy_diag - moy_sec);
	alpha = 1 / (moy_sec + moy_diag)/2;
	if (alpha != alpha || fabs(alpha) > 1e10) alpha = 1.0;

	//  Approximation Score
	uint S=5, N=20, T=50+50*(mode==GAIN_MOYEN), tours=3;
	return mdl_moyenne_score_aveugle(mdl, S, N, T, alpha, tours, mode);
};

//=============================================================