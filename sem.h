#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <errno.h>



/*********************************************************************/
/*  Pour Operation P et V 					     */

int sem_id ;
struct sembuf sem_oper_P ;  /* Operation P */
struct sembuf sem_oper_V ;  /* Operation V */

/*********************************************************************/

int seminit(char* path,int nb){
	key_t key;
	if ( (key=ftok(path,'A'))==-1){
		perror("seminit:ftok");
		return -1;
	}

	if ((sem_id=semget(key,nb,IPC_CREAT | 0666))==-1){
		perror("seminit:semget");
		return -1;
	}
	return 1;
}
int libereSem(int smid){
return semctl(smid,0,IPC_RMID,0);
}
 
void P(int semnum) {

sem_oper_P.sem_num = semnum;
sem_oper_P.sem_op  = -1 ;
sem_oper_P.sem_flg = 0 ;
semop(sem_id,&sem_oper_P,1);

}

void V(int semnum) {
sem_oper_V.sem_num = semnum;
sem_oper_V.sem_op  = 1 ;
sem_oper_V.sem_flg  = 0 ;
semop(sem_id,&sem_oper_V,1);
	
}

