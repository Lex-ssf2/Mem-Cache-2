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
    char manualAddress;
    double aciertos, total;
    bool fetchBoolean = false;
    vias = 2;
    bloques = 4;
    palabras = 2;
    memVias memoria(bloques,palabras,vias), memoriaNM(bloques,palabras,vias);
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
        memoria.readOne((int)curAddr);
        if(!memoria.getCurAcierto())
        {
            memoria.prefetch((int)(curAddr + 1));
        }
    }
    aciertos = memoria.getTotalAciertos();
    total = memoria.getTotalDirecciones();
    cout << "Con MMAP:\nCantidad de bloques: "  << bloques << "\nTotal de palabras: " << palabras << "\nTotal de vias: " << vias;
    cout << "\nTotal de aciertos: " << aciertos << " Total de direcciones: " << total;
    cout << "\nEl porcentaje de aciertos es de: " << float(aciertos/total)*100 << "%\n\n";
    close(fd);
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
                    memoriaNM.prefetch((int)(manualAddress));
                    if(!fetchBoolean)
                        fetchBoolean = true;
                }
            }
        }
    }
    entrada.close();
    aciertos = memoriaNM.getTotalAciertos();
    total = memoriaNM.getTotalDirecciones();
    cout << "Con entrada estandar:\nCantidad de bloques: "  << bloques << "\nTotal de palabras: " << palabras << "\nTotal de vias: " << vias;
    cout << "\nTotal de aciertos: " << aciertos << " Total de direcciones: " << total;
    cout << "\nEl porcentaje de aciertos es de: " << float(aciertos/total)*100 << "%\n";

    return 0;
}