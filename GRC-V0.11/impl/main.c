#include "prospection.h"

void main() {
	charger_les_prixs();
	generer_les_instructions();
	//
	Mdl_t * mdl = generique();
	lier_le_socle(mdl);
	aleatoire_poids();
	plum_mdl_socle(mdl);
	printf("gain=%f, pertes=%f\n",
		estimer_score(mdl, GAIN_MOYEN),
		estimer_score(mdl, ANTI_PERTE));
	//
	Mdl_t * meilleur = petite_tribue(mdl);
	aleatoire_poids();
	plum_mdl_socle(meilleur);
	printf("gain=%f, pertes=%f\n",
		estimer_score(meilleur, GAIN_MOYEN),
		estimer_score(meilleur, ANTI_PERTE));
	prospection(meilleur);
	//

	//
	liberer_les_prixs();
};

void main_verifier() {
	charger_les_prixs();
	generer_les_instructions();

	// ========== Ker Programme ==========
	Mdl_t * mdl = generique();
	lier_le_socle(mdl);
	aleatoire_poids();
	plum_mdl_socle(mdl);

	printf("============ GAINS ===========\n");
	srand(0);
	comparer_mdt1e5_retro_propa_gains(mdl);

	printf("============ PERTES ===========\n");
	srand(0);
	comparer_mdt1e5_retro_propa_pertes(mdl);

	// ===================================
	liberer_les_prixs();
};

void main0() {
	charger_les_prixs();
	generer_les_instructions();

	// ========== Ker Programme ==========
	Mdl_t * mdl = generique();
	lier_le_socle(mdl);
	aleatoire_poids();

	//plum_mdl_socle(mdl);
	//prospection(mdl);

	//
	plum_mdl_socle(mdl);
	printf("gain=%f, pertes=%f\n",
		estimer_score(mdl, GAIN_MOYEN),
		estimer_score(mdl, ANTI_PERTE));

	uint T = 100;

	float gains[T+1];
	float pertes[T+1];

	gains[0] = estimer_score(mdl, GAIN_MOYEN);
	pertes[0] = estimer_score(mdl, ANTI_PERTE);

	uint enfants=7, gagnants=4;
	Petite_Tribue_t * petite_tribue = cree_petite_tribue(
		enfants, gagnants, mdl);

	INIT_CHRONO();

	//
	for (uint t=0; t < T; t++) {
		DEPART_CHRONOS();
		//
		petite_tribue_scores(petite_tribue);
		petite_tribue_mutations(petite_tribue);
		printf("%i/%i  temps restant ~= %f mins\n", t+1, T, (T-t-1)*VALEUR_CHRONO()/60.0);
		//printf("##################################\n");
		//plum_mdl_socle(petite_tribue->models[petite_tribue->podium[0]]);
		//printf("points : ");
		//for (uint j=0; j < petite_tribue->N; j++) {
		//	printf("%i, ", petite_tribue->points[j]);
		//}
		//printf("\n");
		gains[1+t] = estimer_score(
			petite_tribue->models[petite_tribue->podium[0]], GAIN_MOYEN);
		pertes[1+t] = estimer_score(
			petite_tribue->models[petite_tribue->podium[0]], ANTI_PERTE);
	}

	printf("=================================================\n");
	printf("=================================================\n");
	printf("=================================================\n");

	//	Meilleur
	petite_tribue_scores(petite_tribue);
	Mdl_t * meilleur = petite_tribue->models[petite_tribue->podium[0]];

	//
	plum_mdl_socle(meilleur);
	printf("gain=%f, pertes=%f\n",
		estimer_score(meilleur, GAIN_MOYEN),
		estimer_score(meilleur, ANTI_PERTE));

	gnuplot(gains, T+1);
	gnuplot(pertes, T+1);

	prospection(meilleur);

	//
	liberer_petite_tribue(petite_tribue);

	// ===================================
	liberer_les_prixs();
};

void main1() {
	//main_verifier();
	//return;

	charger_les_prixs();
	generer_les_instructions();

	// ========== Ker Programme ==========
	Mdl_t * mdl = generique();
	lier_le_socle(mdl);
	aleatoire_poids();

	for (uint i=0; i < 100; i++) {
		optimiser_gains(mdl, 10);
		optimiser_pertes(mdl, 10);
	}

	plum_mdl_socle(mdl);
	prospection(mdl);

	// ===================================
	liberer_les_prixs();
};

void main7() {
	charger_les_prixs();
	generer_les_instructions();

	// ========== Ker Programme ==========
	Mdl_t * mdl = generique();
	lier_le_socle(mdl);
	aleatoire_poids();

	plum_mdl_socle(mdl);
	printf("gain=%f, pertes=%f\n",
		estimer_score(mdl, GAIN_MOYEN),
		estimer_score(mdl, ANTI_PERTE));

	printf("=================================================\n");
	printf("=================================================\n");
	printf("=================================================\n");

	uint T = 20;

	float gains[T+1];
	float pertes[T+1];

	gains[0] = estimer_score(mdl, GAIN_MOYEN);
	pertes[0] = estimer_score(mdl, ANTI_PERTE);

	Peuple_t * peuple = cree_peuple(
		mdl,
		2, 1,
		1, 2
	);

	INIT_CHRONO();

	printf(" == Depart ==\n");
	printf("0/%i ---- \n", T);

	//
	for (uint t=0; t < T; t++) {
		DEPART_CHRONOS();
		//
		for (uint i=0; i < 30; i++) {
			peuple_vie_interne_tribue(peuple);
			printf("\t%i/%i\n", i+1, 30);
		}
		peuple_meilleur_score(peuple);
		//
		printf("%i/%i  temps restant ~= %f mins\n", t+1, T, (T-t-1)*VALEUR_CHRONO()/60.0);
		//
		Petite_Tribue_t * meilleur_tribue = peuple->tribue[peuple->podium[0]];
		Mdl_t * meilleur_mdl = meilleur_tribue->models[meilleur_tribue->podium[0]];
		//
		gains[1+t] = estimer_score(
			meilleur_mdl,
			GAIN_MOYEN);
		pertes[1+t] = estimer_score(
			meilleur_mdl,
			ANTI_PERTE);
		//
		//
		//
		if (t!=T-1)
			peuple_nouvelles_tribues_mutee(peuple);
	}

	printf("=================================================\n");
	printf("=================================================\n");
	printf("=================================================\n");

	//	Meilleur
	peuple_meilleur_score(peuple);
	Petite_Tribue_t * meilleur_tribue = peuple->tribue[peuple->podium[0]];
	petite_tribue_scores(peuple->tribue[peuple->podium[0]]);
	Mdl_t * meilleur = meilleur_tribue->models[meilleur_tribue->podium[0]];

	//
	plum_mdl_socle(meilleur);
	printf("gain=%f, pertes=%f\n",
		estimer_score(meilleur, GAIN_MOYEN),
		estimer_score(meilleur, ANTI_PERTE));

	//gnuplot(gains, T+1);
	//gnuplot(pertes, T+1);

	float *gainss[] = {gains};
	float *pertees[] = {pertes};

	gnuplots_titre(1, T+1, gainss, "Gains");
	gnuplots_titre(1, T+1, pertees, "Pertes");

	prospection(meilleur);

	//
	liberer_peuple(peuple);

	// ===================================
	liberer_les_prixs();
};