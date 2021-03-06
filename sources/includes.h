#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>

struct list{
	int taille;
	void *debutAlloc;
	void *finAlloc;
	struct list *allocSuivant;
};

typedef struct list listeAllocation;
listeAllocation *listeAllocations = NULL;
listeAllocation *espaceLibres = NULL;



void *espaceMemoireDebut;

void *espaceMemoireFin;

void *adresseAlloc;

void afficherAllocation();

void *rechercherAllocation(void *p);

int taille;

int initMemory(int nBytes);

void *myalloc(int nBytes);

int myfree(void *p);

int freeMemory();

void *blocLibre(int nBytes);

int tailleOctet(int nBytes);
