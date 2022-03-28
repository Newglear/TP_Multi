#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

void handler(int sig){
    if(sig == 10)
    {
        printf("reception d’un signal usr1: %d\n",sig);
        printf("Meurtre du processus"); 
        exit(0);

    }
    else{
        printf("Mon fils est mort\n");
    }

}

void f_fils(){
    struct sigaction action; /* structure de mise en place du handler */
    /* mise en place du handler */
    action.sa_handler = handler;
    action.sa_flags=0;
    
    sigset_t ens_signal; 
    if(sigfillset(&ens_signal)==-1){
        printf("Impossible de vider les masques\n");
        exit(1);
    }
    if(sigdelset(&ens_signal,SIGUSR1)==-1){
        printf("Impossible d'ajouter le masque\n");
        exit(1);
    } 
    if (sigprocmask(SIG_SETMASK, &ens_signal,NULL)){
        printf("pb mise en place du masque\n");
        exit(-1);
    }
    while (1){
        printf("FILS\n");
        if(sigaction(SIGUSR1, &action, NULL)){
        printf("pb mise en place du handler\n");
        exit(-1);
        }
        sleep(1);
    }

}
void f_pere(int pid_fils){
    struct sigaction action; /* structure de mise en place du handler */
    /* mise en place du handler */
    action.sa_handler = handler;
    action.sa_flags=0;
    
    sigset_t ens_signal; 
    if(sigfillset(&ens_signal)==-1){
        printf("Impossible de vider les masques\n");
        exit(1);
    }
    if(sigdelset(&ens_signal,SIGUSR1)==-1){
        printf("Impossible d'ajouter au masque\n");
        exit(1);
    }
    if(sigdelset(&ens_signal,SIGCHLD)==-1){
        printf("Impossible d'ajouter au masque\n");
        exit(1);
    }
    if (sigprocmask(SIG_SETMASK, &ens_signal,NULL)){
        printf("pb mise en place du masque\n");
        exit(-1);
    }
    sleep(10);
    kill(pid_fils,SIGUSR1);
    while(1){
        printf("PERE\n");
        if(sigaction(SIGUSR1, &action, NULL)){
        printf("pb mise en place du handler\n");
        exit(-1);
        }
        if(sigaction(SIGCHLD, &action, NULL)){
        printf("pb mise en place du handler\n");
        exit(-1);
        }
        sleep(1);
    }
}
int main(){
    
    int id; 
    id = fork();
    switch(id){
        case 0:
            printf("PID fils %d:",getpid());
            f_fils();
            break;
        case -1: 
            printf("Erreur de création");
            break;
        default: 
        
            printf("Processus père: %d",getpid());
            f_pere(id);
            break;
        
    }
         
        
}