/* 
 * File:   ListaTabu.cpp
 * Author: JoseDavid
 * 
 * Created on 20 de octubre de 2014, 18:12
 */

#include "ListaTabu.h"
#include <iostream>
using namespace std;

ListaTabu::ListaTabu(int tam) {
    lista.resize(tam);
    maxTam = tam * 2.5;
    minTam = tam * 0.5;
    indice = 0;
}

ListaTabu::ListaTabu(const ListaTabu& orig) {
    //lista();
    lista.resize(orig.lista.size());
    indice = orig.indice;
}

void ListaTabu::reset(bool aumento) {
    int tam = lista.size();
    lista.clear();
    indice = 0;
    if (aumento) {
        int nTam = tam * 1.5;
        if (nTam < maxTam) {
            lista.resize(nTam);
        } else {
            lista.resize(tam);
        }
    } else {
        int nTam = tam * 0.5;
        if(nTam > minTam && nTam > 0){
            lista.resize(nTam);
        } else {
            lista.resize(tam);
        }
    }
}

int ListaTabu::insert(int i, int j, int posI, int posJ) {
    MovimientoTabu mt(i, j, posI, posJ);
    lista[indice] = mt;
    int tam = lista.size();
    indice = (indice + 1) % lista.size();
}

bool ListaTabu::exist(int i, int j, int posI, int posJ) {
    for (int x = 0; x < lista.size(); x++) {
        if ((i == lista.at(x).GetI() && j == lista.at(x).GetJ() && posI == lista.at(x).GetPosI() && posJ == lista.at(x).GetPosJ()) ||
                j == lista.at(x).GetI() && i == lista.at(x).GetJ() && posJ == lista.at(x).GetPosI() && posI == lista.at(x).GetPosI()) {
            return true;
        }
    }
    return false;
}

ListaTabu::~ListaTabu() {
}

void ListaTabu::clear() {
    int tam = lista.size();
    lista.clear();
    lista.resize(tam);
}
