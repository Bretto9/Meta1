/* 
 * File:   util.h
 * Author: JoseDavid
 *
 * Created on 21 de octubre de 2014, 20:40
 */

#ifndef UTIL_H
#define	UTIL_H

#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <bitset>         // std::bitset
#include <time.h>
#include <vector>
#include <stdlib.h>

#include "ListaTabu.h"
using namespace std;

int lectura(int **&flujos, int **&distancias, string fichero);

int *potencial(int **v, int &tam);

int mayor(int *v, int tam);

int menor(int *v, int tam);

int coste(int *v, int tam, int **distancias, int **flujos);

int greedy(int **flujos, int **distancias, int *&solGreedy, int nCasos);

int* solInicial(int tam, int seed);

int factorizacion(int* v, int tam, int **flujos, int** distancias, int r, int s);

int *busquedaLocal(int nCasos, int **flujos, int **distancias, int seed);

bool existeVecino(int r, int s, vector<pair<int, int> > vecinos, int tam);

int generacionMejorVecino(ListaTabu lista, int &mejorR, int &mejorS, int nCasos, int* solActual, int **flujos, int **distancias, int k, int seed);

int* largoPlazo(int **frec, int nCasos) ;

int* busquedaTabu(int nCasos, int **flujos, int **distancias, int seed);



#endif	/* UTIL_H */

