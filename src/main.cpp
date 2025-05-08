#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip>
#include "ordenamientos.h"
using namespace std;


size_t mejorA (size_t M, size_t b) {
    cout << "\n\n-----------------------Búsqueda binaria para mejor valor de a-----------------------\n\n";
    size_t mejorA = 2;
    size_t N = 62914560 / sizeof(int64_t); //60MB segun el enunciado
    double mejorTiempo = numeric_limits<double>::max();
    
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

    size_t M = 200; // M de tamaño 100 para la demostracion de ordenamiento
    size_t B = 4096; // tamaño del bloque en bytes
    size_t b = B / sizeof(int64_t);
    size_t a = 64; //a fijo para la demostracion de ordenamiento, luego se calcula el mejor a.
    size_t N = M / sizeof(int64_t); // tamaño de archivo binario


    cout << "\n\n-----------------------Datos Entrada (Demostracion de ordenamiento)-----------------------\n\n";
    crearArchivo("numeros.bin", N);
    vector<int64_t> datos = leerArchivo("numeros.bin", b);
    for (int64_t n : datos) {
        cout << n << " ";
    }

    cout << "\n\n-----------------------Datos Salida MergeSort (Demostracion de ordenamiento)-----------------------\n\n";
    
    mergesortExterno("numeros.bin", "salida.bin", M, b, a);
    vector<int64_t> datosSalida = leerArchivo("salida.bin", b);
    for (int64_t n : datosSalida) {
        cout << n << " ";
    }

    cout << "\n\n-----------------------Datos Salida QuickSort (Demostracion de ordenamiento)-----------------------\n\n";

    quicksortExterno("numeros.bin", "salida.bin", M, b, a);
    datosSalida = leerArchivo("salida.bin", b);
    for (int64_t n : datosSalida) {
        cout << n << " ";
    }

    /************************* Se cambian valores de variables para hacer pruebas reales *************************/

    //a = mejorA(M, b);
    a = 64;
    M = 52428800 / sizeof(int64_t); // 50MB en bytes
    B = 4096; // tamaño del bloque en bytes
    b = B / sizeof(int64_t);
    
    cout << "\n\n-----------------------Datos Salida 5 4M, 8M, ..., 60M MergeSort-----------------------\n\n";

    cout << "Se inicia 75 secuencias para MergeSort con M = " << M << ", N = " << N << ", B = " << B << ", b = " << b << " y a = " << a << "\n";

    ofstream csvFileMerge("resultados_mergesort.csv");
    csvFileMerge << "N_elementos,Tiempo_promedio_segundos,Total_IOs\n";
    csvFileMerge << std::fixed << std::setprecision(6);

    size_t totalIOs = 0;
    size_t ios_actual = 0;
    double tiempoTotal = 0;
    double promedioTiempo = 0;
    
    for (int mult = 4; mult <= 60; mult += 4) {
        size_t N = mult * M; // cantidad de elementos

        totalIOs = 0;
        ios_actual = 0;
        tiempoTotal = 0;

        cout << "MergeSort: Secuencia con N=" << N << "\n";
    
        for (int i = 1; i <= 5; ++i) {
            string nombreEntrada = "entrada_" + to_string(mult) + "M_" + to_string(i) + ".bin";
            string nombreSalida = "salida_" + to_string(mult) + "M_" + to_string(i) + ".bin";
    
            crearArchivo(nombreEntrada, N); // genera N números aleatorios de 64 bits
    
            auto inicio = chrono::high_resolution_clock::now();
            mergesortExterno(nombreEntrada, nombreSalida, M, b, a);
            auto fin = chrono::high_resolution_clock::now();

            ifstream ios_in("temp_ios.txt");
            ios_actual = 0;
            ios_in >> ios_actual;
            ios_in.close();
            totalIOs += ios_actual;
    
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
        double promedioTiempo = tiempoTotal / 5;
        csvFileMerge << N << ";" 
             << to_string(promedioTiempo).replace(to_string(promedioTiempo).find('.'), 1, ",") 
             << ";" << totalIOs << "\n";
    }
    csvFileMerge.close();
    remove("temp_ios.txt");

    cout << "\n\n-----------------------Datos Salida 5 4M, 8M, ..., 60M QuickSort-----------------------\n\n";

    tiempoTotal = 0;
    promedioTiempo = 0;
    totalIOs = 0;
    ios_actual = 0;

    cout << "Se inicia 75 secuencias para QuickSort con M = " << M << ", N = " << N << ", B = " << B << ", b = " << b << " y a=" << a << "\n";

    ofstream csvFile("resultados_quicksort.csv");
    csvFile << "N_elementos,Tiempo_promedio_segundos,Total_IOs\n";
    csvFile << std::fixed << std::setprecision(6);

    for (int mult = 4; mult <= 60; mult += 4) {
        size_t N = mult * M; // tamaño en MB, se divide en 8 para obtener la cantidad de int64_t
    
        cout << "QuickSort: Secuencia con N=" << N << "\n";

        totalIOs = 0;
        ios_actual = 0;


        for (int i = 1; i <= 5; ++i) {
            string nombreEntrada = "entrada_" + to_string(mult) + "M_" + to_string(i) + ".bin";
            string nombreSalida = "salida_" + to_string(mult) + "M_" + to_string(i) + ".bin";
    
            crearArchivo(nombreEntrada, N); // genera N números aleatorios de 64 bits
    
            auto inicio = chrono::high_resolution_clock::now();
            quicksortExterno(nombreEntrada, nombreSalida, M, b, a);
            auto fin = chrono::high_resolution_clock::now();

            ifstream ios_in("temp_ios.txt");
            ios_actual = 0;
            ios_in >> ios_actual;
            ios_in.close();
            totalIOs += ios_actual;
    
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
        promedioTiempo = tiempoTotal / 5;
        csvFile << N << ";" 
        << to_string(promedioTiempo).replace(to_string(promedioTiempo).find('.'), 1, ",") 
        << ";" << totalIOs << "\n";
        tiempoTotal = 0;
    }
    remove("temp_ios.txt");
    csvFile.close();

    return 0;
}