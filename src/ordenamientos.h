#pragma once
#include <vector>       // <-- esto faltaba
#include <cstdint>      // si usas int64_t
using namespace std;    // o usar std::vector explícitamente
vector<int64_t> leerEscribirBloque(string nombre_archivo, size_t B);
#ifndef ORDENAMIENTOS_H
#define ORDENAMIENTOS_H
using namespace std;


void crearArchivo(string nombre_archivo, int64_t cantidad_numeros);
vector<int64_t> leerEscribirBloque(string nombre_archivo, size_t B);
void mergesortExterno(string archivo_entrada, string archivo_salida, size_t M, size_t B, size_t a);
//void quickSort(int arr[], int left, int right);

#endif