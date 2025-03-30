#define _CRT_SECIRE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>

struct Telefon {
	int id;
	int RAM;
	char* producator;
	float pret;
	char serie; //un singur caracter - codul ASCII al acelui caracter
};

struct Telefon initializare(int id, int ram, const char* producator/*transmitem adresa de unde e stocata informatia, putem modifica fara const si nu este ok, deci nu uitam de const!*/, float pret, char serie) 
	//initializare un articol de tipul structurii telefon - functia initializare e asemanatoare cu constructorul cu param de la c++
{ 
	struct Telefon t; //de fiecare data cand declaram ceva de tipul Telefon trebuie sa folosim 'struct'
	t.id = id;
	t.RAM = ram;
	t.producator = (char*)malloc(sizeof(char)*(strlen(producator)+1)); //malloc imi aloca spatiu si imi returneaza adresa de inceput a blocului de memorie pe care o salvez in t.producator
	//producatorul este un sir de caractere -> strlen(producator) calculeaza cate caractere am intr-un parametru primit, adunam + 1 
	//parametrul este dimensiunea -> cati octeti trebuie sa aloce -> imi va aloca un bloc de mem care va ocupa fix atatia octeti cat ii voi spune eu
	//malloc este o functie care in primul rand returneaza void* (pointer generic) => facem cast explicit pt a transforma din void* in char*
	strcpy_s(t.producator, strlen(producator) + 1, producator); //destinatie, dimensiune, sursa
	t.pret = pret;
	t.serie = serie; //copiaza codul ASCII
	return t;
}

void afisare(struct Telefon t) 
{
	if (t.producator != NULL) {
		printf("%d. Telefonul %s seria %c are %d GB RAM si costa %5.2f RON.\n", t.id, t.producator, t.serie, t.RAM, t.pret);
	}
	else {
		printf("%d. Telefonul din seria %c are %d GB RAM si costa %5.2f RON.\n", t.id, t.serie, t.RAM, t.pret);
	}
}

void modificaPret(struct Telefon* t, float noulPret) //comparativ cu c++, trebuie sa ii dam si articolul de tip telefon t ca parametru
//crestem pretul -> aceasta fct e echivalentul unui setter (ptc nu avem metode in c)
//ca sa pastrez valoarea lui t si la iesirea din functie, transmit telefonul prin adresa -> prin pointer* => t este un pointer care imi va retine o adresa
//fiindca vreau sa il modific, nu il declar constant
{ 
	if (noulPret > 0) {
		t->pret = noulPret; //il transmit prin pointer -> trebuie sa dereferentiez (ma duc la adresa obiectului t, accesez atributul pret si il modific) -> (*t).pret = noulPret; sau prin operatorul ->
		//astfel, fac modificarea direct in stiva de mem. a functiei main => stiva de memorie se sterge, dar telefonul ramane cu pretul modificat
		//grija la transmiterea parametrilor!
		//in JAVA se transmit prin referinta majoritatea param.
	}
}

void dezalocare(struct Telefon* t) //transmitem prin pointer pentru ca facem o modificare, avem alocat dinamic doar producatorul
{
	if (t->producator != NULL) {
		free(t->producator); //am sters spatiul de memorie, dar pointerul in continuare retine adresa spatiului sters => initializam pointerul cu NULL
		t->producator = NULL;
	}
}

int main() { 
	struct Telefon t; //stiva de memorie a functiei main -> cand transmitem catre modificaPret se face o copie (transmitere prin val. -> se copiaza val. lui t din stiva de mem. a functiei main in stiva de mem.
	//a functiei modificaPret)
	//modific pretul telefonului copiat din modificaPret, iar cand ies din functie stiva functiei modificaPret va fi stearsa
	t = initializare(1, 256, "Samsung", 2000.5, 'A'); //sir de caractere -> "", un caracter -> '' (in python nu conteaza daca pun "" sau '', conteaza doar sa inchid cu acelasi lucru cu care deschid
	afisare(t);
	modificaPret(&t, 1000);  //daca primeste pointer in functie atunci trebuie sa ii dau o adresa functiei, nu un obiect de tipul telefon! -> adaug &
	afisare(t);
	dezalocare(&t); //& de t pentru ca il transmitem prin adresa
	afisare(t);
	return 0;
} //tot telefonul t va fi sters in momentul in care este stearsa stiva lui main, producatorul este alocat in zona heap (dinamic), deci trebuie dezalocat manual spatiul