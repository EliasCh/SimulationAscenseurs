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

typedef struct {
        long  type;
        int dest;
		pid_t who;
		char* txt;
        } dm;

int sem_id,td,ta;
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
void ascenseur1(int ta,int** dispo1){
	if(!fork()){
	
	int asc1=open("asc1.txt",O_WRONLY|O_CREAT,0666);
	close(1);
	dup(asc1);
	while(1){
			P(3);	
			printf("Asc1:Ouverture porte niv0\n");
			if((*(*dispo1))==0) continue;
			int arr[5]={0,0,0,0,0};
			for(int i=0;i<5;i++){
				sleep(1);	
				dm message;	
				if( msgrcv(ta, &message, sizeof(dm) - 4, 1,0) == -1) 
						perror("Erreur de lecture reponse dans l'asc2\n");
				arr[i]=message.dest;
				printf("Asc1: dest %d,message=%s,from%ld\n",message.dest,message.txt,(long )message.who);
				if( i==4) 			(*(*dispo1))=0;		
				V(4);		
				//printf("********* i=%d\n",i);	
			}	
			selectionSort(arr,5);
			printf("arr: {%d,%d,%d,%d,%d}\n",arr[0],arr[1],arr[2],arr[3],arr[4]);
			int prv=0;		
			for(int i=0;i<5;i++){				
				printf("\ndeplacement vers %d\n",arr[i]);
				for(int j=prv;j<arr[i];j++){
					sleep(1);
					printf("-");
				}		
				prv=arr[i];
			}
			printf("Asc1:retour vers niv0\n");
			sleep(arr[4]);
			(*(*dispo1))=1;

		}
		
		exit(0);
	}
}
//5-6
void ascenseur2(int ta,int** dispo2){
if(!fork()){
	int asc2=open("asc2.txt",O_WRONLY|O_CREAT,0666);
	close(1);
	dup(asc2);
		
	while(1){
			P(5);	
			printf("Asc2:Ouverture porte niv0\n");
			if((*(*dispo2))==0) continue;		
			for(int i=0;i<3;i++){
				sleep(1);	
				dm message;	
				if( msgrcv(ta, &message, sizeof(dm) - 4, 2,0) == -1) 
						perror("Erreur de lecture reponse dans l'asc2\n");
				printf("Asc2: dest %d,message=%s,from%ld\n",message.dest,message.txt,(long )message.who);
				
				V(6);		
				//printf("********* i=%d\n",i);	
			}	
		
								(*(*dispo2))=0;

		}

exit(0);
}
}

void ascenseur3(int ta,int** dispo3){
if(!fork()){
	int asc3=open("asc3.txt",O_WRONLY|O_CREAT,0666);
	close(1);
	dup(asc3);
	
	while(1){
			P(10);	
			printf("Asc3:Ouverture porte niv0\n");
			if(			(*(*dispo3))==0) continue;
			for(int i=0;i<2;i++){
				sleep(1);	
				dm message;	
				if( msgrcv(ta, &message, sizeof(dm) - 4, 3,0) == -1) 
						perror("Erreur de lecture reponse dans l'asc3\n");
				printf("Asc3: dest %d,message=%s,from%ld\n",message.dest,message.txt,(long )message.who);
				V(11);		
				//printf("********* i=%d\n",i);	
			}	
		
								(*(*dispo3))=0;

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
ascenseur1(ta,&dispo1);
ascenseur2(ta,&dispo2);
ascenseur3(ta,&dispo3);
										// 0 -> nombre délivreures , 1 sémaphore d'attente , 2 COUPETIF
//creation des delivreures 
V(0);V(1);V(7);V(8);V(9);
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
		exit(0);
	}
}
int i=0;

while(1){
	P(2);
	dm message;
	if( msgrcv(td, &message, sizeof(dm) - 4, 1,0) == -1) 
	    perror("Erreur de lecture reponse serveur\n");	
	//printf("%d:qqn a mis(dest=%d,who=%ld)\n",(i++),message.dest,(long) message.who);

P(7);	
if( (*dispo1)){
V(7);	
//	printf("asc1 dispo\n");	
		dm masc;
		masc.type=1;
		masc.dest=message.dest;
		masc.who=message.who;
		masc.txt=message.txt;
   	    if (msgsnd(ta, &masc, sizeof(dm) - 4,0) == -1) {
			printf("n'as pas pu mettre message\n");
	    }
		V(3);//CES V(3) vont générer un excès du sémaphore 3 
		P(4);
	continue;
	}
	else V(7);
	P(8);
	if( (*dispo2)){
		V(8);	
		//printf("asc2 dispo\n");
		dm masc;
		masc.type=2;
		masc.dest=message.dest;
		masc.who=message.who;
		masc.txt=message.txt;
   	    if (msgsnd(ta, &masc, sizeof(dm) - 4,0) == -1) {
			printf("n'as pas pu mettre message\n");
	    }
		V(5);//CES V(5) vont générer un excès du sémaphore 5 
		P(6);	
		continue;	
	}
	else V(8);
	P(9);
	if( (*dispo3))
	{
		V(9);
	//	printf("asc3 dispo\n");	
		dm masc;
		masc.type=3;
		masc.dest=message.dest;
		masc.who=message.who;
		masc.txt=message.txt;
   	    if (msgsnd(ta, &masc, sizeof(dm) - 4,0) == -1) {
			printf("n'as pas pu mettre message\n");
	    }
		V(10);//CES V(5) vont générer un excès du sémaphore 5 
		P(11);	
	//continue;
	}
	else V(9);
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




