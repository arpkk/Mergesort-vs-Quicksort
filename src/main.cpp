#include <iostream>
#include <chrono>
#include "ordenamientos.h"

size_t mejorA (size_t M, size_t b) {
    size_t mejorA = 2;
    size_t N = 62914560 / sizeof(int64_t);
    double mejorTiempo = numeric_limits<double>::max();
    //size_t N_a = 60 * 1024 * 1024; // 60 millones de enteros
    //size_t N_a = 60 * 1024; // 60 mil de enteros
    
    crearArchivo("entrada_para_a.bin", N);

    for (size_t a = 2; a <= 512; a *= 2) {  // prueba potencias de 2 hasta b = 64
        string salidaA = "salida_para_a.bin";

        auto inicio = chrono::high_resolution_clock::now();
        mergesortExterno("entrada_para_a.bin", salidaA, M, b, a);
        auto fin = chrono::high_resolution_clock::now();

        chrono::duration<double> duracion = fin - inicio;
        cout << "a = " << a << ": " << duracion.count() << " segundos\n";

        if (duracion.count() < mejorTiempo) {
            mejorTiempo = duracion.count();
            mejorA = a;
        }

        remove(salidaA.c_str());
        for (int j = 0; ; ++j) {
            string runfile = "run_" + to_string(j) + ".bin";
            if (remove(runfile.c_str()) != 0) break;
        }
    }

    cout << "Mejor valor de a encontrado: " << mejorA << " con tiempo: " << mejorTiempo << " segundos\n";

    remove("entrada_para_a.bin");
    return mejorA;
}

int main() {

    cout << "\n\n-----------------------Búsqueda binaria para mejor valor de a-----------------------\n\n";
    size_t M = 52428800; // memoria en bytes
    size_t B = 4096;      // tamaño del bloque en bytes
    size_t b = B / sizeof(int64_t);
    size_t a = mejorA(M, b); //aridad
    size_t N = M / sizeof(int64_t); // tamaño de archivo binario


    cout << "\n\n-----------------------Datos Entrada-----------------------\n\n";
    crearArchivo("numeros.bin", N);
    /*vector<int64_t> datos = leerArchivo("numeros.bin", b);
    for (int64_t n : datos) {
        cout << n << " ";
    }
    */

    cout << "\n\n-----------------------Datos Salida MergeSort-----------------------\n\n";
    
    mergesortExterno("numeros.bin", "salida.bin", M, b, a);
    /*vector<int64_t> datosSalida = leerArchivo("salida.bin", b);
    for (int64_t n : datosSalida) {
        cout << n << " ";
    }
    */

    cout << "\n\n-----------------------Datos Salida 5 secuencias MergeSort-----------------------\n\n";

    for (int i = 1; i <= 5; ++i) {
        string nombreEntrada = "entrada_" + to_string(N) + "_" + to_string(i) + ".bin";
        string nombreSalida = "salida_" + to_string(N) + "_" + to_string(i) + ".bin";

        crearArchivo(nombreEntrada, N); // genera N números aleatorios de 64 bits

        auto inicio = chrono::high_resolution_clock::now();
        mergesortExterno(nombreEntrada, nombreSalida, M, b, a);
        auto fin = chrono::high_resolution_clock::now();

        chrono::duration<double> duracion = fin - inicio;
        cout << "Secuencia " << i << ": " << duracion.count() << " segundos\n";

        remove(nombreEntrada.c_str());
        remove(nombreSalida.c_str());
    }

    /*cout << "\n\n-----------------------Datos Salida 5 4M, 8M, ..., 60M MergeSort-----------------------\n\n";

    M = 52428800; // 50MB en bytes
    
    for (int mult = 4; mult <= 60; mult += 4) {
        size_t N = mult * M; // cantidad de elementos
    
        for (int i = 1; i <= 5; ++i) {
            string nombreEntrada = "entrada_" + to_string(mult) + "M_" + to_string(i) + ".bin";
            string nombreSalida = "salida_" + to_string(mult) + "M_" + to_string(i) + ".bin";
    
            crearArchivo(nombreEntrada, N); // genera N números aleatorios de 64 bits
    
            auto inicio = chrono::high_resolution_clock::now();
            mergesortExterno(nombreEntrada, nombreSalida, M, b, a);
            auto fin = chrono::high_resolution_clock::now();
    
            chrono::duration<double> duracion = fin - inicio;
            cout << "N = " << mult << "M, Secuencia " << i << ": " << duracion.count() << " segundos\n";
            
            remove(nombreEntrada.c_str());
            remove(nombreSalida.c_str());
            for (int j = 0; ; ++j) {
                string runfile = "run_" + to_string(j) + ".bin";
                if (remove(runfile.c_str()) != 0) break;
            }
        }
    }*/

    cout << "\n\n-----------------------Datos Salida QuickSort-----------------------\n\n";
    
    quicksortExterno("numeros.bin", "salida.bin", M, b, a);
    /*vector<int64_t> datosSalida1 = leerArchivo("salida.bin", b);
    for (int64_t n : datosSalida1) {
        cout << n << " ";
    }
    */

    cout << "\n\n-----------------------Datos Salida 5 secuencias QuickSort-----------------------\n\n";

    for (int i = 1; i <= 5; ++i) {
        string nombreEntrada = "entrada_" + to_string(N) + "_" + to_string(i) + ".bin";
        string nombreSalida = "salida_" + to_string(N) + "_" + to_string(i) + ".bin";

        crearArchivo(nombreEntrada, N); // genera N números aleatorios de 64 bits

        auto inicio = chrono::high_resolution_clock::now();
        quicksortExterno(nombreEntrada, nombreSalida, M, b, a);
        auto fin = chrono::high_resolution_clock::now();

        chrono::duration<double> duracion = fin - inicio;
        cout << "Secuencia " << i << ": " << duracion.count() << " segundos\n";

        remove(nombreEntrada.c_str());
        remove(nombreSalida.c_str());
    }

    cout << "\n\n-----------------------Datos Salida 5 4M, 8M, ..., 60M QuickSort-----------------------\n\n";

    M = 52428800; // 50MB en bytes
    double tiempoTotal = 0;

    cout << "Se inicia 75 secuencias para QuickSort con M=" << M << ", N=" << N << ", B=" << B << ", b=" << b << "y a=" << a << "\n";

    for (int mult = 4; mult <= 60; mult += 4) {
        size_t N = mult * M / sizeof(int64_t); // tamaño en MB, se divide en 8 para obtener la cantidad de int64_t
    
        cout << "Secuencia con N=" << N << "\n";

        for (int i = 1; i <= 5; ++i) {
            string nombreEntrada = "entrada_" + to_string(mult) + "M_" + to_string(i) + ".bin";
            string nombreSalida = "salida_" + to_string(mult) + "M_" + to_string(i) + ".bin";
    
            crearArchivo(nombreEntrada, N); // genera N números aleatorios de 64 bits
    
            auto inicio = chrono::high_resolution_clock::now();
            quicksortExterno(nombreEntrada, nombreSalida, M, b, a);
            auto fin = chrono::high_resolution_clock::now();
    
            chrono::duration<double> duracion = fin - inicio;
            cout << "N = " << mult << "M, Secuencia " << i << ": " << duracion.count() << " segundos\n";
            
            tiempoTotal += duracion.count();

            remove(nombreEntrada.c_str());
            remove(nombreSalida.c_str());
            for (int j = 0; ; ++j) {
                string runfile = "run_" + to_string(j) + ".bin";
                if (remove(runfile.c_str()) != 0) break;
            }
        }

        tiempoTotal = 0;

    }

    return 0;
}