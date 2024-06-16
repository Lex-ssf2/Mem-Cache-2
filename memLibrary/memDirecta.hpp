#ifndef MEM_DIRECTA_H
#define MEM_DIRECTA_H

#include "memoria.hpp"
#include <vector>

using namespace std;

class memDirecta : public Memoria
{
private:
    vector<BloqueCache> listaCache;
public:
    memDirecta(int tam, int palabras);
    ~memDirecta();

    void start();
    bool acierto(int index, int etiqueta, int palabra);
};

memDirecta::memDirecta(int tam, int palabras)
{
    this->type = "Correspondencia Directa:";
    init(tam,palabras);
    listaCache.resize(cantPalabras * tamanoBloque, BloqueCache());
    start();
}

memDirecta::~memDirecta()
{
    listaCache.clear();
}

void memDirecta::start(){
    
    int entrada;
    while(cin >> entrada){

        int etiqueta,indice,palabra;
        spliceData(entrada,etiqueta,indice,palabra);
        updateTable(entrada,etiqueta,indice,palabra);
        curAcierto = acierto(indice,etiqueta,palabra);
    };

}

bool memDirecta::acierto(int index, int etiqueta, int palabra){

    if(listaCache[index].getEtiqueta() != etiqueta)
    {
        listaCache[index].setEtiqueta(etiqueta);
        listaCache[index].setPalabra(palabra);
        listaCache[index].setAcierto(false);
        HM += "M, ";
        return false;
    }
    listaCache[index].setAcierto(true);
    listaCache[index].setPalabra(palabra);
     HM += "H, ";
     return true;
    
}

#endif