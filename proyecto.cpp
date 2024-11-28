#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Dimensiones máximas de las secuencias
const int MAXN = 1001;
const int MAXM = 1001;

// Matriz de emparejamiento (para A, G, C, T)
int matrizEmparejamiento[4][4]; // 4x4 matriz para A, G, C, T
char alfabeto[4] = {'A', 'G', 'C', 'T'}; // Índices para la matriz

// Función para convertir un carácter en un índice (0-3)
int indiceCaracter(char c) {
    for (int i = 0; i < 4; i++) {
        if (alfabeto[i] == c) {
            return i;
        }
    }
    return -1; // Retorna -1 si el carácter no está en el alfabeto
}

string leerSecuencia(const string &archivo) {
    cout << "Intentando abrir el archivo: " << archivo << endl;
    ifstream archivoEntrada(archivo);
    if (!archivoEntrada.is_open()) {
        cerr << "Error al abrir el archivo: " << archivo << endl;
        exit(EXIT_FAILURE);
    }

    string secuencia, linea;
    while (getline(archivoEntrada, linea)) {
        secuencia += linea;
    }
    archivoEntrada.close();
    cout << "Secuencia leída: " << secuencia << endl;
    return secuencia;
}

void leerMatrizEmparejamiento(const string &archivo) {
    cout << "Intentando abrir el archivo de matriz: " << archivo << endl;
    ifstream archivoEntrada(archivo);
    if (!archivoEntrada.is_open()) {
        cerr << "Error al abrir el archivo: " << archivo << endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            archivoEntrada >> matrizEmparejamiento[i][j];
            cout << "matrizEmparejamiento[" << i << "][" << j << "] = " << matrizEmparejamiento[i][j] << endl;
        }
    }
    archivoEntrada.close();
}

// Algoritmo de Needleman-Wunsch
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
                f[i][j] = max(
                    f[i - 1][j] + V, // No emparejar S[i]
                    max(
                        f[i][j - 1] + V, // No emparejar T[j]
                        f[i - 1][j - 1] + matrizEmparejamiento[indiceCaracter(S[i - 1])][indiceCaracter(T[j - 1])] // Emparejar S[i] con T[j]
                    )
                );
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
        } else if (i > 0 && f[i][j] == f[i - 1][j] + V) {
            alineamientoS = S[i - 1] + alineamientoS;
            alineamientoT = '-' + alineamientoT;
            i--;
        } else if (j > 0 && f[i][j] == f[i][j - 1] + V) {
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

int main(int argc, char *argv[]) {
    cout << "Iniciando programa..." << endl;

    if (argc != 9) {
        cerr << "Uso: ./programa -C1 archivo1 -C2 archivo2 -U matriz -V valor" << endl;
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

    //probando que los parametros de entrada funcionesn bien
    cout << "Archivos procesados:" << endl;
    cout << "Secuencia 1: " << archivoS << endl;
    cout << "Secuencia 2: " << archivoT << endl;
    cout << "Matriz: " << archivoU << endl;
    cout << "Valor de hueco: " << V << endl;

    string S = leerSecuencia(archivoS);
    string T = leerSecuencia(archivoT);
    leerMatrizEmparejamiento(archivoU);

    cout << "Secuencia S: " << S << endl;
    cout << "Secuencia T: " << T << endl;

    needlemanWunsch(S, T, V);

    cout << "Programa finalizado." << endl;
    return 0;
}
