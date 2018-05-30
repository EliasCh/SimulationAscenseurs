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

#include"sem.h"
#include"file.h"
#define ui unsigned int 
#define NB_TECH 4

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
	perror("Erreur de creation de la clé \n");
	exit(1);
    }

    if ((td = msgget(key, 0750 | IPC_CREAT | IPC_EXCL)) == -1) {
	perror("Erreur de creation de la file\n");
	exit(1);
    }


printf("asc\n");
//valeur par défaut de nbt:nb techniciens , nbr:nb résidents
ascenseur1();
ascenseur2();
ascenseur3();

//shared memory 
int* nd ;

int shmid = shmget(IPC_PRIVATE, sizeof(int), 0666);
nd = (int *)shmat(shmid, NULL, 0);
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
	printf("%d:Je gère\n",(i++));
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



