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

void ascenseur1(int ta,int** dispo1){
	if(!fork()){
	while(1){
			P(3);	
			printf("Asc1:Ouverture porte niv0\n");
			for(int i=0;i<5;i++){
				sleep(1);	
				dm message;	
				if( msgrcv(ta, &message, sizeof(dm) - 4, 1,0) == -1) 
						perror("Erreur de lecture reponse dans l'asc\n");
				printf("Asc1: dest %d,message=%s,from%ld\n",message.dest,message.txt,(long )message.who);
				V(4);			
			}	
		
								(*(*dispo1))=0;

		}
		
		exit(0);
	}
}
void ascenseur2(int ta){
if(!fork()){
exit(0);
}
}
void ascenseur3(int ta){
if(!fork()){
exit(0);
}
}

void clearFile(int sig){
   msgctl(td, IPC_RMID, NULL);
	libereSem(sem_id);
	signal(SIGINT,SIG_DFL);
	raise(SIGINT);
}
int main() {
//gestion signaux 
	signal(SIGINT,clearFile);

seminit("/home/",5);

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
V(3);
//valeur par défaut de nbt:nb techniciens , nbr:nb résidents
ascenseur1(ta,&dispo1);
ascenseur2(ta);
ascenseur3(ta);
										// 0 -> nombre délivreures , 1 sémaphore d'attente , 2 COUPETIF
//creation des delivreures 
V(0);V(1);
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
printf("child%d:\n",(i++));
		//put msg to terminal
		dm message;
		message.type=1;
		message.dest=1;
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
	printf("%d:qqn a mis(dest=%d,who=%ld)\n",(i++),message.dest,(long) message.who);
	if( (*dispo1)){
	printf("asc1 dispo\n");	
		dm masc;
		masc.type=1;
		masc.dest=1;
		masc.who=message.who;
		masc.txt=message.txt;
   	    if (msgsnd(ta, &masc, sizeof(dm) - 4,0) == -1) {
			printf("n'as pas pu mettre message\n");
	    }
		V(3);
		P(4);

	continue;
	}
if( (*dispo2)){
	printf("asc2 dispo\n");
	continue;	
}
if( (*dispo3)){
	printf("asc3 dispo\n");	
	continue;
}

printf("pere\n");}
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



