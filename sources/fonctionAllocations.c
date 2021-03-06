#include<stdio.h>
#include<stdlib.h>
#include"includes.h"


int tailleOctet(int nBytes){
	int taille=0;
	if(nBytes % 8 ==0){
		taille = nBytes/8;	
	}
	else{
		taille= (nBytes/8)+1;	
	}
	return taille;
}
/**
 *cette fonction retourne un pointeur (*espaceMemoireDebut)
 *de type void sur l'espace memoire de travail interne.
 *\param espaceMemoireDebut est l'adresse de debut de notre espace d'allocation
 *\param espaceMemoireFin est est l'adresse de fin de notre espace d'allocation
 */

int initMemory(int nBytes){
	if(espaceMemoireDebut=malloc(nBytes/8)){
		espaceMemoireFin = espaceMemoireDebut + nBytes/8;
		taille = nBytes/8;
		return nBytes;	
	}
	else{
		return 0;		
	}
}

/**
 *cette fonction libere l'espace memoire de travaille interne et retourne
 *la taille initialement alloué.
 *\param espaceMemoireDebut etant le pointeur retourner par le malloc sur initMemory() est liberé.
 */
int freeMemory(){
	if(espaceMemoireDebut){
		free(espaceMemoireDebut);
		return taille*8;	
	}
	else{
		return -1;	
	}
}

/**
 *cette fonction alloue de la memoire sur notre zone de memoire interne
 */
void *myalloc(int nBytes){
	/**
	 *On verifie si aucune allocation n'a été faite.
	 *si OUI on fait la premiere allocation qui aura pour adresse \param espaceMemoireDebut
	 */
	if(espaceMemoireDebut != NULL){
		if (listeAllocations == NULL){
			listeAllocations = malloc(sizeof(listeAllocation));
			listeAllocations->taille = tailleOctet(nBytes); /*taille en octet*/
			listeAllocations->debutAlloc = espaceMemoireDebut;
			listeAllocations->finAlloc = listeAllocations->debutAlloc + nBytes/8;
			if(listeAllocations->finAlloc >espaceMemoireFin){
				printf("Allocation impossible espace trop grand\n");
				listeAllocations= NULL;
				return listeAllocations;
								
			}
			else{
				listeAllocations->allocSuivant = NULL;
				printf("---------Allocation de %d bits-------------\n",nBytes);
				printf("Zone allouée : %p jusqu'a %p\n",listeAllocations->debutAlloc,listeAllocations->finAlloc);
				return listeAllocations->debutAlloc;
			}	 	
		}
		else{
			/**
		 	 *Si des allocations on deja été faite on cherche la derniere allocation dans notre liste
		 	 *Puis on ajoute la nouvelle allocation à la suite de celle ci
		 	 */
			listeAllocation *copieListe = listeAllocations;
			while(copieListe->allocSuivant!=NULL){
				copieListe = copieListe->allocSuivant;	
			}
			listeAllocation *newAlloc = malloc(sizeof(listeAllocation));		
			newAlloc->taille = tailleOctet(nBytes); //taille en octet
			newAlloc->debutAlloc = copieListe->finAlloc+1;
			newAlloc->finAlloc = newAlloc->debutAlloc + nBytes/8;
			newAlloc->allocSuivant = NULL;
			if(newAlloc->debutAlloc < espaceMemoireFin && newAlloc->finAlloc <espaceMemoireFin){
				copieListe->allocSuivant = newAlloc;
				printf("---------Allocation de %d bits-------------\n",nBytes);
				printf("Zone allouée : %p jusqu'a %p\n",
				newAlloc->debutAlloc,newAlloc->finAlloc);
				return newAlloc->debutAlloc;
			}
			else{	/**
				 *La nouvelle taille à allouer ayant depassé la delimitation de la zone memoire, 
				 *on recherche si il existe un bloc libre suite à une suppression pouvant contenir notre nouvelle allocation,
				 *si oui on effectue l'allocation,
				 *Si non on affiche un message d'erreur.				 
				 */	
				void *bloc = blocLibre(nBytes);
				if(bloc!=NULL){
					newAlloc->debutAlloc = bloc;
					newAlloc->finAlloc = bloc+tailleOctet(nBytes);
					newAlloc->allocSuivant=listeAllocations;
					listeAllocations = newAlloc;
					printf("---------Allocation de %d bits-------------\n",nBytes);
					printf("Zone allouée : %p jusqu'a %p\n",
					newAlloc->debutAlloc,newAlloc->finAlloc);
					return newAlloc->debutAlloc;			
				}
				else{
					printf("Allocation impossible......\n");
					return NULL;
				}		
			}			
		}
	}
	else{
		printf("Zone de memoire non initialisé\n");	
	}	
}
/**
 *cette fonction permet de liberer une allocation faite dans notre zone de memoire.
 *Lorsque une zone est liberée , elle est placée sur une liste tampon nous permettant de faire la gestion de l'espace libre.
 *\param espaceLibres est la liste qui contiennes les zones liberées par les suppressions.
 */
int myfree(void *p){
	if(p == NULL){
		return -1;	
	}
	else{
		listeAllocation *copieListe = listeAllocations;
		listeAllocation *cache;
		int tailleLiberer=0;
		if(copieListe->debutAlloc == p){
			listeAllocations = copieListe->allocSuivant;
			tailleLiberer = copieListe->taille * 8;
			if(espaceLibres==NULL){
				espaceLibres = malloc(sizeof(listeAllocation));
				espaceLibres->taille = copieListe->taille;
				espaceLibres->debutAlloc = copieListe->debutAlloc;
				espaceLibres->finAlloc = copieListe->finAlloc;
				espaceLibres->allocSuivant = NULL;				
			}
			else{
				listeAllocation *copieEspaceLibres = espaceLibres;
				while(copieEspaceLibres->allocSuivant!=NULL){
					copieEspaceLibres = copieEspaceLibres->allocSuivant;				
				}
				listeAllocation *nvEspaceLibre = malloc(sizeof(listeAllocation));
				nvEspaceLibre->taille = copieListe->taille;
				nvEspaceLibre->debutAlloc = copieListe->debutAlloc;
				nvEspaceLibre->finAlloc = copieListe->finAlloc;
				nvEspaceLibre->allocSuivant = NULL;
				copieEspaceLibres->allocSuivant = nvEspaceLibre;
			}
			free(copieListe);
			return tailleLiberer;			
		}
		else{
			while(copieListe!=NULL){
				if(copieListe->allocSuivant->debutAlloc != p){
					copieListe = copieListe->allocSuivant;
				}
				else{
					cache = copieListe->allocSuivant;
					copieListe->allocSuivant= copieListe->allocSuivant->allocSuivant;
					tailleLiberer = cache->taille * 8;
					if(espaceLibres==NULL){
						espaceLibres = malloc(sizeof(listeAllocation));
						espaceLibres->taille = cache->taille;
						espaceLibres->debutAlloc = cache->debutAlloc;
						espaceLibres->finAlloc = cache->finAlloc;
						espaceLibres->allocSuivant = NULL;				
					}
					else{
						listeAllocation *copieEspaceLibres = espaceLibres;
						while(copieEspaceLibres->allocSuivant!=NULL){
							copieEspaceLibres = copieEspaceLibres->allocSuivant;				
						}
						listeAllocation *nvEspaceLibre = malloc(sizeof(listeAllocation));
						nvEspaceLibre->taille = cache->taille;
						nvEspaceLibre->debutAlloc = cache->debutAlloc;
						nvEspaceLibre->finAlloc = cache->finAlloc;
						nvEspaceLibre->allocSuivant = NULL;
						copieEspaceLibres->allocSuivant = nvEspaceLibre;
					}
					free(cache);
					return tailleLiberer;
									
				}		
			}	
		}
	}
	
}
/**
 *cette fonction recherche un bloc de memoire liberer suite à la suppression d'une allocation.
 *La fonction recherchera et retournera un bloc avec la taille la plus petite pouvant satisfaire la nouvelle allocation.
 *Ce bloc servira pour faire une nouvelle allocation dans la zone de memoire
 *Lorsque ce bloc sera allouer il est supprimer sur la liste tampon des zones liberées.
 *\param espaceLibres contiennent les zones liberées.
 *\return p un pointeur sur l'adresse libre pour l'allocation.
 */
void *blocLibre(int nBytes){
	int tailleMin = nBytes;
	void *p = NULL;
	listeAllocation *copieEspaceLibres = espaceLibres;
	while(copieEspaceLibres !=NULL){
		if(copieEspaceLibres->taille*8>=nBytes){
			if(tailleMin<=copieEspaceLibres->taille*8){
				tailleMin = copieEspaceLibres->taille*8;
				p = copieEspaceLibres->debutAlloc;			
			}		
		}
		copieEspaceLibres = copieEspaceLibres->allocSuivant;
	}
	copieEspaceLibres = espaceLibres;
	if(copieEspaceLibres != NULL&&p!=NULL){
		if(copieEspaceLibres->debutAlloc==p){
			espaceLibres = copieEspaceLibres->allocSuivant;
			free(copieEspaceLibres);	
		}
		else{
			while(copieEspaceLibres->allocSuivant->debutAlloc != p){
				copieEspaceLibres = copieEspaceLibres->allocSuivant;		
			}
			listeAllocation *bloc = copieEspaceLibres->allocSuivant;
			copieEspaceLibres->allocSuivant = bloc->allocSuivant;
			free(bloc);	
		}
	}
	return p;
}
/**
 *cette fonction affiche les adresses de debut des allocations effectué,
 *sur notre zone de memoire
 */
void afficherAllocation(){
	listeAllocation *copieListe = listeAllocations;
	while(copieListe!=NULL){
			printf("allocation :%p\n",copieListe->debutAlloc);
			copieListe=copieListe->allocSuivant;		
	}
}

/**
 *cette fonction affiche les adresses de debut des allocations effectué,
 *sur notre zone de memoire
 */
void *rechercherAllocation(void *p){
	listeAllocation *copieListe = listeAllocations;
	while(copieListe!=NULL){
			if(p==copieListe->debutAlloc){
				printf("adresse %p trouver\n",p);
				return copieListe->debutAlloc;
			}
			else{
			copieListe=copieListe->allocSuivant;
			}		
	}
	printf("adresse %p non trouver\n",p);
		return NULL;
}
