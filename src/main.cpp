#include <iostream>
#include "ordenamientos.h"

int main() {
    crearArchivo("numeros.bin", 100);
    vector<int64_t> datos = crearArreglo("numeros.bin");
    for (int64_t n : datos) {
        cout << n << " ";
    }
    return 0;
}