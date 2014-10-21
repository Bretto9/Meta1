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
    //lista();
    lista.resize(tam);
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
        lista.resize(nTam);
    } else {
        int nTam = tam * 0.5;
        lista.resize(nTam);
    }
}

int ListaTabu::insert(int i, int j, int posI, int posJ) {
    MovimientoTabu mt(i, j, posI, posJ);
    //std::cout<<lista.size()<<"CREA MOV TABU"<<std::endl;
    
    lista.at(indice) = mt;
     //   std::cout<<"CREA MOV TABU"<<std::endl;

    indice = (indice + 1) % lista.size();
    
}

bool ListaTabu::exist(int i, int j, int posI, int posJ) {
    bool existe = false;
    for (int x = 0; x < lista.size(); x++) {
        if ((i == lista.at(x).GetI() && j == lista.at(x).GetJ() && posI == lista.at(x).GetPosI() && posJ == lista.at(x).GetPosJ()) ||
                j == lista.at(x).GetI() && i == lista.at(x).GetJ() && posJ == lista.at(x).GetPosI() && posI == lista.at(x).GetPosI()) {
            existe = true;
        }
        if (existe) {
            break;
        }
    }
    return existe;
}

ListaTabu::~ListaTabu() {
}

