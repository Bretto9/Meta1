/* 
 * File:   ListaTabu.h
 * Author: JoseDavid
 *
 * Created on 20 de octubre de 2014, 17:58
 */
#include <iostream>
#include <vector>
#ifndef LISTATABU_H
#define	LISTATABU_H



class MovimientoTabu {    
private:
    int i;
    int j;
    int posI;
    int posJ;
public:
    MovimientoTabu(): i(-1), j(-1), posI(-1), posJ(-1){};
    MovimientoTabu(int _i, int _j, int _posI, int _posJ): i(_i), j(_j), posI(_posI), posJ(_posJ){}
    MovimientoTabu(const MovimientoTabu &orig){
        i=orig.i;
        j=orig.j;
        posI=orig.posI;
        posJ=orig.posJ;
    }
    void SetPosJ(int posJ) {
        this->posJ = posJ;
    }

    int GetPosJ() const {
        return posJ;
    }

    void SetPosI(int posI) {
        this->posI = posI;
    }

    int GetPosI() const {
        return posI;
    }

    void SetJ(int j) {
        this->j = j;
    }

    int GetJ() const {
        return j;
    }

    void SetI(int i) {
        this->i = i;
    }

    int GetI() const {
        return i;
    };
    
    
};

class ListaTabu {
public:
    ListaTabu(int tam = 0);
    ListaTabu(const ListaTabu& orig);
    virtual ~ListaTabu();
    int insert(int i, int j, int posI, int posJ);
    bool exist(int i, int j, int posI, int posJ);
    void reset(bool aumento);
    
private:
    std::vector<MovimientoTabu> lista;
    int indice;

};

#endif	/* LISTATABU_H */

