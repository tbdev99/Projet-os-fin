#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stddef.h>
#include"fonctionAllocations.c"
#include <string.h>
/**
 *C'est la fonction qui va executer le test en mode interactif du programme. 
 */
void menu(){
int option,init,nbBits;
	printf("----Application d'allocation de la memoire----\n");
	printf("----taper 1 pour initialiser la zone de memoire avec le nombre----\n");
	scanf("%d",&init);
	if(init == 1){
		printf("Entrer le nombre de bits à initialiser\n");
		scanf("%d",&nbBits);
		if(nbBits >0){
			printf("Vous avez reservé %d bits\n",initMemory(nbBits));
			printf("Votre zone de memoire: %p jusqu'a %p\n",espaceMemoireDebut,espaceMemoireFin);
			do{
				printf("----taper 2 pour refaire une allocation----\n");
				printf("----taper 3 pour supprimer une allocation----\n");
				printf("----taper 4 pour afficher toutes les allocations----\n");
				printf("----taper 5 pour liberer la memoire et quitter----\n");
				scanf("%d",&option);
				switch(option){
					case 2 : 
						printf("Entrer le nombre de bits à allouer\n");
						scanf("%d",&nbBits);
						if(nbBits >0){
							myalloc(nbBits);						
						}
						break;
					case 3 :
						
						afficherAllocation();
						void *p,*pointeur;
						printf("Entrer l'adresse de l'alloation à supprimer\n");
						scanf("%p",&p);
						pointeur = rechercherAllocation(p);
						printf("%d libérés \n",myfree(pointeur));
						break;
					case 4 :
						afficherAllocation();
						break;
					case 5 :
						printf("La memoire de %d bits initialement alloué est liberée\n",freeMemory());
						break;
					default : 
						 printf("option incorrect\n");								
				}
			}while(option!=5);
		}
	}
}
/**
 *C'est la fonction qui va executer le test en mode ligne de commande du programme.
 *il prend en entrée le nombre d'arguments et les valeurs des arguments passés à l'execution
 *\param argc
 *\param argv 
 */
void menuLigneCommande(int argc,char **argv){
	if(argc>1 && argc <=6){
		for (int i=1;i<argc;i++){
			int nbBits;
			if(strcmp(argv[i],"initMemory")==0){
				printf("Entrer le nombre de bits à initialiser\n");
				scanf("%d",&nbBits);
				if(nbBits >0){
					printf("Vous avez reservé %d bits\n",initMemory(nbBits));
					printf("Votre zone de memoire: %p jusqu'a %p\n",espaceMemoireDebut,espaceMemoireFin);	
				}
			}
			if(strcmp(argv[i],"myalloc")==0){
				printf("Entrer le nombre de bits à allouer\n");
				scanf("%d",&nbBits);
				if(nbBits >0){
					myalloc(nbBits);						
				}
			}
			if(strcmp(argv[i],"myfree")==0){
				afficherAllocation();
				void *p,*pointeur;
				printf("Entrer l'adresse de l'alloation à supprimer\n");
				scanf("%p",&p);
				pointeur = rechercherAllocation(p);
				printf("%d libérés \n",myfree(pointeur));
			}
			if(strcmp(argv[i],"afficherAllocation")==0){
				afficherAllocation();
			}
			if(strcmp(argv[i],"freeMemory")==0){
				printf("La memoire de %d bits initialement alloué est liberée\n",freeMemory());
			}
			
		}
	}
}
/**
 *C'est la fonction qui va executer le test en mode fichier de configuration du programme. 
 */
void modebatch(){
	FILE* fichconf = NULL;
	fichconf = fopen("fichconf","r+");
	char ligne[25];
	if(fichconf != NULL){
		while(fgets(ligne,20,fichconf)!=NULL){
			int nbBits;
			if(strcmp(ligne,"initMemory\n")==0){
				printf("Entrer le nombre de bits à initialiser\n");
				scanf("%d",&nbBits);
				if(nbBits >0){
					printf("Vous avez reservé %d bits\n",initMemory(nbBits));
					printf("Votre zone de memoire: %p jusqu'a %p\n",espaceMemoireDebut,espaceMemoireFin);	
				}
			}
			if(strcmp(ligne,"myalloc\n")==0){
				printf("Entrer le nombre de bits à allouer\n");
				scanf("%d",&nbBits);
				if(nbBits >0){
					myalloc(nbBits);						
				}
			}
			if(strcmp(ligne,"myfree\n")==0){
				afficherAllocation();
				void *p,*pointeur;
				printf("Entrer l'adresse de l'alloation à supprimer\n");
				scanf("%p",&p);
				pointeur = rechercherAllocation(p);
				printf("%d libérés \n",myfree(pointeur));
			}
			if(strcmp(ligne,"afficherAllocation\n")==0){
				afficherAllocation();
			}
			if(strcmp(ligne,"freeMemory\n")==0){
				printf("La memoire de %d bits initialement alloué est liberée\n",freeMemory());
			}		
		}
	}
	else{
		printf("fichier introuvable\n");	
	}
}
