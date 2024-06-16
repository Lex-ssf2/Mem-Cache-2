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
  
using namespace std;

int main(int argc, const char* argv[])
{
    char *addr;
    int fd = open("ancona_data.csv", O_RDONLY);
    int direccion;
    int vias, bloques, palabras;
    vias = 2;
    bloques = 8;
    palabras = 2;
    memVias memoria(bloques,palabras,vias);

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
    char curAddr;
    for (size_t i = 0; i < size; ++i,++addr)
    {
        curAddr = *(addr);
        if(curAddr == ','){
            continue;
        }
        memoria.readOne(curAddr);
    }
    double aciertos, total;
    aciertos = memoria.getTotalAciertos();
    total = memoria.getTotalDirecciones();
    cout << "Cantidad de bloques: "  << bloques << "\nTotal de palabras: " << palabras << "\nTotal de vias: " << vias;
    cout << "\nTotal de aciertos: " << aciertos << " Total de direcciones: " << total;
    cout << "\nEl porcentaje de aciertos es de: " << float(aciertos/total)*100 << "%";
    close(fd);

    /*if(entrada.good()){
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
    }*/

    return 0;
}