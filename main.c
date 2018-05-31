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

#include"sem.h"
#include"file.h"
#define ui unsigned int 
#define NB_TECH 4

typedef struct {
        long  type;
        int dest;
		pid_t who;
        } dm;

int sem_id ;

void ascenseur1(){
	if(!fork()){
/*		while(1){
			int max=getMax();
			for(int i=0;i<max;i++){
				
				sleep(1);
				//if (i==element) drop 
			}
			//sleep P(X);w
		}
*/
		
		exit(0);
	}
	else 
		wait(0);
}
void ascenseur2(){
if(!fork()){
exit(0);
}
else wait(0);
}
void ascenseur3(){
if(!fork()){
exit(0);
}
else wait(0);
}
int td;
void clearFile(int sig){
   msgctl(td, IPC_RMID, NULL);
	libereSem(sem_id);
	signal(SIGINT,SIG_DFL);
	raise(SIGINT);
}
int main() {
//gestion signaux 
	signal(SIGINT,clearFile);

seminit("/home/",3);

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


printf("td=%d\n",td);
//valeur par défaut de nbt:nb techniciens , nbr:nb résidents
ascenseur1();
ascenseur2();
ascenseur3();

//shared memory 
int* nd ;
/* Shared status files 
int* idAsc1;int* idAsc2;int* idAsc3;
bool* dispo1;bool* dispo2;bool* dispo3;
(*dispo1)=true;
(*dispo2)=true;
(*dispo3)=true;
*/
int shmid = shmget(IPC_PRIVATE, sizeof(int), 0666);
nd = (int *) shmat(shmid, NULL, 0);
/*
shmid = shmget(IPC_PRIVATE, sizeof(int), 0666);
idAsc1 = (int *) shmat(shmid, NULL, 0);
shmid = shmget(IPC_PRIVATE, sizeof(int), 0666);
idAsc2 = (int *) shmat(shmid, NULL, 0);
shmid = shmget(IPC_PRIVATE, sizeof(int), 0666);
idAsc3 = (int *) shmat(shmid, NULL, 0);

shmid = shmget(IPC_PRIVATE, sizeof(bool), 0666);
dispo1 = (bool*) shmat(shmid, NULL, 0);

shmid = shmget(IPC_PRIVATE, sizeof(bool), 0666);
dispo2 = (bool*) shmat(shmid, NULL, 0);

shmid = shmget(IPC_PRIVATE, sizeof(bool), 0666);
dispo3 = (bool*) shmat(shmid, NULL, 0);
*/
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
//	if( (*dispo1)){
	dm message;
	if( msgrcv(td, &message, sizeof(dm) - 4, 1,0) == -1) 
	    perror("Erreur de lecture reponse serveur\n");	
	printf("%d:qqn a mis(dest=%d,who=%ld)\n",(i++),message.dest,(long) message.who);

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



