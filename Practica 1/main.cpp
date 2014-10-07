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

int main(int argc, char** argv) {
    
    int nCasos = 0;
    string temp;
    ifstream flujo("dat/bur26a.dat");

    if (flujo.is_open()) {


        if (flujo.good()) {

            getline(flujo, temp);

            nCasos = atoi(temp.c_str());
            
            int **flujos;
            flujos=new int*[nCasos];
            for(int i = 0; i < nCasos; i++){
                flujos[i] = new int[nCasos];
            }
    
        int **distancias;

        }
        //     char c = flujo.get();
        //    while (flujo.good()) {
        //        
        //     // cout << c;
        //      c = flujo.get();
        //    }
    } else {
        cout << "Error opening file";
    }
    cout << nCasos;
    
    
    

    return 0;
}
