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

int* solInicial(int tam);

int factorizacion(int* v, int tam, int **flujos, int** distancias, int r, int s);

int *busquedaLocal(int nCasos, int **flujos, int **distancias);

bool existeVecino(int r, int s, vector<pair<int, int> > vecinos, int tam);

int generacionMejorVecino(ListaTabu lista, int &mejorR, int &mejorS, int nCasos, int* solActual, int **flujos, int **distancias, int k);

int* largoPlazo(int **frec, int nCasos) ;

int* busquedaTabu(int nCasos, int **flujos, int **distancias);

int main(int argc, char** argv) {

    int **flujos, **distancias;
    string fichero = "dat/els19.dat";
    string ficheros[20] = {"dat/els19.dat", "dat/chr20a.dat", "dat/chr25a.dat", "dat/nug25.dat",
        "dat/bur26a.dat", "dat/bur26b.dat", "dat/tai30a.dat", "dat/tai30b.dat",
        "dat/esc32a.dat", "dat/kra32.dat", "dat/tai35a.dat", "dat/tai35b.dat",
        "dat/tho40.dat", "dat/tai40a.dat", "dat/sko42.dat", "dat/sko49.dat",
        "dat/tai50a.dat", "dat/tai50b.dat", "dat/tai60a.dat", "dat/lipa90a.dat"};
    for (int i = 0; i < 20; i++) {
        fichero = ficheros[i];

        cout << "Leyendo fichero... " << fichero << endl;
        int nCasos = lectura(flujos, distancias, fichero);

        // GREEDY
        int *solGreedy;
        int costo = greedy(flujos, distancias, solGreedy, nCasos);
        cout << "Coste del algoritmo GREEDY para el fichero( " << i + 1 << " ) " << fichero << " es:" << costo << endl;

        // LOCAL
        int *solLocal = busquedaLocal(nCasos, flujos, distancias);
        costo = coste(solLocal, nCasos, distancias, flujos);
        cout << "Coste del algoritmo LOCAL para el fichero( " << i + 1 << " ) " << fichero << " es:" << costo << endl;

        // TABU
        int *solTabu = busquedaTabu(nCasos, flujos, distancias);
        costo = coste(solTabu, nCasos, distancias, flujos);
        cout << "Coste del algoritmo TABU para el fichero " << fichero << " es: " << costo << endl << endl;

        for (int j = 0; j < nCasos; j++) {
            delete[] flujos[j];
            delete[] distancias[j];
        }
        delete[] flujos;
        delete[] distancias;
        delete[] solGreedy;
        delete[] solLocal;
        delete[] solTabu;
    }
    return 0;
}


// DESARROLLO Y COMENTARIO DE LAS FUNCIONES

/**
 * Rellena la matriz de flujos y distancias a partir de un fichero dado
 * @param flujos Matriz de flujos a llenar
 * @param distancias Matriz de distancias a llenar
 * @param fichero Fichero del que leer
 * @return nCasos numero de filas y columnas que tiene las matrices
 */
int lectura(int **&flujos, int **&distancias, string fichero) {
    string temp;
    ifstream flujo(fichero.c_str());
    int nCasos = 0;
    if (flujo.is_open()) {
        if (flujo.good()) {
            getline(flujo, temp);
            nCasos = atoi(temp.c_str());
            flujos = new int*[nCasos];
            for (int i = 0; i < nCasos; i++) {
                flujos[i] = new int[nCasos];
            }

            distancias = new int*[nCasos];
            for (int i = 0; i < nCasos; i++) {
                distancias[i] = new int[nCasos];
            }

            int tempi = 0;
            int tempj = 0;
            int numero;
            bool fin = false;
            while (getline(flujo, temp) && !fin) {
                if (temp.length() > 0) {
                    stringstream lectura(temp);
                    while (lectura >> numero) {
                        flujos[tempi][tempj] = numero;
                        tempj++;
                        if (tempj == nCasos) {
                            tempj = 0;
                            tempi++;
                        }
                    }
                }
                if (tempi >= nCasos) {
                    fin = true;
                }
            }
            tempi = 0;
            tempj = 0;
            fin = false;
            while (getline(flujo, temp) && !fin) {
                if (temp.length() > 0) {
                    stringstream lectura(temp);
                    while (lectura >> numero) {
                        distancias[tempi][tempj] = numero;
                        tempj++;
                        if (tempj == nCasos) {
                            tempj = 0;
                            tempi++;
                        }
                    }
                }
                if (tempi >= nCasos) {
                    fin = true;
                }
            }
            return nCasos;
        } else {
            cout << "Error opening file";
            return -1;
        }
    }
}

/**
 * Calcula el vector de potenciales
 * @param v Matriz a partir de la cual calcular los potenciales
 * @param tam Numero de casos a evaluar
 * @return Vector de potenciales
 */
int *potencial(int **v, int &tam) {
    int *pot = new int[tam];

    for (int i = 0; i < tam; i++) {
        pot[i] = 0;
        for (int j = 0; j < tam; j++) {
            pot[i] += v[j][i];
        }
    }
    return pot;
}

/**
 * Dado un vector de enteros, devuelve la posicion del mayor de sus elementos
 * @param v Vector en el que buscar
 * @param tam Tamaño del vector
 * @return Posicion del mayor dato
 */
int mayor(int *v, int tam) {
    if (tam <= 0) {
        return -1;
    } else if (tam == 1) {
        return v[0];
    } else if (tam > 1) {
        int m = 0;
        for (int i = 1; i < tam; i++) {
            if (v[i] > v[m]) {
                m = i;
            }
        }
        return m;
    }
}

/**
 * Dado un vector de enteros, devuelve la posicion del menor de sus elementos
 * @param v Vector en el que buscar
 * @param tam Tamaño del vector
 * @return Posicion del menor dato
 */
int menor(int *v, int tam) {
    if (tam <= 0) {
        return -1;
    } else if (tam == 1) {
        return v[0];
    } else if (tam > 1) {
        int m = 0;
        for (int i = 1; i < tam; i++) {
            if (v[i] < v[m]) {
                m = i;
            }
        }
        return m;
    }
}

/**
 * Dado un vector de solucion, calcula el coste de esa solucion
 * 
 * @param v Vector solucion
 * @param tam Tamaño del vector
 * @param distancias Matriz de distancias de donde obtener los valores para el coste
 * @param flujos Matriz de flujos de donde obtener los valores para el coste
 * @return coste total de la solucion
 */
int coste(int *v, int tam, int **distancias, int **flujos) {
    int costo = 0;
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            costo += (flujos[i][j])*(distancias[v[i]][v[j]]);
        }
    }
    return costo;
}

/**
 * Desarrollo del algoritmo voraz para encontrar una solucion
 * @param flujos Matriz de flujos
 * @param distancias Matriz de distancias
 * @param solGreedy Vector a rellenar con una solucion
 * @param nCasos Tamaño del problema
 * @return Coste de la solucion
 */
int greedy(int **flujos, int **distancias, int *&solGreedy, int nCasos) {
    int *potFlujos = potencial(flujos, nCasos);
    int *potDistancias = potencial(distancias, nCasos);
    solGreedy = new int[nCasos];
    int cont = nCasos;
    int cost;
    while (cont > 0) {
        int may = mayor(potFlujos, nCasos);
        int men = menor(potDistancias, nCasos);
        if (may != -1 && men != -1) {
            potFlujos[may] = -1;
            potDistancias[men] = 999999;
            solGreedy[may] = men;
            cont--;
        } else {
            break;
        }
    }

    int costo;
    if (cont == 0) {
        costo = coste(solGreedy, nCasos, flujos, distancias);
        return costo;
    } else {
        return -1;
        cost = coste(solGreedy, nCasos, distancias, flujos);
        return cost;

    }
}

/**
 * Calcula una solucion aleatoria
 * @param tam Tamaño del vector solucion
 * @return Vector solucion, generado aleatoriamente
 */
int* solInicial(int tam) {
    int seed = time(0);
    srand(seed);
    int aleatorio;
    int *solucionInicial = new int[tam];
    bool* usado = new bool[tam];
    for (int i = 0; i < tam; i++) {
        usado[i] = false;
    }

    for (int i = 0; i < tam; i++) {
        do {
            aleatorio = rand() % tam;
        } while (usado[aleatorio] == true);

        usado[aleatorio] = true;
        solucionInicial[i] = aleatorio;

    }
    return solucionInicial;
}

/**
 * Devuelve la diferencia de coste existente entre una solucion, 
 *  y la misma con dos posiciones intercambiadas
 * 
 * @param v  vector solucion inicial
 * @param tam Tamaño del vector
 * @param flujos Matriz de flujos
 * @param distancias Matriz de distancias
 * @param r posicion 1 a cambiar
 * @param s posicion 2 a cambiar
 * @return Variacion con respecto al coste original: Positivo si empeora, Negativo si mejora
 */
int factorizacion(int* v, int tam, int **flujos, int** distancias, int r, int s) {

    int fact = 0;
    for (int k = 0; k < tam; k++) {
        if (k != r && k != s) {
            fact += flujos[r][k]*(distancias[v[s]][v[k]] - distancias[v[r]][v[k]]) +
                    flujos[s][k]*(distancias[v[r]][v[k]] - distancias[v[s]][v[k]]) +
                    flujos[k][r]*(distancias[v[k]][v[s]] - distancias[v[k]][v[r]]) +
                    flujos[k][s]*(distancias[v[k]][v[r]] - distancias[v[k]][v[s]]);
        }
    }

    return fact;
}

/**
 * Algoritmo para la busqueda local de la solucion
 * 
 * @param nCasos Tamaño del problema
 * @param flujos Matriz de flujos
 * @param distancias Matriz de distancias
 * @return Vector solucion
 */
int *busquedaLocal(int nCasos, int **flujos, int **distancias) {
    int* solucionActual = solInicial(nCasos);
    int costo = coste(solucionActual, nCasos, distancias, flujos);
    bitset<100> dlb(0);
    bool mejora = false;
    for (int i = nCasos; i < 100; i++) {
        dlb.flip(i);
    }
    while (!dlb.all()) {
        for (int i = 0; i < nCasos; i++) {
            if (!dlb.test(i)) {
                mejora = false;
                for (int j = 0; j < nCasos; j++) {
                    if (!dlb.test(j)) {
                        int variacion = factorizacion(solucionActual, nCasos, flujos, distancias, i, j);
                        if (variacion < 0) {
                            costo += variacion;
                            int tmp = solucionActual[j];
                            solucionActual[j] = solucionActual[i];
                            solucionActual[i] = tmp;
                            dlb.reset(i);
                            dlb.reset(j);
                            mejora = true;
                        }
                    }
                }
                if (!mejora) {
                    dlb.set(i);
                }
            }
        }
    }
    return solucionActual;
}

/**
 * Funcion para comprobar si un vecino ha sido ya generado
 * 
 * @param r Posicion 1 a comprobar
 * @param s Posicion 2 a comprobar
 * @param vecinos Vector de vecinos
 * @param tam Numero de vecinos insertados
 * @return True si se ha encontrado, False si no
 */
bool existeVecino(int r, int s, vector<pair<int, int> > vecinos, int tam) {
    for (int i = 0; i < tam; i++) {
        if ((vecinos[i].first == r && vecinos[i].second == s) || (vecinos[i].first == s && vecinos[i].second == r)) {
            return true;
        }
    }
    return false;
}

int generacionMejorVecino(ListaTabu lista, int &mejorR, int &mejorS, int nCasos, int* solActual, int **flujos, int **distancias, int k) {
    vector<pair<int, int> > vecinos;
    int nVecinos = 0;
    int totalVecinos = 30;
    srand(time(0));
    int r = 1;
    int s = 1;
    pair<int, int> a(-1, -1);
    vecinos.resize(totalVecinos, a);

    while (nVecinos < totalVecinos) {
        r = rand() % nCasos;
        s = rand() % nCasos;

        if (r == s) {
            continue;
        }

        //INTRODUCIR CRITERIO DE ASPIRACION
        bool existe = existeVecino(r, s, vecinos, nVecinos);
        bool tabu = lista.exist(solActual[r], solActual[s], r, s);
        if (tabu && !existe) {
            int variacion = abs(factorizacion(solActual, nCasos, flujos, distancias, r, s));
            int costo = coste(solActual, nCasos, distancias, flujos);
            if (k >= 0 && k < 1500) {
                if (variacion > costo) {
                    vecinos[nVecinos].first = r;
                    vecinos[nVecinos].second = s;
                    nVecinos++;
                }
            } else if (k >= 1500 && k < 3000) {
                if (variacion > costo * 0.9) {
                    vecinos[nVecinos].first = r;
                    vecinos[nVecinos].second = s;
                    nVecinos++;
                }
            } else if (k >= 3000 && k < 4500) {
                if (variacion > costo * 0.7) {
                    vecinos[nVecinos].first = r;
                    vecinos[nVecinos].second = s;
                    nVecinos++;
                }
            } else if (k >= 4500 && k < 6000) {
                if (variacion > costo * 0.6) {
                    vecinos[nVecinos].first = r;
                    vecinos[nVecinos].second = s;
                    nVecinos++;
                }
            } else if (k >= 6000 && k < 7500) {
                if (variacion > costo * 0.3) {
                    vecinos[nVecinos].first = r;
                    vecinos[nVecinos].second = s;
                    nVecinos++;
                }
            } else if (k >= 7500 && k < 9000) {
                if (variacion > costo * 0.15) {
                    vecinos[nVecinos].first = r;
                    vecinos[nVecinos].second = s;
                    nVecinos++;
                }
            } else {
                if (variacion > costo * 0.05) {
                    vecinos[nVecinos].first = r;
                    vecinos[nVecinos].second = s;
                    nVecinos++;
                }
            }
        } else if (!tabu && !existe) {
            vecinos[nVecinos].first = r;
            vecinos[nVecinos].second = s;
            nVecinos++;
        }
    }


    mejorR = vecinos[0].first;
    mejorS = vecinos[0].second;
    int mejorCosto = factorizacion(solActual, nCasos, flujos, distancias, vecinos[0].first, vecinos[0].second);
    int costoActual;

    for (int i = 1; i < totalVecinos; i++) {
        costoActual = factorizacion(solActual, nCasos, flujos, distancias, vecinos[i].first, vecinos[i].second);
        if (costoActual < mejorCosto) {
            mejorCosto = costoActual;
            mejorR = vecinos[i].first;
            mejorS = vecinos[i].second;
        }
    }

    return mejorCosto;
}

int* largoPlazo(int **frec, int nCasos) {
    int *nuevaSolucion = new int[nCasos];
    for (int i = 0; i < nCasos; i++) {
        nuevaSolucion[i] = 0;
    }

    int **aux = new int*[nCasos];
    for (int i = 0; i < nCasos; i++) {
        aux[i] = new int[nCasos];
        for (int j = 0; j < nCasos; j++) {
            aux[i][j] = frec[i][j];
        }
    }

    int asignados = 0;
    int menori = 0;
    int menorj = 0;
    while (asignados <= nCasos) {
        for (int i = 0; i < nCasos; i++) {
            for (int j = i; j < nCasos; j++) {
                if (aux[i][j] < aux[menori][menorj]) {
                    menori = i;
                    menorj = j;
                }
            }
        }
        for (int j = 0; j < nCasos; j++) {
            aux[menori][j] = 99999999;
            aux[j][menorj] = 99999999;
        }
        nuevaSolucion[menori] = menorj;
        asignados++;
    }
    for (int i = 0; i < nCasos; i++) {
        delete[] aux[i];
    }
    delete[] aux;
    return nuevaSolucion;
}

int* busquedaTabu(int nCasos, int **flujos, int **distancias) {

    int* solucionActual = solInicial(nCasos);
    int* mejSolGlobal = new int[nCasos];
    for (int i = 0; i < nCasos; i++) {
        mejSolGlobal[i] = solucionActual[i];
    }

    int **frec = new int*[nCasos];
    for (int i = 0; i < nCasos; i++) {
        frec[i] = new int[nCasos];
        for (int j = 0; j < nCasos; j++)
            frec[i][j] = 0;
    }

    int costoGlobal = coste(mejSolGlobal, nCasos, distancias, flujos);

    ListaTabu lista(nCasos / 2);
    int r, s, variacion;
    int temp;
    int noSeActualiza = 0;
    srand(time(0));
    int prob;
    for (int k = 0; k < 10000; k++) {

        if (noSeActualiza == 10) { //PARAMETROS DE REINICIALIZACION
            prob = rand() % 100;
            if (prob < 24) {
                solucionActual = solInicial(nCasos);
                lista.reset(true);
            } else if (prob < 49) {
                for (int i = 0; i < nCasos; i++) {
                    solucionActual[i] = mejSolGlobal[i];
                }
                lista.reset(false);
            } else {
                solucionActual = largoPlazo(frec, nCasos);
                lista.reset(true);
            }
        }

        variacion = generacionMejorVecino(lista, r, s, nCasos, solucionActual, flujos, distancias, k);
        temp = solucionActual[r];
        solucionActual[r] = solucionActual[s];
        solucionActual[s] = temp;
        if (r > s) { //Actualizar matriz frecuencia
            frec[r][s]++;
        } else {
            frec[s][r]++;
        }
        lista.insert(solucionActual[r], solucionActual[s], r, s);
        if (variacion + costoGlobal < costoGlobal) { //Actualizar mejorSolucion global
            costoGlobal = variacion + costoGlobal;
            for (int i = 0; i < nCasos; i++) {
                mejSolGlobal[i] = solucionActual[i];
            }
            noSeActualiza = 0;
        } else {
            noSeActualiza++;
        }
    }
    return mejSolGlobal;
}