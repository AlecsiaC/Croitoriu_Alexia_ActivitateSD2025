#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

void afisareMasina(Masina masina) {
	printf("ID:%d\n", masina.id);
	printf("Nr usi:%d\n", masina.nrUsi);
	printf("Pret:%.2f\n", masina.pret);
	printf("Model: s%\n", masina.model);
	printf("numeSofer:%s\n", masina.numeSofer);
	printf("Serie:%c\n", masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	
	for (int i = 0; i < nrMasini;i++) {
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) { //daca stim ca nu va fi sters e ok shallow copy, daca nu stim deep copy!
	
	Masina* temp;
	temp = malloc(sizeof(Masina) * ((*nrMasini) + 1));
	for (int i = 0; i < (*nrMasini); i++) {
		temp[i] = (*masini)[i];
	}
	temp[(*nrMasini)] = masinaNoua; //shallow copy and deep copy - review
	(*nrMasini)++;
	free((*masini));
	(*masini) = temp;



}

Masina citireMasinaFisier(FILE* file) {
	//functia citeste o masina dintr-un strceam deja deschis
	//masina citita este returnata;
	char sep[3] = ",\n";
	char* line[101];
	fgets(line, 100, file);
	Masina temp;
	temp.id = atoi(strtok(line, sep)); //tocare?? il taiem !!!!!atoi - ascii to integer
	temp.nrUsi = atoi(strtok(NULL, sep));
	temp.pret = atof(strtok(NULL, sep));

	char* buffer = strtok(NULL, sep);
	temp.model = (char*)malloc(sizeof(char) * (strlen(buffer)+1));
	strcpy_s(temp.model, (strlen(buffer) + 1), buffer);

	buffer = strtok(NULL, sep);
	temp.numeSofer = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy_s(temp.numeSofer, (strlen(buffer) + 1), buffer);
	
	buffer = strtok(NULL, sep);
	temp.serie = buffer[0]; //temp.serie = *buffer; merge
	return temp;
	
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {

	FILE* file;
	file = fopen(numeFisier, "r");

	Masina* vec = NULL;

	while (!feof(file)) {
		
		adaugaMasinaInVector(&vec, nrMasiniCitite, citireMasinaFisier(file));

	}

	fclose(file);
	return vec;

}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//este dezalocat intreg vectorul de masini
	for (int i = 0; i < nrMasini; i++) {
		free((*vector)[i].model); //eroare la dezalocare -> pb la alocare
		free((*vector)[i].numeSofer);
	}
	*nrMasini = 0;
	free(*vector);
	*vector = NULL;
}

int main() {

	int nr;
	nr = 0;
	Masina* vectorMasini = NULL;
	vectorMasini = citireVectorMasiniFisier("masini.txt", &nr);
	afisareVectorMasini(vectorMasini, nr);
	dezalocareVectorMasini(&vectorMasini, &nr);
	afisareVectorMasini(vectorMasini, nr);

	return 0;
}
