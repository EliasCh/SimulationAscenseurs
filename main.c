#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdbool.h>
#include<string.h>
#include<fcntl.h>
#include"sem.h"
//#include"file.h"
#define ui unsigned int 
#define NB_TECH 4
#define MAX1 5
#define MAX2 5
#define MAX3 5

typedef struct {
        long  type;
        int dest;
		pid_t who;
		char* txt;
        } dm;

int sem_id,td,ta,da;
void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
void translaterArr(int arr[],int dep,int n){
	for(int i=dep;i<(n-1);i++)
		arr[i]=arr[i+1];
	arr[n-1]=0;
}
int nombreLibre(int arr[],int n){
	int l=0;
	for(int i=0;i<n;i++)
		if(arr[i]==0) l++;
	return l;
}
int posZero(int arr[],int n){
	for(int i=0;i<n;i++)
		if(arr[i]==0) return i;
return -1;
}
int allZeros(int arr[],int n){
	for(int i=0;i<n;i++)
		if(arr[i]!=0) return 0;
	return 1;
}
void selectionSort(int arr[], int n)
{
    int i, j, min_idx;
 
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++)
          if (arr[j] < arr[min_idx])
            min_idx = j;
 
        // Swap the found minimum element with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}
int min(long a,int b){
	if(a<b) return (int) a;
	return (int) b;
}
int max(long a,int b){
	if(a>b) return (int) a;
	return (int) b;
}
void ascenseur1(int ta,int da){
	if(!fork()){
	
	int asc1=open("asc1.txt",O_WRONLY|O_CREAT,0666);
	close(1);
	dup(asc1);
	int dep=0;
	long term=0;
	dm mid;				
/*	if( msgrcv(ta, &mid, sizeof(dm) - 4, 1,IPC_NOWAIT) == -1) {
		perror("a pas pu lire l'id du terminal\n");
	}
	else
		printf("pid=%ld\n",term);
*/	int arr[5]={0,0,0,0,0};
	while(1){
				printf("Asc1:Ouverture porte niv0\n");
				printf("Attendre si quelqu'un veut monter a l'etage 0\n");
				for(int i=0;i<5;i++){
					sleep(1);				
					dm message;				
					if( msgrcv(ta, &message, sizeof(dm) - 4, 1,IPC_NOWAIT) != -1) {
						arr[posZero(arr,5)]=message.dest;	
					}
				}	
			selectionSort(arr,5);
			printf("arr: {%d,%d,%d,%d,%d}\n",arr[0],arr[1],arr[2],arr[3],arr[4]);
			long prv=0;		
			while(!allZeros(arr,5)){				
				printf("\ndeplacement vers %d depuis%ld\n",arr[0],prv);
				for(int j=min(prv,arr[0]);j<max(prv,arr[0])-1;j++){
					sleep(1);
					dep++;
					printf("-");
					/*if(dep==MAX1){
						printf("panne a %d,pid=%ld",j,message.who);
						dm mpanne;
						mpanne.type=4;
						mpanne.dest=j;
						mpanne.who=getpid();
						mpanne.txt="";
				   	    if (msgsnd(ta, &mpanne, sizeof(dm) - 4,0) == -1) {
							printf("n'as pas pu mettre message de panne\n");
						}	
									
					}*/
						
				}
				prv=arr[0];
				printf("prv=%ld\n",prv);
				while(arr[0]==arr[1]){
					translaterArr(arr,0,5);		
				}
				translaterArr(arr,0,5);
				printf("arr: {%d,%d,%d,%d,%d}\n",arr[0],arr[1],arr[2],arr[3],arr[4]);	
				int x=nombreLibre(arr,5);
				printf("Attendre si quelqu'un veut monter a l'etage %ld\n",prv);
				if(prv!=0){
					for(int i=0;i<x;i++){
						sleep(1);				
						dm message;		
					printf("prv=%ld\n",prv);		
						if( msgrcv(da, &message, sizeof(dm) - 4, prv,IPC_NOWAIT) != -1) {
							arr[posZero(arr,5)]=message.dest;	
							printf("prv=%ld=type=%ld\n",prv,message.type);
						}
					}	
				}
			}
			printf("Asc1:retour vers niv0\n");
			for(int i=0;i<prv;i++){
				printf("-");				
				sleep(1);
			}

		}
		
		exit(0);
	}
}
//5-6
void ascenseur2(int ta,int da){
	if(!fork()){
	
	int asc2=open("asc2.txt",O_WRONLY|O_CREAT,0666);
	close(1);
	dup(asc2);
	int arr[3]={0,0,0};
	while(1){
				printf("Asc2:Ouverture porte niv0\n");
				printf("Attendre si quelqu'un veut monter a l'etage 0\n");
				for(int i=0;i<3;i++){
					sleep(1);				
					dm message;				
					if( msgrcv(ta, &message, sizeof(dm) - 4, 2,IPC_NOWAIT) != -1) {
						arr[posZero(arr,3)]=message.dest;	
					}
				}	
			selectionSort(arr,3);
			printf("arr: {%d,%d,%d}\n",arr[0],arr[1],arr[2]);
			long prv=0;		
			while(!allZeros(arr,3)){				
				printf("\ndeplacement vers %d depuis%ld\n",arr[0],prv);
				for(int j=min(prv,arr[0]);j<max(prv,arr[0])-1;j++){
					sleep(1);
					printf("-");
				}
				prv=arr[0];
				printf("prv=%ld\n",prv);
				while(arr[0]==arr[1]){
					translaterArr(arr,0,3);		
				}
				translaterArr(arr,0,3);
				printf("arr: {%d,%d,%d}\n",arr[0],arr[1],arr[2]);	
				int x=nombreLibre(arr,3);
				printf("Attendre si quelqu'un veut monter a l'etage %ld\n",prv);
				if(prv!=0){
					for(int i=0;i<x;i++){
						sleep(1);				
						dm message;		
					printf("prv=%ld\n",prv);		
						if( msgrcv(da, &message, sizeof(dm) - 4, prv,IPC_NOWAIT) != -1) {
							arr[posZero(arr,3)]=message.dest;	
							printf("prv=%ld=type=%ld\n",prv,message.type);
						}
					}	
				}
			}
			printf("Asc2:retour vers niv0\n");
			for(int i=0;i<prv;i++){
				printf("-");				
				sleep(1);
			}

		}
		
		exit(0);
	}

}

void ascenseur3(int ta,int da){
	if(!fork()){
	
	int asc3=open("asc3.txt",O_WRONLY|O_CREAT,0666);
	close(1);
	dup(asc3);
	int arr[2]={0,0};
	while(1){
				printf("Asc3:Ouverture porte niv0\n");
				printf("Attendre si quelqu'un veut monter a l'etage 0\n");
				for(int i=0;i<2;i++){
					sleep(1);				
					dm message;				
					if( msgrcv(ta, &message, sizeof(dm) - 4, 3,IPC_NOWAIT) != -1) {
						arr[posZero(arr,2)]=message.dest;	
					}
				}	
			selectionSort(arr,2);
			while(arr[0]==0){
					translaterArr(arr,0,2);		
				}
			printf("arr: {%d,%d}\n",arr[0],arr[1]);
			long prv=0;		
			while(!allZeros(arr,2)){				
				printf("\ndeplacement vers %d depuis%ld\n",arr[0],prv);
				for(int j=min(prv,arr[0]);j<max(prv,arr[0])-1;j++){
					sleep(1);
					printf("->");
				}
				prv=arr[0];
				printf("prv=%ld\n",prv);
				while(arr[0]==arr[1]){
					translaterArr(arr,0,2);		
				}
				translaterArr(arr,0,2);
				printf("arr: {%d,%d}\n",arr[0],arr[1]);	
				int x=nombreLibre(arr,2);
				printf("Attendre si quelqu'un veut monter a l'etage %ld\n",prv);
				if(prv!=0){
					for(int i=0;i<x;i++){
						sleep(1);				
						dm message;		
					printf("prv=%ld\n",prv);		
						if( msgrcv(da, &message, sizeof(dm) - 4, prv,IPC_NOWAIT) != -1) {
							arr[posZero(arr,2)]=message.dest;	
							printf("prv=%ld=type=%ld\n",prv,message.type);
						}
					}	
				}
			}
			printf("Asc3:retour vers niv0\n");
			for(int i=0;i<prv;i++){
				printf("->");				
				sleep(1);
			}

		}
		
		exit(0);
	}

}

void clearFile(int sig){
   msgctl(td, IPC_RMID, NULL);
	libereSem(sem_id);
		fclose(fopen("asc3.txt","w"));
	fclose(fopen("asc2.txt","w"));
	fclose(fopen("asc1.txt","w"));
	signal(SIGINT,SIG_DFL);
	raise(SIGINT);
}
int main() {
//gestion signaux 
	signal(SIGINT,clearFile);

seminit("/home/",11);

//une file
    key_t key;	
    int longMSG ;	    
	  if ((key = ftok("/home/machine/lo41", 'A')) == -1) {
	clearFile(2);	
	perror("Erreur de creation de la clé \n");
	exit(1);
    }

    if ((td = msgget(key, 0750 | IPC_CREAT | IPC_EXCL)) == -1) {
	clearFile(2);
	perror("Erreur de creation de la file\n");
	exit(1);
    }

    if ((ta = msgget(IPC_PRIVATE, 0750 | IPC_CREAT )) == -1) {
	clearFile(2);
	perror("Erreur de creation de la file\n");
	exit(1);
    }
	/*dm mid;
	mid.type=1;
	mid.dest=0;
	mid.who=getpid();
	mid.txt="";
	if (msgsnd(ta, &mid, sizeof(dm) - 4,0) == -1) {
		printf("n'as pas pu mettre message de panne\n");
	}
	*/
 	if ((da = msgget(IPC_PRIVATE, 0750 | IPC_CREAT )) == -1) {
	clearFile(2);
	perror("Erreur de creation de la file\n");
	exit(1);
    }
//shared memory 
int* nd ;
// Shared status files 
int* dispo1;int* dispo2;int* dispo3;
int shmid = shmget(IPC_PRIVATE, sizeof(int), 0666);
nd = (int *) shmat(shmid, NULL, 0);

shmid = shmget(IPC_PRIVATE, sizeof(int), 0666);
dispo1 = (int*) shmat(shmid, NULL, 0);

shmid = shmget(IPC_PRIVATE, sizeof(bool), 0666);
dispo2 = (int*) shmat(shmid, NULL, 0);

shmid = shmget(IPC_PRIVATE, sizeof(bool), 0666);
dispo3 = (int*) shmat(shmid, NULL, 0);
(*dispo1)=1;
(*dispo2)=1;
(*dispo3)=1;



printf("td=%d\n",td);
//V(3);
//valeur par défaut de nbt:nb techniciens , nbr:nb résidents
ascenseur1(ta,da);
ascenseur2(ta,da);
ascenseur3(ta,da);
										// 0 -> nombre délivreures , 1 sémaphore d'attente , 2 COUPETIF
//creation des delivreures 
V(0);V(1);V(8);V(9);
for(int i=0;i<10;i++){
	if(!fork()){
		P(0);
		(*nd)++;
		V(0);
		P(0);
		if((*nd)!=1){
			V(0);
			P(1);
		}
		else 
			V(0);
//		printf("child%d:\n",(i++));
		//put msg to terminal
		dm message;
		message.type=1;
		message.dest=(i+1);
		message.who=getpid();
		message.txt="from child";
	   if (msgsnd(td, &message, sizeof(dm) - 4,0) == -1) {
			printf("n'as pas pu mettre message\n");
	    }

		V(2);

		P(0);
		if((*nd)>1){
			V(1);
		}
		(*nd)--;
		V(0);
		if(i==1) {
			dm messages;
			messages.type=6;
			messages.dest=1;
			messages.who=getpid();
			messages.txt="from child";
			if (msgsnd(da, &messages, sizeof(dm) - 4,0) == -1) {
			printf("n'as pas pu mettre message retour\n");
	    	}
		}
		exit(0);
	}
}
int i=0;

while(1){
	printf("pid=%ld\n",getpid());
	P(2);
	dm message;
	if( msgrcv(td, &message, sizeof(dm) - 4, 1,0) == -1) 
	    perror("Erreur de lecture reponse serveur\n");	
	//printf("%d:qqn a mis(dest=%d,who=%ld)\n",(i++),message.dest,(long) message.who);
	
if((message.dest%3)==0){

//	printf("asc1 dispo\n");	
		dm masc;
		masc.type=1;
		masc.dest=message.dest;
		masc.who=message.who;
		masc.txt=message.txt;
   	    if (msgsnd(ta, &masc, sizeof(dm) - 4,0) == -1) {
			printf("n'as pas pu mettre message\n");
	    }
	}
if((message.dest%3)==1){
		//printf("asc2 dispo\n");
		dm masc;
		masc.type=2;
		masc.dest=message.dest;
		masc.who=message.who;
		masc.txt=message.txt;
   	    if (msgsnd(ta, &masc, sizeof(dm) - 4,0) == -1) {
			printf("n'as pas pu mettre message\n");
	    }		
	}
	if(((message.dest%3)==2))
	{
	//	printf("asc3 dispo\n");	
		dm masc;
		masc.type=3;
		masc.dest=message.dest;
		masc.who=message.who;
		masc.txt=message.txt;
   	    if (msgsnd(ta, &masc, sizeof(dm) - 4,0) == -1) {
			printf("n'as pas pu mettre message\n");
	    }
	
	//continue;
	}
	printf("pere\n");
}
/*Msg msg;
long getMsg;
P(1);
	if ((getMsg =
	     msgrcv(td, &msg, sizeof(Msg) - 4, 1,
		    0)) == -1) {
	    perror("Erreur de lecture message (main:70) \n");
	    exit(1);
	}
printf("msg:%s\n",msg.msg);
V(0);
 wait(0);
*/
   msgctl(td, IPC_RMID, NULL);
libereSem(sem_id);
return 0;
}


/* 
//appel des 'd' d'une autre fichier
if(!fork()){
	char tdc[20];
	snprintf(tdc,sizeof(tdc),"%d",td);
	execl("./test","test","1",tdc,NULL);
	exit(0);
}
*/




