#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
using namespace std;

void quicksortExterno(string archivo_entrada, string archivo_salida, size_t M, size_t B, size_t a) {
    ifstream entrada(archivo_entrada, ios::binary | ios::ate);
    size_t N = entrada.tellg();
    entrada.close();

    size_t n = N / sizeof(int64_t); //8 bytes 
    if (N <= M) {
        // Cargar todo en memoria y ordenar directamente
        vector<int64_t> datos(n);
        ifstream in(archivo_entrada, ios::binary);
        in.read(reinterpret_cast<char*>(datos.data()), N);
        in.close();
        sort(datos.begin(), datos.end());
        ofstream salida(archivo_salida, ios::binary);
        salida.write(reinterpret_cast<char*>(datos.data()), N);
        salida.close();
        return;
    }

    // Elegir pivotes (muestra aleatoria de tamaño a, asumimos que caben en un bloque)
    vector<int64_t> muestra(a);
    ifstream in(archivo_entrada, ios::binary);
    for (size_t i = 0; i < a; ++i) {
        size_t pos = (rand() % n) * sizeof(int64_t);
        in.seekg(pos);
        in.read(reinterpret_cast<char*>(&muestra[i]), sizeof(int64_t));
    }
    in.close();
    sort(muestra.begin(), muestra.end());

    // Particionar archivo en a+1 archivos temporales
    ifstream original(archivo_entrada, ios::binary);
    vector<ofstream> particiones(a + 1);
    for (size_t i = 0; i <= a; ++i) {
        particiones[i].open("part_" + to_string(i) + ".bin", ios::binary);
    }

    vector<int64_t> buffer(B / sizeof(int64_t));
    while (original.read(reinterpret_cast<char*>(buffer.data()), B)) {
        size_t leidos = original.gcount() / sizeof(int64_t);
        for (size_t i = 0; i < leidos; ++i) {
            int64_t val = buffer[i];
            size_t idx = 0;
            while (idx < a && val > muestra[idx]) idx++;
            particiones[idx].write(reinterpret_cast<char*>(&val), sizeof(int64_t));
        }
    }

    // Último bloque parcial si quedó algo
    size_t leidos = original.gcount() / sizeof(int64_t);
    if (leidos > 0) {
        for (size_t i = 0; i < leidos; ++i) {
            int64_t val = buffer[i];
            size_t idx = 0;
            while (idx < a && val > muestra[idx]) idx++;
            particiones[idx].write(reinterpret_cast<char*>(&val), sizeof(int64_t));
        }
    }

    original.close();
    for (auto& p : particiones) p.close();

    // Ordenar recursivamente cada partición
    for (size_t i = 0; i <= a; ++i) {
        quicksortExterno("part_" + to_string(i) + ".bin", "salida_" + to_string(i) + ".bin", M, B, a);
    }

    // Combinar particiones
    ofstream final(archivo_salida, ios::binary);
    for (size_t i = 0; i <= a; ++i) {
        ifstream part("part_" + to_string(i) + ".bin", ios::binary);
        final << part.rdbuf();
        part.close();
        remove(("part_" + to_string(i) + ".bin").c_str());
        remove(("salida_" + to_string(i) + ".bin").c_str());
    }
    final.close();
}