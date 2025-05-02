#ifndef ORDENAMIENTOS_H
#define ORDENAMIENTOS_H
using namespace std;


void crearArchivo(string nombre_archivo, int64_t cantidad_numeros);
vector<int64_t> leerEscribirBloque(string nombre_archivo, size_t B);
void mergesortExterno(string archivo_entrada, string archivo_salida, size_t M, size_t B);
//void quickSort(int arr[], int left, int right);

#endif