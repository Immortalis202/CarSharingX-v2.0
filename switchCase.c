#include "include/commons.h"
#include "include/graphUtility.h"
#include "include/inizio.h"
#include <strings.h>

static t_symstruct lookuptable[] = {
    {"1", 1}, {"2",2}, {"3", 3},{"4", 4},{"5", 5},{"6", 6},{"7", 7},{"8", 8},{"9",9}
    };

#define NKEYS (sizeof(lookuptable)/sizeof(t_symstruct))


int keyFromString(char * key){
    for(int i = 0; i< NKEYS; i++){
        t_symstruct *sym = &lookuptable[i];
        if(strcmp(sym->key, key) == 0)
            return sym->val;
    }
    return -1;
}

void caseSeeCity(){
    allCity(graph);
}

void caseSeeCar(int initConnfdesc){
    char city[32];
    int id;
    char toSend[100];
    int readBytes;
WAITREAD:
    printf("WAITREAD\n");
    bzero(toSend,100);
    sprintf(toSend,"Insert a city to see all the car available");
    write(initConnfdesc, toSend,100);

    read(initConnfdesc, city, 32);
    bzero(city, 32);
    read(initConnfdesc, city, 32 );


    size_t len = strlen(city);
    if(city[strlen(city) -1] != '\n'){
        int dropped = 0;
        while(fgetc(stdin) != '\n')
            dropped++;
        if(dropped > 0){
            sprintf(toSend,"overLimit");
            write(initConnfdesc, toSend,10);
            
            goto WAITREAD;
        }
    }
    if (len > 0 && city[len-1] == '\n') {
        city[--len] = '\0';
    }
    if(strcmp(city, "exit") == 0 || strcmp(city, "quit") == 0){
        return;
    }
    sprintf(toSend,"Searching for the city of %s... \n",city);
    write(initConnfdesc, toSend, 100);
    id = findCity(graph, cityAdded, city);
    if(id == -1){
        printf("City not found\n");
        sprintf(toSend,"City not found");
        write(initConnfdesc, toSend, 15);
        sleep(1);
        bzero(toSend, 100);
        write(initConnfdesc, "gabbopower",11);
        goto WAITREAD;
    }else{
        findCar(graph, id);
        }
    

}

void caseAddCity(int initConnfdesc){}

void caseNewCar(int initConnfdesc){}

void caseRemoveCar(int initConnfdesc){}

void caseRemoveCity(int initConnfdesc){}

void caseItinerary(int initConnfdesc){}

void caseExit(int initConnfdesc){}



int switchChoice(char choice[], int initConnfd){
    int initConnfdesc = initConnfd;
    switch (keyFromString(choice)) {
        case 1: 
            caseSeeCity();
            break;
        case 2: 
            caseSeeCar(initConnfdesc);
            break;
        case 3: 
            caseAddCity(initConnfdesc);
            break;
        case 4: 
            caseNewCar(initConnfdesc);
            break;
        case 5: 
            caseRemoveCar(initConnfdesc);
            break;    
        case 6: 
            caseRemoveCity(initConnfdesc);
            break;
        case 7: 
            caseItinerary(initConnfdesc);
            break;
        case 8: 
            caseExit(initConnfdesc);
            break;

                    
        default:
            return -1;
            
            
    }
}
