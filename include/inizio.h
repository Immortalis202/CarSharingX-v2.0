
#include "stdbool.h"


typedef struct {
    char *key; 
    int val;
}t_symstruct;


int keyFromString(char * key);

bool alpha(char input[]);

bool isFloat(char input[]);

bool digit(char input[]);

void caseSeeCity();

void caseSeeCar(int initConnfdesc);

void caseAddCity(int initConnfdesc);

void caseNewCar(int initConnfdesc);

void caseRemoveCar(int initConnfdesc);

void caseRemoveCity(int initConnfdesc);

void caseItinerary(int initConnfdesc);

void caseExit(int initConnfdesc);

int choice();

int switchChoice(char choice[], int initConnfd);