#include <iostream>
#include <sstream>

#include "memLibrary/memDirecta.hpp"
#include "memLibrary/memVias.hpp"
#include "memLibrary/memFullAsoc.hpp"
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <chrono>
#include <ctime>
  
using namespace std;

void porVias(size_t size,char *addr,double aciertos,double total);
void directa(size_t size,char *addr,double aciertos,double total);
void fullAsoc(size_t size,char *addr,double aciertos,double total);

int main(int argc, const char* argv[])
{
    char *addr;
    int fd = open("ancona_data.csv", O_RDONLY);
    int direccion;
    char manualAddress;
    double aciertos, total;
    bool fetchBoolean = false;

    if(fd == -1){
        cout << "Archivo no encontrado";
        return -1;
    }
    struct stat fileInfo = {0};
    if (fstat(fd, &fileInfo) == -1)
    {
        close(fd);
        cout << "Error obteniendo el tamaño";
        return -1;
    }
    addr = (char*)mmap(NULL,fileInfo.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        close(fd);
        cout << "Error mmapping the file";
        return -1;
    }
    size_t size = fileInfo.st_size;
    string type = "-1";
    system("clear");
    while(!isdigit(type[0]) || stoi(type) > 3 || stoi(type) < 1 ){
        cout << "Ingrese el tipo de configuracion que desea\n 1. Directa\n 2. Por Conjuntos\n 3. Completamente Asociativa\n";
        cin >> type;
        if(!isdigit(type[0]) || stoi(type) > 3 || stoi(type) < 1 ){
            system("clear");
            cout << "\nValor invalido, intente de nuevo\n\n";
        }
    }
    switch (stoi(type))
    {
    case 2:
        porVias(size,addr,aciertos,total);
        break;
    case 3:
        fullAsoc(size,addr,aciertos,total);
        break;
    default:
        directa(size,addr,aciertos,total);
        break;
    }
    close(fd);
    return 0;
}

void porVias(size_t size,char *addr,double aciertos,double total)
{
    system("clear");
    char curAddr;
    int vias, bloques, palabras;
    vias = 2;
    bloques = 4;
    palabras = 1;
    string config = "-1";
    while(!isdigit(config[0]) || stoi(config) < 0){
        cout << "Inserta el numero de bloques, colocar 0 para ir a la configuracion predeterminada\n";
        cin >> config;
        if(!isdigit(config[0]) || stoi(config) < 0){
            system("clear");
            cout << "\nValor invalido, intente de nuevo\n\n";
        }
    }
    if(stoi(config) != 0){
        system("clear");
        bloques = stoi(config);
        config = "-1";
        while(!isdigit(config[0]) || stoi(config) < 0){
            cout << "Inserta el numero de palabras\n";
            cin >> config;
            if(!isdigit(config[0]) || stoi(config) < 0){
                system("clear");
                cout << "\nValor invalido, intente de nuevo\n\n";
            }
        }
        system("clear");
        palabras = stoi(config);
        config = "-1";
        while(!isdigit(config[0]) || stoi(config) < 2){
            cout << "Inserta el numero de vias\n";
            cin >> config;
            if(!isdigit(config[0]) || stoi(config) < 2){
                system("clear");
                cout << "\nValor invalido, intente de nuevo\n\n";
            }
        }
        vias = stoi(config);
    }

    memVias memoria(bloques,palabras,vias), memoriaNM(bloques,palabras,vias);
    for (size_t i = 0; i < size; ++i,++addr)
    {
        curAddr = *(addr);
        if(curAddr == ','){
            continue;
        }
        memoria.readOne((int)curAddr);
        if(!memoria.getCurAcierto())
        {
            if(i + 1 < size)
                memoria.prefetch((int)(*(addr) + 1));
        }
    }

    aciertos = memoria.getTotalAciertos();
    total = memoria.getTotalDirecciones();
    cout << "Con MMAP:\nCantidad de bloques: "  << bloques << "\nTotal de palabras: " << palabras << "\nTotal de vias: " << vias;
    cout << "\nTotal de aciertos: " << (int)aciertos << " Total de direcciones: " << (int)total;
    cout << "\nEl porcentaje de aciertos es de: " << float(aciertos/total)*100 << "%\n\n";
    ifstream entrada("ancona_data.csv");
    if(!entrada.is_open())
    {
        cout << "No se encuentra el archivo ancona_data.csv";
        return -1;
    };
    if(entrada.good() && entrada.is_open()){
        while(!entrada.eof()){
            if(!fetchBoolean){
                entrada.get(manualAddress);
            }
            fetchBoolean = false;
            if(manualAddress != ','){
                memoriaNM.readOne((int)manualAddress);
                if(!memoriaNM.getCurAcierto())
                {
                    entrada.get(manualAddress);
                    while(manualAddress == ','){
                        entrada.get(manualAddress);
                    }
                    memoriaNM.prefetch((int)(manualAddress));
                    fetchBoolean = true;
                }
            }
        }
    }
    entrada.close();
    cout << "Con entrada estandar:\nCantidad de bloques: "  << bloques << "\nTotal de palabras: " << palabras << "\nTotal de vias: " << vias;
    cout << "\nTotal de aciertos: " << aciertos << " Total de direcciones: " << total;
    cout << "\nEl porcentaje de aciertos es de: " << float(aciertos/total)*100 << "%\n";
}

void directa(size_t size,char *addr,double aciertos,double total)
{
    system("clear");
    char curAddr;
    int vias, bloques, palabras;
    vias = 0;
    bloques = 4;
    palabras = 1;
    string config = "-1";
    while(!isdigit(config[0]) || stoi(config) < 0){
        cout << "Inserta el numero de bloques, colocar 0 para ir a la configuracion predeterminada\n";
        cin >> config;
        if(!isdigit(config[0]) || stoi(config) < 0){
            system("clear");
            cout << "\nValor invalido, intente de nuevo\n\n";
        }
    }
    if(stoi(config) != 0){
        system("clear");
        bloques = stoi(config);
        config = "-1";
        while(!isdigit(config[0]) || stoi(config) < 0){
            cout << "Inserta el numero de palabras\n";
            cin >> config;
            if(!isdigit(config[0]) || stoi(config) < 0){
                system("clear");
                cout << "\nValor invalido, intente de nuevo\n\n";
            }
        }
        system("clear");
        palabras = stoi(config);
        config = "-1";
    }

    memDirecta memoria(bloques,palabras), memoriaNM(bloques,palabras);
    
    for (size_t i = 0; i < size; ++i,++addr)
    {
        curAddr = *(addr);
        if(curAddr == ','){
            continue;
        }
        memoria.readOne((int)curAddr);
        if(!memoria.getCurAcierto())
        {
            if(i + 1 < size)
                memoria.prefetch((int)(*(addr) + 1));
        }
    }
    aciertos = memoria.getTotalAciertos();
    total = memoria.getTotalDirecciones();
    cout << "Con MMAP:\nCantidad de bloques: "  << bloques << "\nTotal de palabras: " << palabras << "\nTotal de vias: " << vias;
    cout << "\nTotal de aciertos: " << (int)aciertos << " Total de direcciones: " << (int)total;
    cout << "\nEl porcentaje de aciertos es de: " << float(aciertos/total)*100 << "%\n\n";
    ifstream entrada("ancona_data.csv");
    if(!entrada.is_open())
    {
        cout << "No se encuentra el archivo ancona_data.csv";
        return -1;
    };
    if(entrada.good() && entrada.is_open()){
        while(!entrada.eof()){
            if(!fetchBoolean){
                entrada.get(manualAddress);
            }
            fetchBoolean = false;
            if(manualAddress != ','){
                memoriaNM.readOne((int)manualAddress);
                if(!memoriaNM.getCurAcierto())
                {
                    entrada.get(manualAddress);
                    while(manualAddress == ','){
                        entrada.get(manualAddress);
                    }
                    memoriaNM.prefetch((int)(manualAddress));
                    fetchBoolean = true;
                }
            }
        }
    }
    entrada.close();
    cout << "Con entrada estandar:\nCantidad de bloques: "  << bloques << "\nTotal de palabras: " << palabras << "\nTotal de vias: " << vias;
    cout << "\nTotal de aciertos: " << aciertos << " Total de direcciones: " << total;
    cout << "\nEl porcentaje de aciertos es de: " << float(aciertos/total)*100 << "%\n";
}

void fullAsoc(size_t size,char *addr,double aciertos,double total)
{
    system("clear");
    char curAddr;
    int vias, bloques, palabras;
    bloques = 32;

    memFullAsoc memoria(bloques), memoriaNM(bloques);
    for (size_t i = 0; i < size; ++i,++addr)
    {
        curAddr = *(addr);
        if(curAddr == ','){
            continue;
        }
        memoria.readOne((int)curAddr);
        if(!memoria.getCurAcierto())
        {
            if(i + 1 < size)
                memoria.prefetch((int)(*(addr) + 1));
        }
    }

    aciertos = memoria.getTotalAciertos();
    total = memoria.getTotalDirecciones();
    cout << "Con MMAP:\nCantidad de bloques: "  << bloques << "\nTotal de palabras: " << palabras << "\nTotal de vias: " << vias;
    cout << "\nTotal de aciertos: " << (int)aciertos << " Total de direcciones: " << (int)total;
    cout << "\nEl porcentaje de aciertos es de: " << float(aciertos/total)*100 << "%\n\n";
    ifstream entrada("ancona_data.csv");
    if(!entrada.is_open())
    {
        cout << "No se encuentra el archivo ancona_data.csv";
        return -1;
    };
    if(entrada.good() && entrada.is_open()){
        while(!entrada.eof()){
            if(!fetchBoolean){
                entrada.get(manualAddress);
            }
            fetchBoolean = false;
            if(manualAddress != ','){
                memoriaNM.readOne((int)manualAddress);
                if(!memoriaNM.getCurAcierto())
                {
                    entrada.get(manualAddress);
                    while(manualAddress == ','){
                        entrada.get(manualAddress);
                    }
                    memoriaNM.prefetch((int)(manualAddress));
                    fetchBoolean = true;
                }
            }
        }
    }
    entrada.close();
    cout << "Con entrada estandar:\nCantidad de bloques: "  << bloques << "\nTotal de palabras: " << palabras << "\nTotal de vias: " << vias;
    cout << "\nTotal de aciertos: " << aciertos << " Total de direcciones: " << total;
    cout << "\nEl porcentaje de aciertos es de: " << float(aciertos/total)*100 << "%\n";
}