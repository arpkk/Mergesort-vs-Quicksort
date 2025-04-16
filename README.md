# Mergesort Externo vs Quicksort Externo

## Descripción

Este repositorio contiene una implementación en C++ de los algoritmos de **Mergesort Externo** y **Quicksort Externo**. La tarea tiene como objetivo comparar ambos algoritmos en términos de tiempo de ejecución y accesos a disco al trabajar con arreglos grandes que no caben completamente en memoria principal.

### Objetivos de la Tarea

- **Implementación de Mergesort Externo:** Basado en las clases y ejemplos presentados en clase, el Mergesort externo se utiliza para ordenar grandes volúmenes de datos almacenados en disco.
- **Implementación de Quicksort Externo:** Una versión modificada de Quicksort que trabaja en memoria secundaria, realizando particiones de un arreglo grande en subarreglos para ser procesados recursivamente.

### Metodología

- **Trabajar con disco:** Los algoritmos leen y escriben datos en bloques de tamaño fijo desde/para archivos binarios. Esto asegura que el uso de memoria sea eficiente y controlado.
- **Docker:** El entorno de ejecución está configurado en Docker para simular condiciones de memoria limitada y garantizar que los experimentos sean reproducibles [cc4102-cpp-env](https://hub.docker.com/r/pabloskewes/cc4102-cpp-env).

### Estructura del Proyecto

- `src/` - Contiene la implementación en C++ de los algoritmos Mergesort Externo y Quicksort Externo.

### Requisitos

- C++
- Docker (para ejecutar el proyecto en un entorno controlado)
  
### Instrucciones de Ejecución

1. **Compilar el Proyecto**
   ```bash
   g++ ...
