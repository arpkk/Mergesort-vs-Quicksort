#include <iostream>
#include <fstream>
#include <random>
#include <cstdint>
#include <string>
#include <vector>
using namespace std;
#include "ordenamientos.h"

/*
Este script genera un archivo con numeros binarios desordenados
*/

void crearArchivo(string nombre_archivo, int64_t cantidad_numeros){
    ofstream archivo(nombre_archivo, ios::binary);
    if (!archivo.is_open()) {
        cerr << "Error al crear el archivo binario\n";
        return;
    }

    mt19937_64 rng(random_device{}()); // generador de números aleatorios
    uniform_int_distribution<int64_t> dist(INT64_MIN, INT64_MAX);

    for (int64_t i = 0; i < cantidad_numeros; ++i) {
        int64_t numero = dist(rng);
        archivo.write(reinterpret_cast<const char*>(&numero), sizeof(int64_t));
    }

    archivo.close();
    cout << "Archivo '" << nombre_archivo << "' creado con " << cantidad_numeros << " enteros de 64 bits.\n";
}


vector<int64_t> crearArreglo(string nombre_archivo){
    ifstream archivo(nombre_archivo, ios::binary);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo.\n";
        return {};
    }

    vector<int64_t> arreglo;
    int64_t numero;

    // leer mientras queden bloques de 8 bytes
    while (archivo.read(reinterpret_cast<char*>(&numero), sizeof(int64_t))) {
        arreglo.push_back(numero);
    }

    archivo.close();
    return arreglo;
}
