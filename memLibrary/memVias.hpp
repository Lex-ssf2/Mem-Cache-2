#ifndef MEM_CONJUNTOS_H
#define MEM_CONJUNTOS_H

#include "memoria.hpp"
#include <vector>
#include <iostream>

using namespace std;

class memVias : public Memoria
{
private:
    vector<vector<BloqueCache>> listaCacheVias;
public:
    memVias(int tam, int palabras,int vias = 1);
    ~memVias();

    int getLRUindex(int index);
    void start();
    bool acierto(int index, int etiqueta, int palabra, bool prefetch = false);
    void readOne(int entrada);
    void prefetch(int direccion);
};

memVias::memVias(int tam, int palabras,int vias)
{
    this->type = "Correspondencia por vias:";
    init(tam,palabras,vias);
    listaCacheVias.resize(cantPalabras * tamanoBloque, vector<BloqueCache>(viasMax + 1));
}

memVias::~memVias()
{
    listaCacheVias.clear();
}

void memVias::start(){
    
    int entrada;
    while(cin >> entrada){

        int etiqueta,indice,palabra;
        spliceData(entrada,etiqueta,indice,palabra);
        updateTable(entrada,etiqueta,indice,palabra);
        curAcierto = acierto(indice,etiqueta,palabra);
    };
}

void memVias::readOne(int entrada){
    int etiqueta,indice,palabra;
    ++totalDirecciones;
    spliceData(entrada,etiqueta,indice,palabra);
    //updateTable(entrada,etiqueta,indice,palabra);
    curAcierto = acierto(indice,etiqueta,palabra);
}

void memVias::prefetch(int direccion)
{
    int etiqueta,indice,palabra;
    spliceData(direccion,etiqueta,indice,palabra);
    acierto(indice,etiqueta,palabra,false);
}

bool memVias::acierto(int index, int etiqueta, int palabra,bool prefetch){

    bool encontrado = false;
    int contador = listaCacheVias[index][this->viasMax].getCont() + 1;
    listaCacheVias[index][this->viasMax].setCont(contador);

    //Busqueda en las vias
    for (size_t i = 0; i < this->viasMax; i++)
    {
        if(listaCacheVias[index][i].getEtiqueta() == etiqueta){
            listaCacheVias[index][i].setCont(contador);
            listaCacheVias[index][i].setAcierto(true);
            if(!prefetch){
                HM += "H, ";
                ++totalAcierto;
            }
            return true;
        }
    }

    int tmpIndex = getLRUindex(index);
    listaCacheVias[index][tmpIndex].setEtiqueta(etiqueta);
    listaCacheVias[index][tmpIndex].setAcierto(false);
    listaCacheVias[index][tmpIndex].setPalabra(palabra);
    listaCacheVias[index][tmpIndex].setCont(contador);
    HM += "M, ";
    return false;
    
}

int memVias::getLRUindex(int index){

    int tmpCont = listaCacheVias[index][0].getCont();
    int tmpIndex = 0;
    for (size_t i = 0; i < this->viasMax; i++)
    {
        if(listaCacheVias[index][i].getCont() < tmpCont){
            tmpIndex = i;
            tmpCont = listaCacheVias[index][i].getCont();
        }
    }
    return tmpIndex;
}

#endif