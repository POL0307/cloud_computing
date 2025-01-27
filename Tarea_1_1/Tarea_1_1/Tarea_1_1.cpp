// Tarea_1_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int* arregloA, * arregloB, * arregloC;

void mostrar(int cantidad) {
    int limite = 10; // Mostrar solo los primeros y últimos 10 valores

    cout << "> Arreglo A (primeros y últimos 10 valores):\n";
    for (int i = 0; i < limite; i++) {
        cout << arregloA[i] << (i == limite - 1 ? " " : ", ");
    }
    cout << "...";
    for (int i = cantidad - limite; i < cantidad; i++) {
        cout << (i == cantidad - limite ? " " : ", ") << arregloA[i];
    }
    cout << "\n";

    cout << "> Arreglo B (primeros y últimos 10 valores):\n";
    for (int i = 0; i < limite; i++) {
        cout << arregloB[i] << (i == limite - 1 ? " " : ", ");
    }
    cout << "...";
    for (int i = cantidad - limite; i < cantidad; i++) {
        cout << (i == cantidad - limite ? " " : ", ") << arregloB[i];
    }
    cout << "\n";

    cout << "> Arreglo C (primeros y últimos 10 valores):\n";
    for (int i = 0; i < limite; i++) {
        cout << arregloC[i] << (i == limite - 1 ? " " : ", ");
    }
    cout << "...";
    for (int i = cantidad - limite; i < cantidad; i++) {
        cout << (i == cantidad - limite ? " " : ", ") << arregloC[i];
    }
    cout << "\n";
}

void sumarSecuencial(int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        arregloC[i] = arregloA[i] + arregloB[i];
    }
    cout << "Suma secuencial completada.\n";
}

void sumarParalela(int cantidad, int hilos) {
    int* suma_por_hilo = new int[hilos](); // Inicializamos el arreglo para acumular las sumas por hilo

    // Suma paralela con OpenMP
#pragma omp parallel for num_threads(hilos)
    for (int i = 0; i < cantidad; i++) {
        int id_hilo = omp_get_thread_num();
        arregloC[i] = arregloA[i] + arregloB[i];
        suma_por_hilo[id_hilo] += arregloC[i]; // Acumula la suma de cada hilo

        // Imprimir qué índice está procesando cada hilo
#pragma omp critical
        cout << "Hilo " << id_hilo << " procesó índice " << i
            << " (Suma: " << arregloA[i] << " + " << arregloB[i]
            << " = " << arregloC[i] << ")\n";
    }

    cout << "Suma paralela con " << hilos << " hilos completada.\n";

    // Mostrar las sumas acumuladas por cada hilo
    for (int i = 0; i < hilos; i++) {
        cout << "Hilo " << i << " acumuló: " << suma_por_hilo[i] << endl;
    }

    delete[] suma_por_hilo;
}

void aleatorio(int cantidad) {
    // Asignamos memoria dinámica para los arreglos
    arregloA = new int[cantidad];
    arregloB = new int[cantidad];
    arregloC = new int[cantidad] {0}; // Inicializamos el arreglo C en cero

    srand(time(0)); // Semilla para generar números aleatorios
    for (int i = 0; i < cantidad; i++) {
        arregloA[i] = rand() % 100; // Números aleatorios entre 0 y 99
        arregloB[i] = rand() % 100;
    }
}

void liberarMemoria() {
    delete[] arregloA;
    delete[] arregloB;
    delete[] arregloC;
}

int main() {
    int cantidad, hilos, opcion;

    cout << "Ingrese el número de registros (ejemplo: 1000): ";
    cin >> cantidad;

    aleatorio(cantidad);

    cout << "\nSelecciona una opción:\n";
    cout << "1. Realizar la suma de los arreglos secuencialmente\n";
    cout << "2. Realizar la suma de los arreglos en paralelo\n";
    cout << "Opción: ";
    cin >> opcion;

    if (opcion == 1) {
        sumarSecuencial(cantidad);
    }
    else if (opcion == 2) {
        cout << "Ingrese el número de hilos para la suma paralela: ";
        cin >> hilos;
        sumarParalela(cantidad, hilos);
    }
    else {
        cout << "Opción no válida.\n";
        liberarMemoria();
        return 1;
    }

    // Mostrar los resultados
    mostrar(cantidad);

    liberarMemoria();
    return 0;
}
