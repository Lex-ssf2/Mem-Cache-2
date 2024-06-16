#include <iostream>
#include <sstream>

#include "memLibrary/memDirecta.hpp"
#include "memLibrary/memVias.hpp"
#include "memLibrary/memFullAsoc.hpp"
#include <fstream>
  
using namespace std;

int main(int argc, const char* argv[])
{
    ifstream entrada("vestiaire.csv");
    if(!entrada.is_open())
    {
        cout << "No se encuentra el archivo vestiaire.csv";
        return -1;
    }
    string curLine;
    stringstream actualData;
    int direccion;
    memVias memoria(2,2,2);

    if(entrada.good()){
        getline(entrada,curLine);
    }
    while (getline(entrada,curLine))
    {
        actualData.str(curLine);
        actualData >> direccion;
        memoria.readOne(direccion);
    }
    
    return 0;
}