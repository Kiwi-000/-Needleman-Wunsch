Alineamiento de Secuencias Biológicas usando Needleman-Wunsch

Este programa implementa el algoritmo de Needleman-Wunsch para realizar el alineamiento de secuencias biológicas, calculando la similitud entre dos cadenas de ADN. Utiliza una matriz de emparejamiento para determinar 
puntuaciones y genera alineamientos visualizados gráficamente.

> Requisitos del sistema:
    - Lenguaje: C++
    - Dependencias:
        Herramienta Graphviz para la visualización gráfica (comando dot).

> Secuencias a alinear y sus archivos: 
    CYTB1.txt -> Gen CYTB en Macaca Mulatta
    CYTB2.txt -> Gen CYTB en Alouatta Seniculus

> Matriz de emparejamiento [funU.txt]
        - A G C T
        A 1 -1 -1 -1
        G -1 1 -1 -1
        C -1 -1 1 -1
        T -1 -1 -1 1

> Ejecución del Programa:
    :~$ ./proyecto -C1 CYTB.txt -C2 CYTB2.txt -U funU.tex -V val

> Parámetros:
    -C1: Archivo que contiene la primera secuencia de ADN.
    -C2: Archivo que contiene la segunda secuencia de ADN.
    -U: Archivo que define la matriz de emparejamiento.
    -V: Valor entero para la penalización por gaps.

Antonia Rojas (2024)
