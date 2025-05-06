#pragma once
#include <vector>       // <-- esto faltaba
#include <cstdint>      // si usas int64_t
using namespace std;    // o usar std::vector explícitamente
#ifndef ORDENAMIENTOS_H
#define ORDENAMIENTOS_H
using namespace std;


void crearArchivo(string nombre_archivo, int64_t N);
vector<int64_t> leerArchivo(string nombre_archivo, size_t B);
void mergesortExterno(string archivo_entrada, string archivo_salida, size_t M, size_t B, size_t a);
void quicksortExterno(string archivo_entrada, string archivo_salida, size_t M, size_t B, size_t a);

#endif