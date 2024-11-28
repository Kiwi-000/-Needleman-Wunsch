Programa de alineamiento entre dos secuencias 
..........Algoritmo Needlema-Wunch...........

Secuencias a alinear y sus archivos: 
    CYTB1.txt -> Gen CYTB en Macaca Mulatta
    CYTB2.txt -> Gen CYTB en Alouatta Seniculus

Alfabeto:
    A = {'A', 'G', 'C', 'T'}

Matriz de emparejamiento: 
    [funU.txt]

        Inicial :
        - A G C T
        A 1 -1 -1 -1
        G -1 1 -1 -1
        C -1 -1 1 -1
        T -1 -1 -1 1

Compilacion del Programa:
    :~$ ./proyecto -C1 CYTB.txt -C2 CYTB2.txt -U funU.tex -V val
