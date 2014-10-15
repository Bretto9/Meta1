/* 
 * File:   main.cpp
 * Author: JoseDavid
 *
 * Created on 7 de octubre de 2014, 17:15
 */

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
            getline(flujo, temp);
            int tempi = 0;
            int tempj = 0;
            while (getline(flujo, temp) && tempi < nCasos) {
                for (int i = 0; i < nCasos; i++) {
                    temp = temp.substr(temp.find_first_not_of(" "));
                    flujos[tempi][tempj] = atoi(temp.substr(0, temp.find(' ')).c_str());
                    tempj++;
                    temp = temp.substr(temp.find(' ') + 1);
                }
                tempi++;
                tempj = 0;
            }
            distancias = new int*[nCasos];
            for (int i = 0; i < nCasos; i++) {
                distancias[i] = new int[nCasos];
            }
            tempi = 0;
            tempj = 0;
            while (getline(flujo, temp) && tempi < nCasos) {
                for (int i = 0; i < nCasos; i++) {
                    temp = temp.substr(temp.find_first_not_of(" "));
                    distancias[tempi][tempj] = atoi(temp.substr(0, temp.find(' ')).c_str());
                    tempj++;
                    temp = temp.substr(temp.find(' ') + 1);
                }
                tempi++;
                tempj = 0;
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

int greedy(int **flujos, int **distancias, int *&solGreedy, int nCasos) {
    int *potFlujos = potencial(flujos, nCasos);
    int *potDistancias = potencial(distancias, nCasos);
    solGreedy = new int[nCasos];
    int cont = nCasos;

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
    cout << "ASIGNACION GREEDY"<< endl;
    if (cont == 0) {
        for (int i = 0; i < nCasos; i++) {
            cout << "Para la unidad " << i + 1 << " localizacion " << solGreedy[i] + 1 << endl;
        }
        return 1;
    } else { 
        return -1;
    }

}

int main(int argc, char** argv) {
    int **flujos, **distancias;
    string fichero = "dat/bur26a.dat";
    int nCasos = lectura(flujos, distancias, fichero);
    int *solGreedy;
    greedy(flujos, distancias, solGreedy, nCasos);

    //        cout << "FLUJOS" << endl;
    //        for (int i = 0; i < nCasos; i++) {
    //            for (int j = 0; j < nCasos; j++) {
    //                cout << flujos[i][j] << " ";
    //            }
    //            cout << endl;
    //        }
    //        cout << endl << "DISTANCIAS" << endl;
    //        for (int i = 0; i < nCasos; i++) {
    //            for (int j = 0; j < nCasos; j++) {
    //                cout << distancias[i][j] << " ";
    //            }
    //            cout << endl;
    //        }
    //    
    //        int *potFlujos = potencial(flujos, nCasos);
    //        cout << endl << "POTENCIAL FLUJOS" << endl;
    //        for(int i = 0; i < nCasos; i++){
    //            cout << potFlujos[i] << " ";
    //        }
    //    
    //        int *potDistancias = potencial(distancias, nCasos);
    //        cout << endl << "POTENCIAL DISTANCIAS" << endl;
    //        for(int i = 0; i < nCasos; i++){
    //            cout << potDistancias[i] << " ";
    //        }
    return 0;
}
