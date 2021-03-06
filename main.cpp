/*Simulation champs moyen
 *les populations considèrées sont n et nu, n pour la population de cellule CS (composante solide) et nu poru la population de cellule CVD (composante verre dépoli)
 *les lettres de notre alphabet sont les taux pour la population de la CS et les lettres grec pour la CVD
 *
 *Chaqu'une des populations à 3 traits indicé 1, 2, 3. b1 est donc le taux de naissance associer à la population CS de trait 1, beta 1 est le taux de naissance associer
 *à la population CVD de trait 1
 *
 *Je pense que beaucoup de taux de mutation ne seront pas utile mais il est plus facile de les mêttre dès le départ et de les mêttre à 0 que de devoir en rajouter après
 *donc je les ai mis
 */

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <random>

using namespace std;

int main()
{
/******************************************************************************************
***                              déclaration des variables                              ***
******************************************************************************************/
// PARAMETRE DU SYSTEME !
	int const K(1);
	int const NOMBRE_DE_VALEUR(2500);
	int const NOMBRE_DE_POPULATION(2);
	int const NOMBRE_DE_TRAIT(3);
	int nombreIteration = NOMBRE_DE_VALEUR*K;

	double const TAUX_MUTANT_MU(0.5);
	double X(0.); // X interval entre T[i] et T[i+1]
	double tauxGlobal(0.); // taux global de tout les PPP ensemble
// SAUVEGARDE DES DONNEES ! 
	string const fichierSaveData("/home/aurigolys/Documents/master/1_stage_M2/3_Work/4_cpp/nodules_simulation/data.dat"); //Il faudra changer ça pour le mettre bien pour vos pc
	ofstream savingData(fichierSaveData.c_str());
// TABLEAU
//                               taux de naissance (2*3)
	vector< vector<double> > tauxNaissance(NOMBRE_DE_POPULATION, vector<double >(NOMBRE_DE_TRAIT));
	tauxNaissance[0][0] = double(1.000);	//taux de naissance de n1    b1 
	tauxNaissance[0][1] = double(1.000);	//taux de naissance de n2    b2 
	tauxNaissance[0][2] = double(1.000);	//taux de naissance de n3    b3 
	tauxNaissance[1][0] = double(1.000);	//taux de naissance de nu1   beta1
	tauxNaissance[1][1] = double(1.000);	//taux de naissance de nu2   beta2
	tauxNaissance[1][2] = double(1.000);	//taux de naissance de nu3   beta3

//                               taux de mort (2*3)
	vector< vector<double> > tauxMort(NOMBRE_DE_POPULATION, vector<double >(NOMBRE_DE_TRAIT));
	tauxMort[0][0] = double(1.000);	//taux de mort de n1    d1 
	tauxMort[0][1] = double(1.000);	//taux de mort de n2    d2 
	tauxMort[0][2] = double(1.000);	//taux de mort de n3    d3 
	tauxMort[1][0] = double(1.000);	//taux de mort de nu1   delta1
	tauxMort[1][1] = double(1.000);	//taux de mort de nu2   delta2
	tauxMort[1][2] = double(1.000);	//taux de mort de nu3   delta3

//                               taux de chaque population (2*3)
	vector< vector<double> > tauxEspece(NOMBRE_DE_POPULATION, vector<double >(NOMBRE_DE_TRAIT));
	tauxEspece[0][0] = double(0.000);	//taux de n1 
	tauxEspece[0][1] = double(0.000);	//taux de n2
	tauxEspece[0][2] = double(0.000);	//taux de n3  
	tauxEspece[1][0] = double(0.000);	//taux de nu1
	tauxEspece[1][1] = double(0.000);	//taux de nu2
	tauxEspece[1][2] = double(0.000);	//taux de nu3

//                               taux de chaque population (2*3)
	vector< vector<double> > probaEspeces(NOMBRE_DE_POPULATION, vector<double >(NOMBRE_DE_TRAIT));
	probaEspeces[0][0] = double(0.000);	//proba que n1 agisse
	probaEspeces[0][1] = double(0.000);	//proba que n2 agisse
	probaEspeces[0][2] = double(0.000);	//proba que n3 agisse  
	probaEspeces[1][0] = double(0.000);	//proba que nu1 agisse
	probaEspeces[1][1] = double(0.000);	//proba que nu2 agisse
	probaEspeces[1][2] = double(0.000);	//proba que nu3 agisse

//                               taux de compétition (6*6)
	vector< vector<double> > tauxCompetition(NOMBRE_DE_POPULATION*NOMBRE_DE_TRAIT, vector<double>(NOMBRE_DE_POPULATION*NOMBRE_DE_TRAIT));
	tauxCompetition[0][0] = double(1.000)/double(K);	//taux de competition de n1 sur n1 		c11  
	tauxCompetition[0][1] = double(1.000)/double(K);	//taux de competition de n1 sur n2		c12
	tauxCompetition[0][2] = double(1.000)/double(K);	//taux de competition de n1 sur n3		c13
	tauxCompetition[0][3] = double(1.000)/double(K);	//taux de competition de n1 sur nu1		c14
	tauxCompetition[0][4] = double(1.000)/double(K);	//taux de competition de n1 sur nu2		c15
	tauxCompetition[0][5] = double(1.000)/double(K);	//taux de competition de n1 sur nu3		c16

	tauxCompetition[1][0] = double(1.000)/double(K);	//taux de competition de n2 sur n1		c21
	tauxCompetition[1][1] = double(1.000)/double(K);	//taux de competition de n2 sur n2		c22
	tauxCompetition[1][2] = double(1.000)/double(K);	//taux de competition de n2 sur n3		c23
	tauxCompetition[1][3] = double(1.000)/double(K);	//taux de competition de n2 sur nu1		c24
	tauxCompetition[1][4] = double(1.000)/double(K);	//taux de competition de n2 sur nu2		c25
	tauxCompetition[1][5] = double(1.000)/double(K);	//taux de competition de n2 sur nu3		c26

	tauxCompetition[2][0] = double(1.000)/double(K);	//taux de competition de n3 sur n1		c31
	tauxCompetition[2][1] = double(1.000)/double(K);	//taux de competition de n3 sur n2		c32
	tauxCompetition[2][2] = double(1.000)/double(K);	//taux de competition de n3 sur n3		c33
	tauxCompetition[2][3] = double(1.000)/double(K);	//taux de competition de n3 sur nu1		c34
	tauxCompetition[2][4] = double(1.000)/double(K);	//taux de competition de n3 sur nu2		c35
	tauxCompetition[2][5] = double(1.000)/double(K);	//taux de competition de n3 sur nu3		c36

	tauxCompetition[3][0] = double(1.000)/double(K);	//taux de competition de nu1 sur n1		c41
	tauxCompetition[3][1] = double(1.000)/double(K);	//taux de competition de nu1 sur n2		c42
	tauxCompetition[3][2] = double(1.000)/double(K);	//taux de competition de nu1 sur n3		c43
	tauxCompetition[3][3] = double(1.000)/double(K);	//taux de competition de nu1 sur nu1		c44
	tauxCompetition[3][4] = double(1.000)/double(K);	//taux de competition de nu1 sur nu2		c45
	tauxCompetition[3][5] = double(1.000)/double(K);	//taux de competition de nu1 sur nu3		c46

	tauxCompetition[4][0] = double(1.000)/double(K);	//taux de competition de nu2 sur n1		c51
	tauxCompetition[4][1] = double(1.000)/double(K);	//taux de competition de nu2 sur n2		c52
	tauxCompetition[4][2] = double(1.000)/double(K);	//taux de competition de nu2 sur n3		c53
	tauxCompetition[4][3] = double(1.000)/double(K);	//taux de competition de nu2 sur nu1		c54
	tauxCompetition[4][4] = double(1.000)/double(K);	//taux de competition de nu2 sur nu2		c55
	tauxCompetition[4][5] = double(1.000)/double(K);	//taux de competition de nu2 sur nu3		c56

	tauxCompetition[5][0] = double(1.000)/double(K);	//taux de competition de nu3 sur n1		c61
	tauxCompetition[5][1] = double(1.000)/double(K);	//taux de competition de nu3 sur n2		c62
	tauxCompetition[5][2] = double(1.000)/double(K);	//taux de competition de nu3 sur n3		c63
	tauxCompetition[5][3] = double(1.000)/double(K);	//taux de competition de nu3 sur nu1		c64
	tauxCompetition[5][4] = double(1.000)/double(K);	//taux de competition de nu3 sur nu2		c65
	tauxCompetition[5][5] = double(1.000)/double(K);	//taux de competition de nu3 sur nu3		c66

//                               taux de mutation (6*6)
	vector< vector<double> > tauxMutation(NOMBRE_DE_POPULATION*NOMBRE_DE_TRAIT, vector<double>(NOMBRE_DE_POPULATION*NOMBRE_DE_TRAIT));
	tauxMutation[0][0] = double(1.000)/double(K);	//taux de Mutation de n1 vers n1		m11
	tauxMutation[0][1] = double(1.000)/double(K);	//taux de Mutation de n1 vers n2		m12
	tauxMutation[0][2] = double(1.000)/double(K);	//taux de Mutation de n1 vers n3		m13
	tauxMutation[0][3] = double(1.000)/double(K);	//taux de Mutation de n1 vers nu1		m14
	tauxMutation[0][4] = double(1.000)/double(K);	//taux de Mutation de n1 vers nu2		m15
	tauxMutation[0][5] = double(1.000)/double(K);	//taux de Mutation de n1 vers nu3		m16

	tauxMutation[1][0] = double(1.000)/double(K);	//taux de Mutation de n2 vers n1		m21
	tauxMutation[1][1] = double(1.000)/double(K);	//taux de Mutation de n2 vers n2		m22
	tauxMutation[1][2] = double(1.000)/double(K);	//taux de Mutation de n2 vers n3		m23
	tauxMutation[1][3] = double(1.000)/double(K);	//taux de Mutation de n2 vers nu1		m24
	tauxMutation[1][4] = double(1.000)/double(K);	//taux de Mutation de n2 vers nu2		m25
	tauxMutation[1][5] = double(1.000)/double(K);	//taux de Mutation de n2 vers nu3		m26

	tauxMutation[2][0] = double(1.000)/double(K);	//taux de Mutation de n3 vers n1		m31
	tauxMutation[2][1] = double(1.000)/double(K);	//taux de Mutation de n3 vers n2		m32
	tauxMutation[2][2] = double(1.000)/double(K);	//taux de Mutation de n3 vers n3		m33
	tauxMutation[2][3] = double(1.000)/double(K);	//taux de Mutation de n3 vers nu1		m34
	tauxMutation[2][4] = double(1.000)/double(K);	//taux de Mutation de n3 vers nu2		m35
	tauxMutation[2][5] = double(1.000)/double(K);	//taux de Mutation de n3 vers nu3		m36

	tauxMutation[3][0] = double(1.000)/double(K);	//taux de Mutation de nu1 vers n1		m41
	tauxMutation[3][1] = double(1.000)/double(K);	//taux de Mutation de nu1 vers n2		m42
	tauxMutation[3][2] = double(1.000)/double(K);	//taux de Mutation de nu1 vers n3		m43
	tauxMutation[3][3] = double(1.000)/double(K);	//taux de Mutation de nu1 vers nu1		m44
	tauxMutation[3][4] = double(1.000)/double(K);	//taux de Mutation de nu1 vers nu2		m45
	tauxMutation[3][5] = double(1.000)/double(K);	//taux de Mutation de nu1 vers nu3		m46

	tauxMutation[4][0] = double(1.000)/double(K);	//taux de Mutation de nu2 vers n1		m51
	tauxMutation[4][1] = double(1.000)/double(K);	//taux de Mutation de nu2 vers n2		m52
	tauxMutation[4][2] = double(1.000)/double(K);	//taux de Mutation de nu2 vers n3		m53
	tauxMutation[4][3] = double(1.000)/double(K);	//taux de Mutation de nu2 vers nu1		m54
	tauxMutation[4][4] = double(1.000)/double(K);	//taux de Mutation de nu2 vers nu2		m55
	tauxMutation[4][5] = double(1.000)/double(K);	//taux de Mutation de nu2 vers nu3		m56

	tauxMutation[5][0] = double(1.000)/double(K);	//taux de Mutation de nu3 vers n1		m61
	tauxMutation[5][1] = double(1.000)/double(K);	//taux de Mutation de nu3 vers n2		m62
	tauxMutation[5][2] = double(1.000)/double(K);	//taux de Mutation de nu3 vers n3		m63
	tauxMutation[5][3] = double(1.000)/double(K);	//taux de Mutation de nu3 vers nu1		m64
	tauxMutation[5][4] = double(1.000)/double(K);	//taux de Mutation de nu3 vers nu2		m65
	tauxMutation[5][5] = double(1.000)/double(K);	//taux de Mutation de nu3 vers nu3		m66

//                               Taille des populations (2*3*nombreIteration)
	vector< vector< vector<double> > > populationsSize;
	populationsSize.resize(NOMBRE_DE_POPULATION);
	for (int i(0); i < NOMBRE_DE_POPULATION; i++)
	{
		populationsSize[i].resize(NOMBRE_DE_TRAIT);
		for (int j(0); j < NOMBRE_DE_TRAIT; j++)
		{
			populationsSize[i][j].resize(nombreIteration);	
		}
	}

//				 Population initial
	populationsSize[0][0][0] = double(500.000);	//n1 (t=0)
	populationsSize[0][1][0] = double(500.000);	//n2 (t=0)
	populationsSize[0][2][0] = double(500.000);	//n3 (t=0)
	populationsSize[1][0][0] = double(500.000);	//nu1(t=0)
	populationsSize[1][1][0] = double(500.000);	//nu2(t=0)
	populationsSize[1][2][0] = double(500.000);	//nu3(t=0)

//                               T (nombreIteration)
	vector<double> T(nombreIteration);

/******************************************************************************************
***                                 Paramètre pour random                               ***
******************************************************************************************/
// seed pour les nombre random
	srand(time(NULL));
// création de l'objet pour la variable exponentiel
	default_random_engine generator;

	
/******************************************************************************************
***                                 Boucle de calcule                                   ***
******************************************************************************************/

	for (int i(1); i < nombreIteration; i++)
	{
//Etape 1 : Détermination du taux global !
//Réinitialisation du taux global
	tauxGlobal = 0.;
		for (int ii(0); ii < NOMBRE_DE_POPULATION; ii++)
		{
			for (int j(0); j < NOMBRE_DE_TRAIT; j++)
			{
				tauxEspece[ii][j] = populationsSize[ii][j][i-1]*K*(tauxNaissance[ii][j] + tauxMort[ii][j] + populationsSize[0][0][i-1]*K*tauxCompetition[ii][0] + populationsSize[0][1][i-1]*K*tauxCompetition[ii][1] + populationsSize[0][2][i-1]*K*tauxCompetition[ii][2] + populationsSize[1][0][i-1]*K*tauxCompetition[ii][3] + populationsSize[1][1][i-1]*K*tauxCompetition[ii][4] + populationsSize[1][2][i-1]*K*tauxCompetition[ii][5])*(1-TAUX_MUTANT_MU) + TAUX_MUTANT_MU*(populationsSize[0][0][i-1]*K*tauxMutation[ii][0] + populationsSize[0][1][i-1]*K*tauxMutation[ii][1] + populationsSize[0][2][i-1]*K*tauxMutation[ii][2] + populationsSize[1][0][i-1]*K*tauxMutation[ii][3] + populationsSize[1][1][i-1]*K*tauxMutation[ii][4] + populationsSize[1][2][i-1]*K*tauxMutation[ii][5]) + TAUX_MUTANT_MU*populationsSize[ii][j][i-1]*K*tauxMutation[ii][j];
// Heu je ne suis pas sur c'est peut être la formule commenter qu'il faut utiliser il faut encore que je réfléchisse la dessus
				//tauxEspece[ii][j] = populationsSize[ii][j][i-1]*K*(tauxNaissance[ii][j] + tauxMort[ii][j] + populationsSize[0][0][i-1]*K*tauxCompetition[ii][0] + populationsSize[0][1][i-1]*K*tauxCompetition[ii][1] + populationsSize[0][2][i-1]*K*tauxCompetition[ii][2] + populationsSize[1][0][i-1]*K*tauxCompetition[ii][3] + populationsSize[1][1][i-1]*K*tauxCompetition[ii][4] + populationsSize[1][2][i-1]*K*tauxCompetition[ii][5])*(1-TAUX_MUTANT_MU) + TAUX_MUTANT_MU*(populationsSize[0][0][i-1]*K*tauxMutation[ii][0] + populationsSize[0][1][i-1]*K*tauxMutation[ii][1] + populationsSize[0][2][i-1]*K*tauxMutation[ii][2] + populationsSize[1][0][i-1]*K*tauxMutation[ii][3] + populationsSize[1][1][i-1]*K*tauxMutation[ii][4] + populationsSize[1][2][i-1]*K*tauxMutation[ii][5]);


//par defaut populationsSize[*][*][i] = populationsSize[*][*][i-1]. On dit que toutes les populations vallent ce qu'elles vallaient avant
 				populationsSize[ii][j][i] = populationsSize[ii][j][i-1];
				tauxGlobal = tauxGlobal + tauxEspece[ii][j];
			}
		}
//Etape 2 : détermination du temps ! 
		X = 0;
		exponential_distribution<double> exponentialLaw(tauxGlobal);
		X = exponentialLaw(generator);
		T[i] = T[i-1] + X;
//Etape 3 : Determination de l'évenement qui arrive. Mutation ? 
		double probaTesteur(0.);
		probaTesteur = double(rand())/double(RAND_MAX);
		if (probaTesteur < TAUX_MUTANT_MU)
		{
//Etape 3 M 1 : Determination de la population qui a un mutant !
//Reset tauxGlobal pour les mutations
			tauxGlobal = 0.;
			for (int ii(0); ii < NOMBRE_DE_POPULATION; ii++)
			{
				for (int j(0); j < NOMBRE_DE_TRAIT; j++)
				{
					tauxEspece[ii][j] = TAUX_MUTANT_MU*(populationsSize[0][0][i-1]*K*tauxMutation[ii][0] + populationsSize[0][1][i-1]*K*tauxMutation[ii][1] + populationsSize[0][2][i-1]*K*tauxMutation[ii][2] + populationsSize[1][0][i-1]*K*tauxMutation[ii][3] + populationsSize[1][1][i-1]*K*tauxMutation[ii][4] + populationsSize[1][2][i-1]*K*tauxMutation[ii][5]) + TAUX_MUTANT_MU*populationsSize[ii][j][i-1]*K*tauxMutation[ii][j];
					//tauxEspece[ii][j] = TAUX_MUTANT_MU*(populationsSize[0][0][i-1]*K*tauxMutation[ii][0] + populationsSize[0][1][i-1]*K*tauxMutation[ii][1] + populationsSize[0][2][i-1]*K*tauxMutation[ii][2] + populationsSize[1][0][i-1]*K*tauxMutation[ii][3] + populationsSize[1][1][i-1]*K*tauxMutation[ii][4] + populationsSize[1][2][i-1]*K*tauxMutation[ii][5]);
					tauxGlobal = tauxGlobal + tauxEspece[ii][j];
					//cout << "taux global mutation : " << tauxGlobal << endl;
				}
			}
			double probaTesteur(0.);
			probaTesteur = double(rand())/double(RAND_MAX);
			bool ChangementActive(true);
			int populationSelecteurFrom(NOMBRE_DE_POPULATION+1);
			int traitSelecteurFrom(NOMBRE_DE_TRAIT+1);
			double probaCumulative(0.);
			for (int ii(0); ii < NOMBRE_DE_POPULATION; ii++)
			{
				for (int j(0); j < NOMBRE_DE_TRAIT; j++)
				{
					probaEspeces[ii][j] = tauxEspece[ii][j]/tauxGlobal;
					//cout << "probaEspeces["<< ii << "][" << j << "] : " << probaEspeces[ii][j] << endl;
					probaCumulative = probaCumulative + probaEspeces[ii][j];
					if (probaTesteur < probaCumulative and ChangementActive == true)
					{
						populationSelecteurFrom = ii;
						traitSelecteurFrom = j;
						ChangementActive = false;
						//cout << "C'est la pop : " << populationSelecteurFrom << " de trait : " << traitSelecteurFrom << " qui agit " << endl;
					}
				}
			}
			//cout << "probaCumulative : " << probaCumulative  << "Devrait etre égal à 1 !" << endl;
//Etape 3 M 2 : Determination de vers quelle population on mute ! 
//Reset tauxGlobal pour savoir vers quelle population on mute
			tauxGlobal = 0.;
			for (int ii(0); ii < NOMBRE_DE_POPULATION; ii++)
			{
				for (int j(0); j < NOMBRE_DE_TRAIT; j++)
				{
					tauxEspece[ii][j] = populationsSize[ii][j][i-1]*K*tauxMutation[ii][j];
					tauxGlobal = tauxGlobal + tauxEspece[ii][j];
					//cout << "taux global mutation vers qui : " << tauxGlobal << endl;
				}
			}
			probaTesteur = 0.;
			probaTesteur = double(rand())/double(RAND_MAX);
			ChangementActive = true;
			int populationSelecteurTo(NOMBRE_DE_POPULATION+1);
			int traitSelecteurTo(NOMBRE_DE_TRAIT+1);
			probaCumulative = 0.;
			for (int ii(0); ii < NOMBRE_DE_POPULATION; ii++)
			{
				for (int j(0); j < NOMBRE_DE_TRAIT; j++)
				{
					probaEspeces[ii][j] = tauxEspece[ii][j]/tauxGlobal;
					//cout << "probaEspeces["<< ii << "][" << j << "] : " << probaEspeces[ii][j] << endl;
					probaCumulative = probaCumulative + probaEspeces[ii][j];
					if (probaTesteur < probaCumulative and ChangementActive == true)
					{
						populationSelecteurTo = ii;
						traitSelecteurTo = j;
						ChangementActive = false;
					}
				}
			}
			//cout << "Un individu de " << populationSelecteurFrom << " du trait" << traitSelecteurFrom << " mute en un individu de la pop : " << populationSelecteurTo << " de trait : " << traitSelecteurTo << endl;
			//cout << "probaCumulative : " << probaCumulative  << "Devrait etre égal à 1 !" << endl;
//Etape 3 M 3 : Modification des tailles des populations concerner !!
			if ((populationSelecteurFrom != populationSelecteurTo or traitSelecteurFrom != traitSelecteurTo) and populationsSize[populationSelecteurFrom][traitSelecteurFrom][i-1] != 0)
			{
				populationsSize[populationSelecteurFrom][traitSelecteurFrom][i] = populationsSize[populationSelecteurFrom][traitSelecteurFrom][i-1] - double(1)/double(K);
				populationsSize[populationSelecteurTo][traitSelecteurTo][i] = populationsSize[populationSelecteurTo][traitSelecteurTo][i-1] + double(1)/double(K);
			}
			//cout << "A T[" << i << "] : " << T[i] << " | n1 : " << populationsSize[0][0][i] << " | n2 : " << populationsSize[0][1][i] << " | n3 : " << populationsSize[0][2][i] << " | nu1 : " << populationsSize[1][0][i] << " | nu2 : " << populationsSize[1][1][i] << " | nu3 : " << populationsSize[1][2][i] << endl;
		}
		else
		{
//Etape 3 C 1 : Determination de l'espèce qui intéragi
			tauxGlobal = 0.;
			//cout << "PAS DE MUTATION !! au temps " << T[i] << endl;
			//cout << "iteration : " << i << endl;
			for (int ii(0); ii < NOMBRE_DE_POPULATION; ii++)
			{
				for (int j(0); j < NOMBRE_DE_TRAIT; j++)
				{
					tauxEspece[ii][j] = populationsSize[ii][j][i-1]*K*(tauxNaissance[ii][j] + tauxMort[ii][j] + populationsSize[0][0][i-1]*K*tauxCompetition[ii][0] + populationsSize[0][1][i-1]*K*tauxCompetition[ii][1] + populationsSize[0][2][i-1]*K*tauxCompetition[ii][2] + populationsSize[1][0][i-1]*K*tauxCompetition[ii][3] + populationsSize[1][1][i-1]*K*tauxCompetition[ii][4] + populationsSize[1][2][i-1]*K*tauxCompetition[ii][5]);

					tauxGlobal = tauxGlobal + tauxEspece[ii][j];
					//cout << "tauxGlobal : " << tauxGlobal << endl;
				}
			}
			double probaTesteur(0.);
			probaTesteur = double(rand())/double(RAND_MAX);
			bool ChangementActive(true);
			int populationSelecteurFrom(NOMBRE_DE_POPULATION+1);
			int traitSelecteurFrom(NOMBRE_DE_TRAIT+1);
			double probaCumulative(0.);
			for (int ii(0); ii < NOMBRE_DE_POPULATION; ii++)
			{
				for (int j(0); j < NOMBRE_DE_TRAIT; j++)
				{
					probaEspeces[ii][j] = tauxEspece[ii][j]/tauxGlobal;
					//cout << "probaEspeces["<< ii << "][" << j << "] : " << probaEspeces[ii][j] << endl;
					probaCumulative = probaCumulative + probaEspeces[ii][j];
					if (probaTesteur < probaCumulative and ChangementActive == true)
					{
						populationSelecteurFrom = ii;
						traitSelecteurFrom = j;
						ChangementActive = false;
						//cout << "C'est la pop : " << populationSelecteurFrom << " de trait : " << traitSelecteurFrom << " qui agit " << endl;
					}
				}
			}
			//cout << "probaCumulative : " << probaCumulative  << "Devrait etre égal à 1 !" << endl;
//Etape 3 C 2 : Determination de ce qui lui arrive ! 
//Reset tauxGlobal pour savoir vers quelle population on mute
			tauxGlobal = tauxEspece[populationSelecteurFrom][traitSelecteurFrom];
			double tauxNaissanceEspece(0.);
			tauxNaissanceEspece = populationsSize[populationSelecteurFrom][traitSelecteurFrom][i-1]*K*tauxNaissance[populationSelecteurFrom][traitSelecteurFrom];
			double probaNaissance(0.);
			probaNaissance = tauxNaissanceEspece/tauxGlobal;

			probaTesteur = 0.;
			probaTesteur = double(rand())/double(RAND_MAX);
			if (probaTesteur < probaNaissance)
			{
				if (populationsSize[populationSelecteurFrom][traitSelecteurFrom][i-1] != 0)
				{
					populationsSize[populationSelecteurFrom][traitSelecteurFrom][i] = populationsSize[populationSelecteurFrom][traitSelecteurFrom][i-1] + double(1)/double(K);
				}
			}
			else 
			{
				if (populationsSize[populationSelecteurFrom][traitSelecteurFrom][i-1] != 0)
				{
					populationsSize[populationSelecteurFrom][traitSelecteurFrom][i] = populationsSize[populationSelecteurFrom][traitSelecteurFrom][i-1] - double(1)/double(K);
				}
			}
			

		}
		cout << populationsSize[0][0][i] << " , " << populationsSize[0][1][i] << " , " << populationsSize[0][2][i] << " , " << populationsSize[1][0][i] << " , " << populationsSize[1][1][i] << " , " << populationsSize[1][2][i] << " , " << T[i] << endl;
		if ((int(populationsSize[0][0][i]) == int(0)) and (int(populationsSize[0][1][i]) == int(0)) and (int(populationsSize[0][2][i]) == int(0)) and (int(populationsSize[1][0][i]) == int(0)) and (int(populationsSize[1][1][i]) == int(0)) and (int(populationsSize[1][2][i]) == int(0)))
		{
			cout << "Toutes les populations vallent 0 => on arrete le programme" << endl; 
			return 0;
		}
		if (savingData)
		{
			savingData << populationsSize[0][0][i] << " , " << populationsSize[0][1][i] << " , " << populationsSize[0][2][i] << " , " << populationsSize[1][0][i] << " , " << populationsSize[1][1][i] << " , " << populationsSize[1][2][i] << " , " << T[i] << endl;
		}
		else 
		{
			cout << "Erreur :: Ne peut pas ouvrir le fichier " << fichierSaveData << " ... " << endl;	
		}
		cout << double(i)/double(nombreIteration-1)*100 << " % " << endl;
	}
	return 0;
}
