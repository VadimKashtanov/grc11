#include "selection.h"

static Mdl_t * cree_mutation(Mdl_t * A) {
	//juste les liens
	Mdl_t * ret = malloc(sizeof(Mdl_t));
	memcpy(ret, A, sizeof(Mdl_t));
	//
	lier_le_socle(A);
	//
	uint inst, __i;
	for (uint i=0; i < socle->influents; i++) {
		__i = socle->influent[i];
		//
		inst = A->neurone_inst[__i];
		//
		for (uint j=0; j < INST[inst]->dar; j++) {
			if (rnd() > 0.80) {
				uint conn = ret->conn[__i][j];
				uint derriere = conn;
				uint devant = total - conn - 1 - 2;
				//
				if (rnd() < (float)derriere/(float)(derriere+devant)) {
					uint derriere = conn;
					//
					float r = powf(rnd(), 5); //~= 0.16
					ret->conn[__i][j] -= (uint)roundf(r * derriere);
				} else {
					uint devant = total - conn - 1 - 2;
					//
					float r = powf(rnd(), 5); //~= 0.16
					ret->conn[__i][j] += (uint)roundf(r * devant);
				}
			};
			if (rnd() > 0.60) {
				ret->conn_inst[__i][j] = rand() % INSTS_1DAR;
			};
			//if (rnd() > 0.70) {
			//	if (ret->conn_temps[__i][j] == 0)
			//		ret->conn_temps[__i][j] = 1;
			//	else
			//		ret->conn_temps[__i][j] = 0;
			//};
		}
	};
	//
	return ret;
};

Petite_Tribue_t * cree_petite_tribue(
	uint enfants, uint gagants, Mdl_t * depart)
{
	Petite_Tribue_t * ret = malloc(sizeof(Petite_Tribue_t));
	//
	ret->enfants = enfants;
	ret->gagants = gagants;
	//
	ret->N = (enfants+1) * gagants;
	//
	ret->score_gain = malloc(sizeof(float) * ret->N);
	ret->score_pertes = malloc(sizeof(float) * ret->N);
	//
	ret->models = malloc(sizeof(Mdl_t*) * ret->N);
	ret->models[0] = copier_mdl(depart);
	for (uint i=1; i < ret->N; i++)
		ret->models[i] = cree_mutation(depart);
	//
	ret->points = malloc(sizeof(uint) * ret->N);
	ret->podium = malloc(sizeof(uint) * ret->N);
	//
	return ret;
};

void liberer_petite_tribue(Petite_Tribue_t * petite_tribue) {
	free(petite_tribue->score_gain);
	free(petite_tribue->score_pertes);
	free(petite_tribue->points);
	free(petite_tribue->podium);
	for (uint i=0; i < petite_tribue->N; i++) {
		//car c'est juste un grand espace alloue.
		//par de pointeurs ou quoi que se soit.
		//c'est juste un bloque qui a ete malloc() une fois
		free(petite_tribue->models[i]);
	}
	free(petite_tribue);
};

//

void petite_tribue_scores(Petite_Tribue_t * petite_tribue) {
	uint N = petite_tribue->N;
	for (uint i=0; i < N; i++) {
		lier_le_socle(petite_tribue->models[i]);
		//
		petite_tribue->score_pertes[i] = estimer_score(
			petite_tribue->models[i], ANTI_PERTE);
		petite_tribue->score_gain[i] = estimer_score(
			petite_tribue->models[i], GAIN_MOYEN);
	};
	//
	memset(petite_tribue->points, 0, sizeof(float) * N);
	//
	uint est_au_dessus_de;
	float gain, perte;
	for (uint i=0; i < N; i++) {
		est_au_dessus_de = 0;
		//anti nan
		gain = petite_tribue->score_gain[i];
		perte = petite_tribue->score_pertes[i];
		if (gain == gain && perte == perte) {
			for (uint j=0; j < N; j++)
				if (gain > petite_tribue->score_gain[j])
					est_au_dessus_de++;
			for (uint j=0; j < N; j++)
				if (perte < petite_tribue->score_pertes[j])
					est_au_dessus_de++;
		}
		//
		petite_tribue->points[i] = est_au_dessus_de;
		//
		petite_tribue->podium[i] = i;
	}
	//
	uint c;
	for (uint i=0; i < N; i++) {
		for (uint j=i+1; j < N; j++) {
			if (petite_tribue->points[j] > petite_tribue->points[i]) {
				c = petite_tribue->points[j];
				petite_tribue->points[j] = petite_tribue->points[i];
				petite_tribue->points[i] = c;
				//
				c = petite_tribue->podium[j];
				petite_tribue->podium[j] = petite_tribue->podium[i];
				petite_tribue->podium[i] = c;
			}
		};
	};
};

void petite_tribue_mutations(Petite_Tribue_t * petite_tribue) {
	//podium:[g0,g1,p0,p1,p2,p3,p4,p5]
	//g0 -> p0:2
	//g1 -> p3:5
	uint c;
	uint gagants=petite_tribue->gagants, enfants=petite_tribue->enfants;
	for (uint i=0; i < petite_tribue->gagants; i++) {
		for (uint j=0; j < petite_tribue->enfants; j++) {
			c = petite_tribue->podium[gagants + i*enfants + j];
			free(petite_tribue->models[c]);
			petite_tribue->models[c] = cree_mutation(
				petite_tribue->models[petite_tribue->podium[i]]
			);
		};
	};
};

Mdl_t * petite_tribue(Mdl_t * depart) {

#define ECRIRE_RESULTAT 1

	INIT_CHRONO();
	//
	uint enfants = 4, gagants = 2;
	Petite_Tribue_t * ret = cree_petite_tribue(
		enfants, gagants, depart);
	//
	uint T = 30;
	//
	float gains[T+1];
	float pertes[T+1];
	//
	gains[0] = estimer_score(depart, GAIN_MOYEN);
	pertes[0] = estimer_score(depart, ANTI_PERTE);
	//
	for (uint t=0; t < T; t++) {
		DEPART_CHRONOS();
		//
		petite_tribue_scores(ret);
		petite_tribue_mutations(ret);
		//
		printf("%i/%i  temps restant ~= %f mins\n", t+1, T, (T-t-1)*VALEUR_CHRONO()/60.0);
		//for (uint i=0; i < ret->N; i++)
		//	plum_mdl_socle(ret->models[i]);
		//printf("\033[93m===============================================\033[0m\n");
#if (ECRIRE_RESULTAT == 1)
		Mdl_t * meilleur_mdl = ret->models[ret->podium[0]];
		gains[1+t] = estimer_score(
			meilleur_mdl,
			GAIN_MOYEN);
		pertes[1+t] = estimer_score(
			meilleur_mdl,
			ANTI_PERTE);
#endif
	}
	//
	petite_tribue_scores(ret);
	Mdl_t * meilleur = copier_mdl(ret->models[ret->podium[0]]);
	//
#if (ECRIRE_RESULTAT == 1)
	float *gainss[] = {gains};
	float *pertees[] = {pertes};
	gnuplots_titre(1, T+1, gainss, "Gains");
	gnuplots_titre(1, T+1, pertees, "Pertes");
#endif
	//
	liberer_petite_tribue(ret);
	//
	return meilleur;
};