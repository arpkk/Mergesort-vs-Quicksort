#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>
#include <cstdlib>

/*
Implementación de Quicksort Externo para ordenamiento en memoria secundaria.
Este algoritmo ordena un archivo binario de enteros (`archivo_entrada`) usando una variante de Quicksort adaptada a memoria externa.
Utiliza una muestra aleatoria de tamaño `a` para seleccionar pivotes, divide los datos en `a+1` particiones, 
ordena recursivamente cada partición y luego las combina en un archivo de salida (`archivo_salida`).

Parámetros:
- archivo_entrada: nombre del archivo binario con los datos a ordenar.
- archivo_salida: nombre del archivo binario de salida con los datos ordenados.
- M: Tamaño de la memoria principal disponible.
- B: Tamaño del bloque de transferencia entre memoria y disco.
- a: Cantidad de pivotes (determina cuántas particiones se crearán).

También registra la cantidad total de accesos a disco (lecturas y escrituras) en un archivo temporal `temp_ios.txt`.
 */

size_t lecturas_disco = 0;
size_t escrituras_disco = 0;
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
        lecturas_disco += (N + B - 1) / B;
        sort(datos.begin(), datos.end());
        ofstream salida(archivo_salida, ios::binary);
        escrituras_disco += (N + B - 1) / B;
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
    lecturas_disco += a;
    sort(muestra.begin(), muestra.end());

    // Particionar archivo en a+1 archivos temporales
    ifstream original(archivo_entrada, ios::binary);
    vector<ofstream> particiones(a + 1);
    for (size_t i = 0; i <= a; ++i) {
        particiones[i].open("part_" + to_string(i) + ".bin", ios::binary);
    }

    vector<int64_t> buffer(B / sizeof(int64_t));
    while (original.read(reinterpret_cast<char*>(buffer.data()), B)) {
        lecturas_disco++;
        escrituras_disco++;
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
        lecturas_disco++;
        escrituras_disco++;
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
        part.seekg(0, ios::end);
        size_t size_part = part.tellg();
        part.seekg(0);
        lecturas_disco += (size_part + B - 1) / B;
        escrituras_disco += (size_part + B - 1) / B;
        final << part.rdbuf();
        part.close();
        remove(("part_" + to_string(i) + ".bin").c_str());
        remove(("salida_" + to_string(i) + ".bin").c_str());
    }
    final.close();

    // Guardar el total de lecturas + escrituras en un archivo temporal
    ofstream ios_temp("temp_ios.txt", ios::out);
    ios_temp << (lecturas_disco + escrituras_disco);
    ios_temp.close();

}