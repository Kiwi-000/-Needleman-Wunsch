#include <iostream>
#include <fstream> // manejo de archivos 
#include <sstream> 
#include <string>
#include <algorithm>
#include <map> // Manejo de funU.txt

using namespace std;

//A = {'A', 'G', 'C', 'T'}
//Donde C1 , S = CYTB.txt 
//Donde C2 , T = CYTB2.txt
// U = funU.txt

// Dimensiones máximas de las secuencias
const int MAXN = 1001;
const int MAXM = 1001;


string leerSecuencia(const string &archivo)
void leerMatrizEmparejamiento(const string &archivo) {}


//------------Algoritmo Needleman-Wunsch---------------------------

void needlemanWunsch(const string &S, const string &T, int V) {
    int n = S.size();
    int m = T.size();

    // Matriz de puntajes
    int f[MAXN + 1][MAXM + 1] = {0};

    // Inicializar la matriz
    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            if (i == 0 && j == 0) {
                f[i][j] = 0; // Caso base
            } else if (i > 0 && j == 0) {
                f[i][j] = f[i - 1][j] + V; // Caso: segunda cadena vacía
            } else if (i == 0 && j > 0) {
                f[i][j] = f[i][j - 1] + V; // Caso: primera cadena vacía
            } else {
                f[i][j] = max({
                    f[i - 1][j] + V, // No emparejar S[i]
                    f[i][j - 1] + V, // No emparejar T[j]
                    f[i - 1][j - 1] + U(S[i - 1], T[j - 1]) // Emparejar S[i] con T[j]
                });
            }
        }
    }

    // El puntaje máximo está en f[n][m]
    cout << "Puntaje máximo: " << f[n][m] << endl;

    // Reconstrucción del alineamiento
    string alineamientoS = "";
    string alineamientoT = "";
    int i = n, j = m;

    while (i > 0 || j > 0) {
        if (i > 0 && j == 0) {
            alineamientoS = S[i - 1] + alineamientoS;
            alineamientoT = '-' + alineamientoT;
            i--;
        } else if (i == 0 && j > 0) {
            alineamientoS = '-' + alineamientoS;
            alineamientoT = T[j - 1] + alineamientoT;
            j--;
        } else if (i > 0 && j > 0 && f[i][j] == f[i - 1][j] + V) {
            alineamientoS = S[i - 1] + alineamientoS;
            alineamientoT = '-' + alineamientoT;
            i--;
        } else if (i > 0 && j > 0 && f[i][j] == f[i][j - 1] + V) {
            alineamientoS = '-' + alineamientoS;
            alineamientoT = T[j - 1] + alineamientoT;
            j--;
        } else {
            alineamientoS = S[i - 1] + alineamientoS;
            alineamientoT = T[j - 1] + alineamientoT;
            i--;
            j--;
        }
    }

    cout << "Alineamiento de S: " << alineamientoS << endl;
    cout << "Alineamiento de T: " << alineamientoT << endl;
}

//Respuestas a entregar:
//Mostrar como queda la matriz 
//Calcular el puntaje maximo de emparejamiento
//Mostrar emparejamiento que da el puntaje maximo 

// Función principal
int main(int argc, char *argv[]) {
    cout << "Iniciando programa..." << endl;

    if (argc != 9) {
        cerr << "Uso: ./proyecto -C1 CYTB.txt -C2 CYTB2.txt -U funU.txt -V val" << endl;
        return EXIT_FAILURE;
    }

    string archivoS, archivoT, archivoU;
    int V;

    for (int i = 1; i < argc; i += 2) {
        string flag = argv[i];
        if (flag == "-C1") archivoS = argv[i + 1];
        else if (flag == "-C2") archivoT = argv[i + 1];
        else if (flag == "-U") archivoU = argv[i + 1];
        else if (flag == "-V") V = stoi(argv[i + 1]);
        else {
            cerr << "Parámetro desconocido: " << flag << endl;
            return EXIT_FAILURE;
        }
    }

    string S = leerSecuencia(archivoS);
    string T = leerSecuencia(archivoT);
    leerMatrizEmparejamiento(archivoU);

    cout << "Secuencia S: " << S << endl;
    cout << "Secuencia T: " << T << endl;

    needlemanWunsch(S, T, V);

    cout << "Programa finalizado." << endl;
    return 0;
}