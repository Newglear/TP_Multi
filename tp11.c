#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#define N 1000

int jeton = 0;

void f_fils(char* buffP, char* buffF,int* pP, int* pF){
    for(int i=0; i < N/2 ; i++){
        // Lecture
        if(read(pP[0],buffF,5) != 5){
            printf("Erreur de lecture"); 
            exit(-1);
        }
        memcpy(&jeton,&buffF,4);
        printf("%d\n",jeton);
        // Ecriture
        jeton++;
        if(write(pF[1],(char*)&jeton,5)!= 5){
            printf("Erreur d'écriture"); 
            exit(-1);
        }
    }
}
void f_pere(char* buffP, char* buffF,int* pP, int* pF){
    
    for(int i=0; i < N/2 ; i++){
        // Ecriture
        jeton++;
        if(write(pP[1],(char*)&jeton,5) !=5){
            printf("Erreur d'écriture"); 
            exit(-1);
        }
        // Lecture 
        if(read(pF[0],buffP,5) != 5){
            printf("Erreur dde lecture"); 
            exit(-1);
        }
        memcpy(&jeton,&buffP,4);
        printf("%d\n",jeton);
           
    }
    
}
int main(){
    int pPere[2]; 
    int pFils[2];
    char buffP[5]; 
    buffP[0]='\0';
    char buffF[5];
    buffF[0]='\0';
    
    if(pipe(pPere)){
        printf("pb creation pipe\n");
        exit(-1);
    }
    if(pipe(pFils)){
        printf("pb creation pipe\n");
        exit(-1);
    }
    int id; 
    id = fork();
    switch(id){
        case 0:
            printf("PID fils %d:",getpid());
            f_fils(buffP,buffF,pPere,pFils);
            break;
        case -1: 
            printf("Erreur de création");
            break;
        default: 
        
            printf("Processus père: %d",getpid());
            f_pere(buffP,buffF,pPere,pFils);
            break;
        
    }
         
        
}