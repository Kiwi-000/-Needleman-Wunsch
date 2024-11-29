#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
//#include <unordered_map>
//#include <cstdlib>  // Para usar system()

using namespace std;

// Dimensiones máximas de las secuencias
const int MAXN = 1001;
const int MAXM = 1001;

// Matriz de emparejamiento
int matrizEmparejamiento[4][4];
char alfabeto[4] = {'A', 'G', 'C', 'T'};

// Función para convertir un carácter en un índice 
int indiceCaracter(char c) {
    for (int i = 0; i < 4; i++) {
        if (alfabeto[i] == c){
            return i;
        }
    }
    return -1;
}

// Leer la secuencia de un archivo
string leerSecuencia(const string &archivo) {
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
    //cout << "Secuencia leída: " << secuencia << endl;
    return secuencia;
}

// Leer la matriz de emparejamiento
void leerMatrizEmparejamiento(const string &archivo) {
    ifstream archivoEntrada(archivo);
    if (!archivoEntrada.is_open()) {
        cerr << "Error al abrir el archivo: " << archivo << endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            archivoEntrada >> matrizEmparejamiento[i][j];
        }
    }
    archivoEntrada.close();
}

// Algoritmo de Needleman-Wunsch
void needlemanWunsch(const string &S, const string &T, int V) {
    int n = S.size();
    int m = T.size();

    // Usando solo dos filas para ahorrar memoria
    //revisar bien
    //consultar por pauta
    int f[2][MAXM + 1] = {0};

    // Inicializar la primera fila y columna
    for (int j = 0; j <= m; ++j) f[0][j] = j * V;

    // Llenar la matriz
    for (int i = 1; i <= n; ++i) {
        f[1][0] = i * V; // Primera columna de la fila actual
        for (int j = 1; j <= m; ++j) {
            int match = f[0][j - 1] + matrizEmparejamiento[indiceCaracter(S[i - 1])][indiceCaracter(T[j - 1])];
            int deleteGap = f[0][j] + V;
            int insertGap = f[1][j - 1] + V;

            f[1][j] = max({match, deleteGap, insertGap});
        }

        // Copiar la fila actual a la fila anterior
        for (int j = 0; j <= m; ++j) {
            f[0][j] = f[1][j];
        }
    }

    // Imprimir la matriz de puntajes correctamente
    cout << "Matriz de puntajes:\n"; //Parcial 
    for (int i = 0; i <= min(n, 30); ++i) {
        for (int j = 0; j <= min(m, 30); ++j) {
            cout << f[i % 2][j] << " ";
        }
        cout << endl;
    }

    // Puntaje máximo
    cout << "----------------------------------------------------------------------------------------\n";
    cout << "Puntaje maximo: " << f[0][m] << endl;
    cout << "----------------------------------------------------------------------------------------\n";

    // Reconstrucción del alineamiento
    string alineamientoS = "", alineamientoT = "";
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
        } else if (i > 0 && f[i % 2][j] == f[(i - 1) % 2][j] + V) {
            alineamientoS = S[i - 1] + alineamientoS;
            alineamientoT = '-' + alineamientoT;
            i--;
        } else if (j > 0 && f[i % 2][j] == f[i % 2][j - 1] + V) {
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

    // Mostrar el alineamiento (parcial)
    cout << "Alineamiento de S: " << alineamientoS.substr(0, 1000) << endl;
    cout << "Alineamiento de T: " << alineamientoT.substr(0, 1000) << endl;
    cout << " " << endl;

}

int main(int argc, char *argv[]) {
    cout << "Iniciando programa..." << endl;

    if (argc != 9) {
        cerr << "Uso: ./proyecto -C1 CYTB.txt -C2 CYTB2.txt -U funU.txt -V 1" << endl;
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
            cerr << "Parasmetro desconocido: " << flag << endl;
            return EXIT_FAILURE;
        }
    }

    // Probando que los parámetros de entrada funcionen bien
    /*cout << "Archivos procesados:" << endl;
    cout << "Secuencia 1: " << archivoS << endl;
    cout << "Secuencia 2: " << archivoT << endl;
    cout << "Matriz: " << archivoU << endl;
    cout << "Valor de hueco: " << V << endl;*/

    string S = leerSecuencia(archivoS);
    string T = leerSecuencia(archivoT);
    leerMatrizEmparejamiento(archivoU);

    cout << "Needleman-Wunsh..." << endl;
    needlemanWunsch(S, T, V);

    cout << "Programa finalizado." << endl;
    return 0;
}
