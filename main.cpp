#include <iostream>
#include <sstream>

#include "memLibrary/memDirecta.hpp"
#include "memLibrary/memVias.hpp"
#include "memLibrary/memFullAsoc.hpp"
#include <fstream>
  
using namespace std;

int main(int argc, const char* argv[])
{
    ifstream entrada("flights.csv");
    if(!entrada.is_open())
    {
        cout << "No se encuentra el archivo vestiaire.csv";
        return -1;
    }
    string curLine;
    stringstream actualData;
    int direccion;
    int vias, bloques, palabras;
    vias = 10;
    bloques = 16;
    palabras = 32;
    memVias memoria(bloques,palabras,vias);

    if(entrada.good()){
        getline(entrada,curLine);
    }
    while (getline(entrada,curLine))
    {
        for (size_t i = 0; i < 5; i++)
        {
            curLine.erase(0,curLine.find(',') + 1);
        }
        actualData.str(curLine);
        actualData >> direccion;
        memoria.readOne(direccion);
    }
    double aciertos, total;
    aciertos = memoria.getTotalAciertos();
    total = memoria.getTotalDirecciones();
    cout << "Cantidad de bloques: "  << bloques << "\nTotal de palabras: " << palabras << "\nTotal de vias: " << vias;
    cout << "\nTotal de aciertos: " << aciertos << " Total de direcciones: " << total;
    cout << "\nEl porcentaje de aciertos es de: " << float(aciertos/total)*100 << "%";
    return 0;
}