#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdlib.h>
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
            int tempi = 0;
            int tempj = 0;
            while (getline(flujo, temp) && tempi < nCasos) {
                if (temp.length() > 0) {
                    for (int i = 0; i < nCasos; i++) {
                        temp = temp.substr(temp.find_first_not_of(" "));
                        flujos[tempi][tempj] = atoi(temp.substr(0, temp.find(' ')).c_str());
                        tempj++;
                        temp = temp.substr(temp.find(' ') + 1);
                    }
                    tempi++;
                    tempj = 0;
                }
            }
            distancias = new int*[nCasos];
            for (int i = 0; i < nCasos; i++) {
                distancias[i] = new int[nCasos];
            }
            tempi = 0;
            tempj = 0;
            while (getline(flujo, temp) && tempi < nCasos) {
                if (temp.length() > 0) {
                    for (int i = 0; i < nCasos; i++) {
                        temp = temp.substr(temp.find_first_not_of(" "));
                        distancias[tempi][tempj] = atoi(temp.substr(0, temp.find(' ')).c_str());
                        tempj++;
                        temp = temp.substr(temp.find(' ') + 1);
                    }
                    tempi++;
                    tempj = 0;
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
        costo = coste(solGreedy,nCasos, flujos, distancias);
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

int main(int argc, char** argv) {

    int **flujos, **distancias;
    string fichero = "dat/els19.dat";
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

    
    //GREEDY UN FICERO
        cout << "Leyendo fichero... " << fichero << endl;
        int nCasos = lectura(flujos, distancias, fichero);
    
        int *solGreedy;
        int costo = greedy(flujos, distancias, solGreedy, nCasos);
        cout << "Coste del algoritmo voraz para el fichero( " << 1 << " ) " << fichero << " es:" << costo << endl;
        for (int j = 0; j < nCasos; j++) {
            delete[] flujos[j];
            delete[] distancias[j];
        }
        delete flujos;
        delete distancias;
        delete solGreedy;


    int* solucionInicial = new int[nCasos];
    solucionInicial = solInicial(nCasos);


    //                cout << endl << "SOLUCION INICIAL" << endl;
    //            for(int i = 0; i < nCasos; i++){
    //                cout << solucionInicial[i] << " ";
    //            }

 

    return 0;
}
