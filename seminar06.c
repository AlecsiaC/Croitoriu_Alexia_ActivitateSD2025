#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//2 obiecte au acelasi cod hash -> coliziune
//double hashing -> 1 element pe un cod hash -> aplic o alta functie de hash pt a primi o alta val. si pana gasesc un loc liber
//hash -> cautam mai usor
//ma uit la poz urm pana gasesc una libera -> linia probei
//chaining -> coliziune -> punem elementul pe aceeasi poz. -> lista simpla inlantuita(in majoritatea cazurilor) cu elementele prin pointer 

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

typedef struct Nod nod;
struct Nod {
	Masina info;
	nod* next;
};

//vect de pointer la nod
struct HashTable {
	int dim;
	nod** vector;

};
typedef struct HashTable HashTable;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(nod *cap) {

	while (cap != NULL) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(nod** cap, Masina masinaNoua) {
	nod* temp = (nod*)malloc(sizeof(nod));
	temp->info = masinaNoua; //shallow copy
	temp->next = NULL;

	if ((*cap) == NULL) {
		(*cap) = temp;
	}
	else {
		nod* aux = (*cap); //-> dereferentiere

		while (aux->next != NULL) {
			aux = aux->next;
		}
		
		aux->next = temp;
	}
}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	//initializeaza vectorul de liste si seteaza fiecare lista ca fiind NULL;
	ht.vector = (nod**)malloc(sizeof(nod*)*dimensiune);
	for (int i = 0; i < dimensiune;i++) {
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char* cheieNumeSofer, int dimensiune) { 
	//inainte sa inmultim valoarea la dimensiune o inmultim cu un nr. prim -> recomandare
	// este calculat hash-ul in functie de dimensiunea tabelei si un atribut al masinii
	//suma caracterelor % dim
	if (!dimensiune && dimensiune < 0) {
		return -1;
	}

		unsigned int suma = 0;
		for (int i = 0; i < strlen(cheieNumeSofer); i++) {
			suma += (int)cheieNumeSofer[i];
		}


		suma %= dimensiune; // rezultatul = hashcode

		return suma;
	

}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	
	int pozitie = calculeazaHash(masina.numeSofer, hash.dim);
	//verificam daca avem coliziune
	if (hash.vector[pozitie] == NULL) {
		//nu avem coliziune
		adaugaMasinaInLista(&(hash.vector[pozitie]), masina);
	}
	else {
		//avem coliziune
		adaugaMasinaInLista(&(hash.vector[pozitie]), masina);
	}

}

HashTable citireMasiniDinFisier(const char* numeFisier, int dimensiune) {

	FILE* f = fopen(numeFisier, "r");
	HashTable hash = initializareHashTable(dimensiune);
	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		inserareMasinaInTabela(hash, masina);
	}
	fclose(f);
	return hash;
}

void afisareTabelaDeMasini(HashTable ht) {
	//sunt afisate toate masinile cu evidentierea clusterelor realizate
	for (int i = 0; i < ht.dim; i++) {
		printf("Suntem la lantul %d\n", i + 1);
		afisareListaMasini(ht.vector[i]);
	}
}

void dezalocareTabelaDeMasini(HashTable *ht) {
	//sunt dezalocate toate masinile din tabela de dispersie
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	//calculeaza pretul mediu al masinilor din fiecare cluster.
	//trebuie sa returnam un vector cu valorile medii per cluster.
	//lungimea vectorului este data de numarul de clustere care contin masini
	return NULL;
}

Masina getMasinaDupaCheie(HashTable ht, const char* numeSofer) {
	Masina m;
	m.id = -1;
	//cauta masina dupa valoarea atributului cheie folosit in calcularea hash-ului
	//trebuie sa modificam numele functiei 
	int pozitie = calculeazaHash(numeSofer, ht.dim);
	if (pozitie >= 0 || pozitie < ht.dim || ht.vector[pozitie] == NULL) {
		return m;
	}
	nod* aux = ht.vector[pozitie];
	while (aux != NULL && strcmp(aux->info.numeSofer, numeSofer) != 0) {
		aux = aux->next;
	}
	if (aux != NULL) {
		m.numeSofer = (char*)malloc(sizeof(strlen(numeSofer)+1));
		strcpy(m.numeSofer, strlen(numeSofer) + 1, numeSofer);
		m.model = (char*)malloc(sizeof(strlen(aux->info.model) + 1));
		strcpy(m.model, strlen(aux->info.model, aux->info.model));
		m.id = aux->info.id;
		m.nrUsi = aux->info.nrUsi;
		m.pret = aux->info.pret;
		m.serie = aux->info.serie;
		
	}
	return m;
	
	
}

int main() {

	HashTable hash;
	hash = citireMasiniDinFisier("masini.txt",5);
	afisareTabelaDeMasini(hash);
	printf("Masina lui Ionescu este: ");
	Masina IonescuMasina = getMasinaDupaCheie(hash, "Ionescu");
	if (IonescuMasina.id != -1) 
	{
		afisareMasina(IonescuMasina);
		free(IonescuMasina.numeSofer);
	}
	else {
		printf("Nu am gasit masina");
	}
	return 0;
}