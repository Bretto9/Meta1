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

int coste(int *v, int tam, int **distancias, int **flujos) {
    int costo = 0;
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            costo += (flujos[i][j])*(distancias[v[i]][v[j]]);
        }
    }
    return costo;
}

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

int* solInicial(int tam) {
    srand(time(0));
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
 * 
 * @param v  vector solucion inicial
 * @param tam nCasos
 * @param flujos 
 * @param distancias
 * @param r  posicion 1 a cambiar
 * @param s posicion 2 a cambiar
 * @return 
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

int *busquedaLocal(int nCasos, int **flujos, int **distancias) {
    int* solucionActual = solInicial(nCasos);
    //int* solucionCandidata = new int[nCasos];
    int costo = coste(solucionActual, nCasos, distancias, flujos);
    //    for (int i = 0; i < nCasos; i++) {
    //        solucionCandidata[i] = solucionActual[i];
    //    }
    bitset<100> dlb(0);
    bool mejora = false;
    for (int i = nCasos; i < 100; i++) {
        dlb.flip(i);
    }
    bool mostrado = false;
    while (!dlb.all()) {
        for (int i = 0; i < nCasos; i++) {
            if (!dlb.test(i)) {
                mejora = false;
                for (int j = 0; j < nCasos; j++) {
                    if (!dlb.test(j)) {
                        //                        solucionCandidata[j] = solucionActual[i];
                        //                        solucionCandidata[i] = solucionActual[j];

                        int variacion = factorizacion(solucionActual, nCasos, flujos, distancias, i, j);

                        if (variacion < 0) {
                            //if (!mostrado) {
                            //                                mostrado = true;
                            //                                int nCoste = coste(solucionCandidata, nCasos, distancias, flujos);
                            //                                int oCoste = coste(solucionActual, nCasos, distancias, flujos);
                            //                                cout << " ------------------------- " <<endl;
                            //                                cout << "Nuevo coste: " << nCoste << endl;
                            //                                cout << "Antiguo coste: " << oCoste << endl;
                            //                                cout << "Nuevo coste + variacion: " << nCoste + variacion << endl;
                            //                                cout << " ------------------------- " <<endl;
                            //}
                            //cout << "Cambio " << i << " por " << j << endl;
                            costo += variacion;
                            int tmp = solucionActual[j];
                            solucionActual[j] = solucionActual[i];
                            solucionActual[i] = tmp;
                            //                            tmp = solucionActual[j];
                            //                            solucionActual[j] = solucionActual[i];
                            //                            solucionActual[i] = tmp;
                            dlb.reset(i);
                            dlb.reset(j);
                            mejora = true;
                        }
                        //                        else {
                        //                            solucionCandidata[i] = solucionActual[i];
                        //                            solucionCandidata[j] = solucionActual[j];
                        //                        }
                    }
                }
                if (!mejora) {
                    dlb.set(i);
                }
            }
        }
        //cout << dlb << endl;
    }
    //    delete[] solucionCandidata;
    //    cout << "Coste sin variacion: " << coste(solucionActual, nCasos, distancias, flujos) << endl;
    //    cout << "Coste con variacion: " << costo << endl;
    return solucionActual;
}

bool existeVecino(int r, int s, vector<pair<int, int> > vecinos, int tam) {

    for (int i = 0; i < tam; i++) {

        if ((vecinos[i].first == r && vecinos[i].second == s) || (vecinos[i].first == s && vecinos[i].second == r)) {
            return true;
        }
    }

    return false;
}

int generacionMejorVecino(vector<int> listaTabu, int &mejorR, int &mejorS, int nCasos, int* solActual, int **flujos, int **distancias) {

    vector<pair<int, int> > vecinos;
    int nVecinos = 0;
    srand(time(0));
    int r;
    int s;

    while (nVecinos < 30) {
        r = rand() % nCasos;
        s = rand() % nCasos;

        if (!listaTabu.exist(solActual[r], solActual[s], r, s,) && !existeVecino(r, s, vecinos, nVecinos)) {

            vecinos[nVecinos].first = r;
            vecinos[nVecinos].second = s;
            nVecinos++;
        }
    }
    mejorR = vecinos[0].first;
    mejorS = vecinos[0].second;
    int mejorCosto = factorizacion(solActual, nCasos, flujos, distancias, vecinos[0].first, vecinos[0].second);
    int costoActual;

    for (int i = 0; i < nCasos; i++) {
        costoActual = factorizacion(solActual, nCasos, flujos, distancias, vecinos[i].first, vecinos[i].second);
        if (costoActual < mejorCosto) {
            mejorCosto = costoActual;
            mejorR = vecinos[i].first;
            mejorS = vecinos[i].second;
        }
    }
    
    return mejorCosto;
}

int* busquedaTabu(int nCasos, int **flujos, int **distancias) {

    int* solucionActual = solInicial(nCasos);
    int* mejSolGlobal = solucionActual;
    int costoGlobal = coste(mejSolGlobal, nCasos, distancias, flujos);



}

int main(int argc, char** argv) {

    int **flujos, **distancias;
    string fichero = "dat/lipa90a.dat";
    string ficheros[20] = {"dat/els19.dat", "dat/chr20a.dat", "dat/chr25a.dat", "dat/nug25.dat",
        "dat/bur26a.dat", "dat/bur26b.dat", "dat/tai30a.dat", "dat/tai30b.dat",
        "dat/esc32a.dat", "dat/kra32.dat", "dat/tai35a.dat", "dat/tai35b.dat",
        "dat/tho40.dat", "dat/tai40a.dat", "dat/sko42.dat", "dat/sko49.dat",
        "dat/tai50a.dat", "dat/tai50b.dat", "dat/tai60a.dat", "dat/lipa90a.dat"};

    // GREEDY MUCHOS FICHEROS
    //    for (int i = 0; i < 20; i++) {
    //        cout << "Leyendo fichero... " << ficheros[i] << endl;
    //        int nCasos = lectura(flujos, distancias, ficheros[i]);
    //
    //        int *solGreedy;
    //        int costo = greedy(flujos, distancias, solGreedy, nCasos);
    //        cout << "Coste del algoritmo voraz para el fichero( " << i + 1 << " ) " << ficheros[i] << " es:" << costo << endl;
    //        for (int j = 0; j < nCasos; j++) {
    //            delete[] flujos[j];
    //            delete[] distancias[j];
    //        }
    //        delete flujos;
    //        delete distancias;
    //        delete solGreedy;
    //    }

    for (int i = 0; i < 20; i++) {
        fichero = ficheros[i];
        //GREEDY UN FICERO
        cout << "Leyendo fichero... " << fichero << endl;
        int nCasos = lectura(flujos, distancias, fichero);

        int *solGreedy;
        int costo = greedy(flujos, distancias, solGreedy, nCasos);
        cout << "Coste del algoritmo voraz para el fichero( " << i + 1 << " ) " << fichero << " es:" << costo << endl;


        int *solLocal = busquedaLocal(nCasos, flujos, distancias);

        costo = coste(solLocal, nCasos, distancias, flujos);
        //    for (int i = 0; i < nCasos; i++) {
        //        cout << "Para la unidad " << i + 1 << " asignada localizacion " << solLocal[i] + 1 << endl;
        //    }
        cout << "Coste del algoritmo LOCAL para el fichero( " << i + 1 << " ) " << fichero << " es:" << costo << endl;

        //                cout << endl << "SOLUCION INICIAL" << endl;
        //            for(int i = 0; i < nCasos; i++){
        //                cout << solucionInicial[i] << " ";

        for (int j = 0; j < nCasos; j++) {
            delete[] flujos[j];
            delete[] distancias[j];
        }
        delete flujos;
        delete distancias;
        delete solGreedy;
    }
    return 0;
}
