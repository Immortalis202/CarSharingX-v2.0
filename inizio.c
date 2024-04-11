#include "include/commons.h"
#include "include/inizio.h"


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
/*===============================================================================================================================//
START
Function to check if alphabetic or digit
START
//===============================================================================================================================*/

bool alpha(char input[]){
    for(int i = 0; i < strlen(input);i++){
        if(isalpha(input[i]) == 0){
            if(input[i] == ' '){
                continue;
            }
            return false;
        }
    }

    return true;
}

bool isFloat(char input[]){
    for(int i = 0; i < strlen(input);i++){
        if(input[i] != '\n'){
            if(((input[i] >= '0' && input[i] <= '9') || input[i] == '.') == 0){
                printf("Not a floating number \n");
                return false;
            }
        }
    }
    return true;
}

bool digit(char input[]){
    for(int i = 0; i < strlen(input);i++){
        if(input[i] != '\n'){
            if(((input[i] >= '0' && input[i] <= '9')) == 0){
                printf("Not an integer number \n");
                return false;
            }
        }
    }
    return true;
}


/*================================================================================================================================//
START
Function to distinguish between user choiche for the software
START
//===============================================================================================================================*/
//CASO 1
//! WORK
//! DONE
void caseSeeCity(){
   
    allCity(graph);
    
}

//CASO 2
//! WORK
void caseSeeCar(int initConnfdesc){
    char city[32];
    int id;
    char toSend[100];
INSERT2:
    sprintf(toSend,"Insert a city to see all the car available \n");
    write(initConnfdesc, toSend,100);
    WAITREAD:
    read(initConnfdesc, city, 32 );
    // fgets(city, 32, stdin);
    size_t len = strlen(city);
    if(city[strlen(city) -1] != '\n'){
        int dropped = 0;
        while(fgetc(stdin) != '\n')
            dropped++;
        if(dropped > 0){
            sprintf(toSend,"overLimit");
            write(initConnfdesc, toSend,100);
            goto WAITREAD;
        }
    }
    if (len > 0 && city[len-1] == '\n') {
        city[--len] = '\0';
    }
    if(alpha(city) == false){
        printf("insert only letter \n");
        goto INSERT2;
    }
    if(strcmp(city, "exit") == 0 || strcmp(city, "quit") == 0){
        return;
    }
    printf("Searching for the city of %s... \n",city);
    id = findCity(graph, cityAdded, city);
    if(id == -1){
        printf("City not found \n");
        goto INSERT2;
    }else{findCar(graph, id);}
    
}
//CASO 3
// ! WORK
void caseAddCity(int initConnfdesc){
    char city[32];
    int id;
    char strLatitude[32];
    double latitude = 0;
    char strLongitude[32];
    double longitude = 0;
    
INSERT3:
    //INPUT CITY
    printf("Insert the name of city you would like to add\n");
    fgets(city, 32, stdin);
    size_t len = strlen(city);
    if(city[strlen(city) -1] != '\n'){
    int dropped = 0;
        while(fgetc(stdin) != '\n')
        dropped++;
    if(dropped > 0){
        printf("Input over the limit \n");
        goto INSERT3;
    }
    }
    if (len > 0 && city[len-1] == '\n') {
        city[--len] = '\0';
    }
    if(alpha(city) == false){
        printf("insert only letter \n");
        goto INSERT3;
    }
    if(strcmp(city, "exit") == 0 || strcmp(city, "quit") == 0){
            return;
        }

    if(findCity(graph, cityAdded, city) != -1){
        printf("City %s already there \n",city);
        goto INSERT3;
    }
INPUTLATITUDE:
    //INPUT LATITUDE
    printf("Insert the latitude of %s\n",city);
    fgets(strLatitude, 32,stdin);
    
    if(isFloat(strLatitude) == false){
        goto INPUTLATITUDE;
    }

    if(strLatitude[strlen(strLatitude) -1] != '\n'){
        int dropped = 0;
            while(fgetc(stdin) != '\n')
            dropped++;
        if(dropped > 0){
            printf("Input over the limit \n");
            goto INPUTLATITUDE;
        }
    }

    latitude = strtod(strLatitude,NULL);
    if(latitude> 90 || latitude < -90){
        printf("Latitude absolute value can't exceed 90 \n");
        goto INPUTLATITUDE;
    }
INPUTLONGITUDE:
    //INPUT LONGITUDE
    printf("Insert the longitude of %s\n",city);
    fgets(strLongitude, 32,stdin);
    if(isFloat(strLongitude) == false){
        goto INPUTLONGITUDE;
    }
    
    if(strLongitude[strlen(strLongitude) -1] != '\n'){
        int dropped = 0;
            while(fgetc(stdin) != '\n')
            dropped++;
        if(dropped > 0){
            printf("Input over the limit \n");
            goto INPUTLONGITUDE;
        }
    }
    longitude = strtod(strLongitude,NULL);
    if(longitude> 180 || longitude < -180){
            printf("Longitude absolute value can't exceed 180 \n");
            goto INPUTLONGITUDE;
        }

    graph[cityAdded] = *createNode(city,cityAdded,latitude,longitude,0);
    createAllEdge(cityAdded, graph );
}
//CASO 4
// ! WORK
void caseNewCar(int initConnfdesc){
    char city[32];
    char car[32];
    char strAutonomy[32];
    long autonomy;
    int id=0;

INSERT4:
    printf("Insert the name of the city where you would like to add the new car\n");
    fgets(city, 32, stdin);
    size_t len = strlen(city);
    if(city[strlen(city) -1] != '\n'){
    int dropped = 0;
        while(fgetc(stdin) != '\n')
            dropped++;
        if(dropped > 0){
            printf("Input over the limit \n");
            goto INSERT4;
        }
    }
    if (len > 0 && city[len-1] == '\n') {
        city[--len] = '\0';
    }
    if(alpha(city) == false){
        printf("insert only letter \n");
        goto INSERT4;
    }
    if(strcmp(city, "exit") == 0 || strcmp(city, "quit") == 0){
        return;
    }
    id = findCity(graph, cityAdded, city);
    if(id == -1){
        printf("City %s doesn't exist\n",city);
        goto INSERT4;
    }

INSERTCAR:
    printf("Insert the name of the car you would like to add\n");
    fgets(car, 32, stdin);
    size_t len2 = strlen(car);
    if(car[strlen(car) -1] != '\n'){
        int dropped = 0;
        while(fgetc(stdin) != '\n')
            dropped++;
        if(dropped > 0){
            printf("Input over the limit \n");
            goto INSERTCAR;
        }
    }
    if (len2 > 0 && car[len2-1] == '\n') {
        car[--len2] = '\0';
    }
    if(alpha(car) == false){
        printf("insert only letter \n");
        goto INSERTCAR;
    }

INPUTAUTONOMY:
    printf("Insert the autonomy of the car: %s\n",car);
    fgets(strAutonomy, 32,stdin);
    
    if(digit(strAutonomy) == false){
        goto INPUTAUTONOMY;
    }

    autonomy = strtol(strAutonomy, NULL,10);

    addCar(&graph[id], car,autonomy);
    createAllEdge(cityAdded, graph );
}
//CASO 5
// ! WORK
void caseRemoveCar(int initConnfdesc){
    char city[32];
    char car[32];
    int id=0;
INPUT5:
    printf("Insert the name of the city where you would like to remove the car\n");
    fgets(city, 32, stdin);
    
    size_t len = strlen(city);
    if(city[strlen(city) -1] != '\n'){
        int dropped = 0;
        while(fgetc(stdin) != '\n')
            dropped++;
        if(dropped > 0){
            printf("Input over the limit \n");
            goto INPUT5;
        }
    }
    if (len > 0 && city[len-1] == '\n') {
        city[--len] = '\0';
    }
    if(alpha(city) == false){
        printf("insert only letter \n");
        goto INPUT5;
    }
    if(strcmp(city, "exit") == 0 || strcmp(city, "quit") == 0){
        return;
    }
    id = findCity(graph, cityAdded, city);

    if(id == -1){
        printf("City %s doesn't exist\n",city);
        goto INPUT5;
    }

INPUTCAR2:
    printf("Insert the name of the car you would like to remove\n");
    fgets(car, 32, stdin);
    size_t len2 = strlen(car);
    if(car[strlen(car) -1] != '\n'){
        int dropped = 0;
        while(fgetc(stdin) != '\n')
            dropped++;
        if(dropped > 0){
            printf("Input over the limit \n");
            goto INPUTCAR2;
        }
    }
    if (len2 > 0 && car[len2-1] == '\n') {
        car[--len2] = '\0';
    }
    if(alpha(car) == false){
        printf("insert only letter \n");
        goto INPUTCAR2;
    }

    removeCar(graph,id, car);
}
//CASO 6
// ! WORK
void caseRemoveCity(int initConnfdesc){
    char city[32];
    int id = 0;
    
INPUT6:
    printf("Insert the name of the city you would like to remove \n");
    fgets(city, 32, stdin);
    size_t len = strlen(city);
    if(city[strlen(city) -1] != '\n'){
        int dropped = 0;
        while(fgetc(stdin) != '\n')
            dropped++;
        if(dropped > 0){
            printf("Input over the limit \n");
            goto INPUT6;
        }
    }
    if (len > 0 && city[len-1] == '\n') {
        city[--len] = '\0';
    }
    if(alpha(city) == false){
        printf("insert only letter \n");
        goto INPUT6;
    }
    if(strcmp(city, "exit") == 0 || strcmp(city, "quit") == 0){
        return;
    }

    id = findCity(graph, cityAdded, city);
    if(id == -1){
        printf("City %s doesn't exists",city);
        goto INPUT6;
    }
    removeCity(graph, id);

    printf("Removed with success city %s",city);
}
//CASO 7
// ! WORK
void caseItinerary(int initConnfdesc){
    char cityStart[32];
    char cityEnd[32];
    int idStart = 0;
    int idEnd = 0;
INPUT7:
    printf("Insert the name of the city to start your itinerary \n");
    fgets(cityStart, 32, stdin);
    size_t len = strlen(cityStart);
    if(cityStart[strlen(cityStart) -1] != '\n'){
        int dropped = 0;
        while(fgetc(stdin) != '\n')
            dropped++;
        if(dropped > 0){
            printf("Input over the limit \n");
            goto INPUT7;
        }
    }
    if (len > 0 && cityStart[len-1] == '\n') {
        cityStart[--len] = '\0';
    }
    if(alpha(cityStart) == false){
        printf("insert only letter \n");
        goto INPUT7;
    }
    if(strcmp(cityStart, "exit") == 0 || strcmp(cityStart, "quit") == 0){
        return;
    }

    idStart = findCity(graph, cityAdded, cityStart);
    if(idStart == -1){
        printf("Cannot find %s\n",cityStart);
        goto INPUT7;
    }
INPUT8:
    printf("Insert the name of the city to end your itinerary \n");
    fgets(cityEnd, 32, stdin);
    size_t len2 = strlen(cityEnd);
    if(cityEnd[strlen(cityEnd) -1] != '\n'){
        int dropped = 0;
        while(fgetc(stdin) != '\n')
            dropped++;
        if(dropped > 0){
            printf("Input over the limit \n");
            goto INPUT7;
        }
    }
    if (len2 > 0 && cityEnd[len2-1] == '\n') {
        cityEnd[--len2] = '\0';
    }
    idEnd = findCity(graph, cityAdded, cityEnd);
    if(alpha(cityEnd) == false){
        printf("insert only letter \n");
        goto INPUT8;;
    }
    if(idEnd == -1){
        printf("Cannot find %s\n",cityEnd);
        goto INPUT8;
    }
    astar(graph,idStart,idEnd,cityAdded);
}
//CASO 8
// ! WORK
void caseExit(int initConnfdesc){
    printf("Thank you for choosing CAR PARKING X");
    exit(1);
}
/*================================================================================================================================//
END
Function to distinguish between user choiche for the software
END
//===============================================================================================================================*/


int choice(){
    char choice[32];

INSERT:
    printf("Scegli un numero tra 1 e 8 \n");
    fgets(choice, 2, stdin);
    //getchar();
    if(choice[strlen(choice) -1] != '\n'){
        int dropped = 0;
        while(fgetc(stdin) != '\n')
            dropped++;
        if(dropped > 0){
            printf("Input over the limit \n");
            goto INSERT;
        }
    }
    return 1;
}

int switchChoice(char choice[], int initConnfd){
    int initConnfdesc = initConnfd;
    switch (keyFromString(choice)) {
        case 1: 
            caseSeeCity();
            return 1;
            break;
        case 2: 
            caseSeeCar(initConnfdesc);
            return 1;

        case 3: 
            caseAddCity(initConnfdesc);
                        return 1;

        case 4: 
            caseNewCar(initConnfdesc);
                        return 1;

        case 5: 
            caseRemoveCar(initConnfdesc);
            return 1;
        case 6: 
            caseRemoveCity(initConnfdesc);
            return 1;
        case 7: 
            caseItinerary(initConnfdesc);
            return 1;
        case 8: 
            caseExit(initConnfdesc);
            return 1;

                    
        default:
            return -1;
            
            
    }
}
