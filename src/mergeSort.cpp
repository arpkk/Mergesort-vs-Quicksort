#include <iostream>
#include <fstream>
#include <random>
#include <cstdint>
#include <string>
#include <vector>
using namespace std;

/*
GenerarRuns: Se generan archivos temporales en memoria principal que contienen una parte del arreglo original
Cuando el tamaño es menor o igual a M
*/
void generarRuns(string archivo_entrada, size_t M, size_t B, vector<string>& archivos_runs) {
    ifstream entrada(archivo_entrada, ios::binary);
    if (!entrada.is_open()) {
        cerr << "Error al abrir archivo de entrada\n";
        return;
    }

    size_t cantidad_por_run = M / sizeof(int64_t);
    vector<int64_t> buffer(cantidad_por_run);

    int i = 0;
    while (entrada.read(reinterpret_cast<char*>(buffer.data()), M)) {
        sort(buffer.begin(), buffer.end());
        string nombre_run = "run_" + to_string(i++) + ".bin";
        ofstream run(nombre_run, ios::binary);
        run.write(reinterpret_cast<char*>(buffer.data()), M);
        run.close();
        archivos_runs.push_back(nombre_run);
    }

    // último bloque parcial
    size_t leidos = entrada.gcount() / sizeof(int64_t);
    if (leidos > 0) {
        sort(buffer.begin(), buffer.begin() + leidos);
        string nombre_run = "run_" + to_string(i++) + ".bin";
        ofstream run(nombre_run, ios::binary);
        run.write(reinterpret_cast<char*>(buffer.data()), leidos * sizeof(int64_t));
        run.close();
        archivos_runs.push_back(nombre_run);
    }

    entrada.close();
}

void mergeRuns(vector<string>& archivos_runs, string archivo_salida, size_t B) {
    size_t k = archivos_runs.size(); //Cantidad de archivos temporales hay que mezclar -> k-way
    vector<ifstream> inputs(k); //Se define ifstream para cada archivo
    vector<vector<int64_t>> buffers(k); //buffers para guardar bloques de tamaño B
    vector<size_t> indices(k, 0);//Indices de posicion dentro de cada buffers
    vector<size_t> tamanos(k, 0);//Tamaño real de datos cargados en buffer

    /* 
    Abrir todos los archivos y leer su primer bloque
    Primera parte de k-way
    */
    for (size_t i = 0; i < k; ++i) {
        inputs[i].open(archivos_runs[i], ios::binary);
        buffers[i].resize(B / sizeof(int64_t));
        inputs[i].read(reinterpret_cast<char*>(buffers[i].data()), B);
        tamanos[i] = inputs[i].gcount() / sizeof(int64_t);//Bits que se leyeron realmente
        indices[i] = 0;
    }

    ofstream salida(archivo_salida, ios::binary);

    while (true) {
        int64_t minimo = INT64_MAX;
        int idx_min = -1; //indice del buffer con numeros menores

        // Buscar el menor valor actual entre todos los buffers
        for (size_t i = 0; i < k; ++i) {
            if (indices[i] < tamanos[i] && buffers[i][indices[i]] < minimo) {
                minimo = buffers[i][indices[i]];
                idx_min = i;
            }
        }

        if (idx_min == -1) 
            break; // todos los buffers vacíos

        // Escribir el mínimo en el archivo de salida
        salida.write(reinterpret_cast<char*>(&minimo), sizeof(int64_t));
        indices[idx_min]++;

        // Si el buffer se vació, cargar otro bloque
        if (indices[idx_min] == tamanos[idx_min]) {
            inputs[idx_min].read(reinterpret_cast<char*>(buffers[idx_min].data()), B);
            tamanos[idx_min] = inputs[idx_min].gcount() / sizeof(int64_t);
            indices[idx_min] = 0;
        }
    }

    salida.close();
    for (auto& in : inputs) in.close();
}

void mergesortExterno(string archivo_entrada, string archivo_salida, size_t M, size_t B) {
    vector<string> archivos_runs;
    generarRuns(archivo_entrada, M, B, archivos_runs);
    mergeRuns(archivos_runs, archivo_salida, B);
}