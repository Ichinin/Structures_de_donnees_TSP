// Exercice3.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>
using namespace std;

void VilleParser(vector<vector<float>> *listeVille, string nomFichier);
float distance(vector<float> ville1, vector<float> ville2);
void matriceDistance(vector<vector<float>> *matriceDistance, const vector<vector<float>> &listeVille);
void solutionAleatoire(vector<int> *solution, float *distanceParcourue, int nbVille, vector<vector<float>> *mD);
vector<int> tourneeNN(vector<vector<float>> *mD,int villeDepart, int nbVilles);


int main()
{
	srand(time(NULL));
	vector<vector<float>> *listeVille = new vector<vector<float>>();
	vector<vector<float>> *mD= new vector<vector<float>>();
	string fileName = "ville.txt";

	//Chargement des villes dans la liste de flottants
	VilleParser(listeVille, fileName);

	int nbVille = listeVille->size();
	int nbIterarion = 3000;

	int i = 0;
	for each (vector<float> ville in *listeVille)
	{
		cout << i<<" X: " << ville[0] << " Y: " << ville[1] << endl;
		i++;
	}

	//Creation d'une matrice carree de distance depuis une ville vers toutes les autres.
	matriceDistance(mD, *listeVille);

	vector <vector<int>> tourneeNnVille;
	vector<float> distancesVille;
	vector<double> distances;
	tourneeNnVille.resize((*listeVille).size());
	distancesVille.resize((*listeVille).size());

	
	//Calcul des tournees et leurs distances de djisktra au depart de chaque ville
	for (int villeDepart = 0; villeDepart < (*listeVille).size(); villeDepart++) {
		vector<int> ordreTournee = tourneeNN(mD, villeDepart, (*listeVille).size());

		double distanceTotale = 0;
		int villeCourante;
		int prochaineVille;
		for (int i = 0; i < (ordreTournee.size()-1); i++) {
			villeCourante = ordreTournee[i];
			prochaineVille = ordreTournee[i + 1];
			distanceTotale += (*mD)[villeCourante][prochaineVille];	
		}

		tourneeNnVille[villeDepart] = ordreTournee;
		distancesVille[villeDepart] = distanceTotale;
		
	}

	//Choix de la tournee la plus courte

	int indexTournee = 0;
	double tourneeCourte = _LLONG_MAX;

	for (int i = 0; i < distancesVille.size(); i++) {
		if (tourneeCourte > distancesVille[i]) {
			tourneeCourte = distancesVille[i];
			indexTournee = i;
		}
	}

	cout << "meilleur Solution :" << endl;

	for (int j = 0; j < tourneeNnVille[indexTournee].size(); j++) {
		cout << tourneeNnVille[indexTournee][j] << " ";
	}

	cout << " " << endl;
	cout << "Distance parcourue: " << tourneeCourte << endl;

	/*
	// Heurisistique de descente avec generation de solution aleatoire
	vector<int> *bestSolution = new vector<int>();
	float a = 0;
	float *shortestDistance = &a;
	solutionAleatoire(bestSolution, shortestDistance, nbVille, mD);

	vector<int> *solutionCourante = new vector<int>();
	float b = 0;
	float *distanceCourante = &b;
	for (int l = 0; l < nbIterarion; l++) {
		solutionAleatoire(solutionCourante, distanceCourante, nbVille, mD);
		if (*distanceCourante < *shortestDistance) {
			cout << "new  best solution" << endl;
			*shortestDistance = *distanceCourante;
			*bestSolution = *solutionCourante;
		}
	}
	cout << "Meilleur solution: ";
	for (int l = 0; l < bestSolution->size(); l++) {
		cout << (*bestSolution)[l] << " " ;
	}
	cout << " "<< endl;
	cout << "Distance parcourue: " << *shortestDistance << endl;
*/

	system("PAUSE");
	return 0;
}

vector<int> tourneeNN(vector<vector<float>> *mD, int villeDepart, int nbVilles) {
	
	vector<int> tournee;
	tournee.resize(nbVilles);
	tournee[0] = villeDepart;
	int villeProche;
	//Pour chaque ville trouver plus proche
	for (int i = 0; i < (nbVilles-1); i++) {

		vector<float> distances = (*mD)[tournee[i]];
		double distanceProche = _LLONG_MAX;
		bool present = false;
		for (int j = 0; j < nbVilles; j++) {
			present = false;
			if (distances[j] != 0 && distanceProche>distances[j])
			{
				for (int k = 0; k < i + 1; k++) {
					if (tournee[k] == j) {
						present = true;
					}
				}

				if (!present) {
					villeProche = j;
					distanceProche = distances[j];
				}
					
			}
		}

		tournee[i + 1] = villeProche;
	}
	return tournee;
}

void solutionAleatoire(vector<int> *solution, float *distanceParcourue, int nbVille, vector<vector<float>> *mD) {
	solution->clear();
	vector<int> *villeDisponible = new vector<int>();
	float d = 0;
	// Premier element
	for (int l = 0; l < nbVille; l++) {
		villeDisponible->push_back(l + 1);
	}
	int r = rand() % villeDisponible->size();
	int ville = (*villeDisponible)[r];
	solution->push_back(ville);
	villeDisponible->erase(villeDisponible->begin() + r);
	
	// Ajout des autres villes
	for (int l = 0; l < (nbVille - 1); l++) {
		r = rand() % villeDisponible->size();
		int ville = (*villeDisponible)[r];
		int villePrec = (*solution)[l];
		solution->push_back(ville);
		if (villePrec < ville) {
			d += (*mD)[villePrec - 1][ville - 1 - villePrec];
		}
		else {
			d += (*mD)[ville - 1][villePrec - 1 - ville];
		}
		villeDisponible->erase(villeDisponible->begin() + r);
	}
	// Modification de la distance parcourue
	*distanceParcourue = d;
}

void matriceDistance(vector<vector<float>> *matriceDistance, const vector<vector<float>> &listeVille) {

	matriceDistance->resize((listeVille.size() - 1));

	for (int i = 0; i < (listeVille.size() - 1); i++) {

		vector<float> villecourante = listeVille[i];

		for (int j = i + 1; j < listeVille.size(); j++) {
			(*matriceDistance)[i].push_back(distance(villecourante, listeVille[j]));
		}
	}

}
float distance(vector<float> ville1, vector<float> ville2) {
	float distance = 0.0;
	float somme = 0.0;

	if (ville1.size() == ville2.size()) {
		for (int i = 0; i < ville2.size(); i++)
		{
			somme += pow(abs(ville1[i] - ville2[i]), 2);
		}
	}

	return sqrt(somme);

}

void VilleParser(vector<vector<float>> *listeVille, string nomFichier) {
	
	
	ifstream infile;
	infile.open(nomFichier);
	string line;

	while (getline(infile, line)) {
		int numVille;
		float axeX, axeY;
		istringstream iss(line);
		if (iss >> numVille >> axeX >> axeY) {
			listeVille->push_back({ axeX, axeY });
		}
	}

}