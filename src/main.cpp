#include <iostream>
#include "ordenamientos.h"

int main() {
    cout << "\n\n-----------------------Datos Entrada-----------------------\n\n";
    crearArchivo("numeros.bin", 100);
    vector<int64_t> datos = leerEscribirBloque("numeros.bin", 512);
    for (int64_t n : datos) {
        cout << n << " ";
    }
    cout << "\n\n-----------------------Datos Salida MergeSort-----------------------\n\n";
    mergesortExterno("numeros.bin", "salida.bin", 50 * 1024 * 1024, 512);
    vector<int64_t> datosSalida = leerEscribirBloque("salida.bin", 512);
    for (int64_t n : datosSalida) {
        cout << n << " ";
    }

    return 0;

}