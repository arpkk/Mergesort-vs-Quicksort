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

void crearArchivo(string nombre_archivo, int64_t N){
    int64_t tamano = N * sizeof(int64_t);
    ofstream archivo(nombre_archivo, ios::binary);
    if (!archivo.is_open()) {
        cerr << "Error al crear el archivo binario\n";
        return;
    }

    mt19937_64 rng(random_device{}()); // generador de números aleatorios
    uniform_int_distribution<int64_t> dist(INT64_MIN, INT64_MAX);

    for (int64_t i = 0; i < N; ++i) {
        int64_t numero = dist(rng);
        archivo.write(reinterpret_cast<const char*>(&numero), sizeof(int64_t));
    }

    archivo.close();
    cout << "Archivo '" << nombre_archivo << "' creado con " << tamano << " B\n";
}


vector<int64_t> leerArchivo(string nombre_archivo, size_t B){

    ifstream archivo(nombre_archivo, ios::binary);

    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo.\n";
        return {};
    }

    vector<int64_t> arreglo;
    int cantidad_por_bloque = B / sizeof(int64_t); //sizeof(int64_t) = 8
    vector<int64_t> buffer(cantidad_por_bloque);

    while (archivo.read(reinterpret_cast<char*>(buffer.data()), B)) {
        arreglo.insert(arreglo.end(), buffer.begin(), buffer.end());
    }

    // Manejar último bloque parcial
    int leidos = archivo.gcount() / sizeof(int64_t);
    arreglo.insert(arreglo.end(), buffer.begin(), buffer.begin() + leidos);

    archivo.close();
    return arreglo;
}
