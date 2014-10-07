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
            getline(flujo, temp);
            int tempi = 0 ;
            int tempj = 0 ;
//            getline(flujo, temp);
//            cout << temp.substr(0, temp.find(' ')) << " ";
//            temp = temp.substr(temp.find(' ')+1);
//            cout << temp.substr(0, temp.find(' '));
            while(getline(flujo,temp) && tempi < nCasos){
                for(int i = 0; i< nCasos; i++){
                    flujos[tempi][tempj] = atoi(temp.substr(0,temp.find(' ')).c_str());
                    tempj++;
                    temp = temp.substr(temp.find(' ')+1);                    
                }
                tempi++;
                tempj = 0;
            }
            cout << "FLUJOS" << endl;
            for(int i = 0;i < nCasos; i++){
                for(int j = 0;j < nCasos; j++){
                    cout << flujos[i][j] << " ";
                }
                cout << endl;
            }
            
            int **distancias;
            distancias=new int*[nCasos];
            for(int i = 0; i < nCasos; i++){
                distancias[i] = new int[nCasos];
            }
            
//            getline(flujo, temp);
//            cout << temp;
            
            tempi = 0;
            tempj = 0;
            while(getline(flujo,temp) && tempi < nCasos){
                for(int i = 0; i< nCasos; i++){
                    distancias[tempi][tempj] = atoi(temp.substr(0,temp.find(' ')).c_str());
                    tempj++;
                    temp = temp.substr(temp.find(' ')+1);                    
                }
                tempi++;
                tempj = 0;
            }
            
            cout << endl << "DISTANCIAS" << endl;
            for(int i = 0;i < nCasos; i++){
                for(int j = 0;j < nCasos; j++){
                    cout << distancias[i][j] << " ";
                }
                cout << endl;
            }
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
    //cout << nCasos;
    
    
    

    return 0;
}
