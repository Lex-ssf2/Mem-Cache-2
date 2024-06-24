#ifndef MEM_FULL_ASOC_H
#define MEM_FULL_ASOC_H

#include "memoria.hpp"
#include <vector>

using namespace std;

class memFullAsoc : public Memoria
{
private:
    vector<BloqueCache> listaCache;
public:
    memFullAsoc(int tam);
    ~memFullAsoc();

    void start();
    bool acierto(int index, bool prefetch = false);
    void updateTable(int entrada);
    void readOne(int entrada);
    void prefetch(int direccion);
};

memFullAsoc::memFullAsoc(int tam)
{
    this->type = "Completamente Asociativa:";
    init(tam);
    this->offsetBit = 0;
    this->offsetPalabra = 0;
    listaCache.resize(tamanoBloque, BloqueCache());
}

memFullAsoc::~memFullAsoc()
{
    listaCache.clear();
}

void memFullAsoc::start(){
    
    int entrada;
    while(cin >> entrada){

        if(listaCache.size() < convertBinary(entrada).size())
        {
            listaCache.resize(convertBinary(entrada).size() + 1);
        }
        updateTable(entrada);
        curAcierto = acierto(entrada);
    };
}

bool memFullAsoc::acierto(int index,bool prefetch){

    if(listaCache[index].getEtiqueta() != index)
    {
        listaCache[index].setEtiqueta(index);
        listaCache[index].setAcierto(false);
        HM += "M, ";
        return false;
    }
    listaCache[index].setAcierto(true);
    if(!prefetch){
     HM += "H, ";
     ++totalAcierto;
    }
    return true;
    
}

void memFullAsoc::readOne(int entrada){
    int etiqueta,indice,palabra;
    ++totalDirecciones;
    //spliceData(entrada,etiqueta,indice,palabra);
    //updateTable(entrada,etiqueta,indice,palabra);
    curAcierto = acierto(entrada);
}

void memFullAsoc::prefetch(int direccion)
{
    int etiqueta,indice,palabra;
    //spliceData(direccion,etiqueta,indice,palabra);
    acierto(direccion,false);
}

void memFullAsoc::updateTable(int entrada){

    string enBin = convertBinary(entrada);
    this->BinAdrss.resize(this->BinAdrss.size() + enBin.size());
    this->Etiqueta.resize(this->Etiqueta.size() + enBin.size());
    this->BinAdrss += enBin + ", ";
    this->Etiqueta += enBin + ", ";
}

#endif