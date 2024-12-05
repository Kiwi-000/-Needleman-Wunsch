#include <iostream> 
#include <fstream> 
#include <string>
#include <algorithm> // max
#include <iomanip> // setw
#include <unordered_map>
#include <cstdlib> // system

using namespace std;

// Dimensiones máximas de las secuencias
const int MAXN = 1001;
const int MAXM = 1001;

// Matriz de emparejamiento/puntuaación
int matrizEmparejamiento[4][4];

//Caracteres de las secuencias
char alfabeto[4] = {'A', 'G', 'C', 'T'};


// Función para convertir un carácter en un índice (0, 1, 2, 3)--------------------------------------------------------------------------------------------------------------------------
int indiceCaracter(char c) {
    for (int i = 0; i < 4; i++) {
        if (alfabeto[i] == c){
            return i;
        }
    }
    return -1; //Si no esta dentro del alfabeto.
}


// Leer la secuencia de un archivo-------------------------------------------------------------------------------------------------------------------------------------------------------
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
    cout << "Secuencia leida: " << secuencia << endl;
    return secuencia;
}


// Leer la matriz de emparejamiento-----------------------------------------------------------------------------------------------------------------------------------------------------
void leerMatrizEmparejamiento(const string &archivo) {
    ifstream archivoEntrada(archivo);
    if (!archivoEntrada.is_open()) {
        cerr << "Error al abrir el archivo: " << archivo << endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            archivoEntrada >> matrizEmparejamiento[i][j]; //llena de los valores
        }
    }
    archivoEntrada.close();

    //Depuración
    cout << "Matriz de emparejamiento:" << endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << matrizEmparejamiento[i][j] << " ";
        }
        cout << endl;
    }
}


// Función para imprimir matriz de puntajes----------------------------------------------------------------------------------------------------------------------------------------------
// Limita tamaño mostrado para mantener orden visual
void imprimirMatrizPuntajes(int** matriz, int filas, int columnas, const string& secuencia1, const string& secuencia2, int limiteFilas = 30, int limiteColumnas = 30) {
    cout << "   ";
    cout << "     ";

    for (int j = 0; j <= min(columnas, limiteColumnas); ++j) {
        if (j == 0)
            cout << setw(3) << "-";
        else
            cout << setw(3) << secuencia2[j - 1];
    }

    if (columnas > limiteColumnas) {
        cout << " ...";
    }
    cout << endl;

    for (int i = 0; i <= min(filas, limiteFilas); ++i) {
        if (i == 0) {
            cout << setw(3) << "-";
        } else {
            cout << setw(3) << secuencia1[i - 1];
        }
        for (int j = 0; j <= min(columnas, limiteColumnas); ++j) {
            cout << setw(3) << matriz[i][j];
        }

        if (columnas > limiteColumnas) {
            cout << " ...";
        }
        cout << endl;
    }

    if (filas > limiteFilas) {
        cout << "...\n";
    }
}


//Función para calcular porcentaje de similitud de las secuencias------------------------------------------------------------------------------------------------------------------------
void calcularPorcentajeSimilitud(const string& alineamientoS, const string& alineamientoT) {
    if (alineamientoS.size() != alineamientoT.size()) {
        cerr << "Error: las secuencias alineadas deben tener el mismo tamaño.\n";
        return;
    }

    int coincidencias = 0;
    int longitud = alineamientoS.size();

    // Contar coincidencias
    for (size_t i = 0; i < alineamientoS.size(); ++i) {
        if (alineamientoS[i] == alineamientoT[i] && alineamientoS[i] != '-' && alineamientoT[i] != '-') {
            coincidencias++;
        }
    }

    // Calcular porcentaje
    double porcentajeSimilitud = (static_cast<double>(coincidencias) / longitud) * 100;

    // Mostrar resultado
    cout << "----------------------------------------------------------------------------------------\n";
    cout << "Porcentaje de similitud: " << fixed << setprecision(2) << porcentajeSimilitud << "%\n";
    cout << "----------------------------------------------------------------------------------------\n";
}


// Función para visualizar el alineamiento ----------------------------------------------------------------------------------------------------------------------------------------------
void visualizarAlineamiento(const string& alineamientoS, const string& alineamientoT) {
    auto obtenerColor = [](char c) -> string {
        unordered_map<char, string> mapaColores = {
            {'A', "pink"}, {'T', "cyan"}, {'C', "lime"}, {'G', "orange"}
        };
        return mapaColores.count(c) ? mapaColores[c] : "grey";
    };

    auto generarNodos = [&](ofstream& fp, const string& cadena, const string& prefijo) {
        for (size_t i = 0; i < cadena.size(); ++i) {
            fp << "    " << prefijo << i + 1 
               << " [label=\"" << cadena[i] 
               << "\", style=filled, fillcolor=" << obtenerColor(cadena[i]) 
               << ", fontcolor=black];\n";
        }
    };

    auto conectarNodos = [&](ofstream& fp, size_t longitud, const string& prefijo) {
        for (size_t i = 0; i < longitud; ++i) {
            fp << "    " << prefijo << i << " -> " << prefijo << i + 1 << ";\n";
        }
    };

    auto conectarAlineamiento = [&](ofstream& fp, const string& cadena1, const string& cadena2) {
        for (size_t i = 0; i < cadena1.size(); ++i) {
            if (cadena1[i] == '-' || cadena2[i] == '-') {
                fp << "    S" << i + 1 << " -> T" << i + 1 
                   << " [style=dashed, color=red];\n";
            } else if (cadena1[i] == cadena2[i]) {
                fp << "    S" << i + 1 << " -> T" << i + 1 
                   << " [style=solid, color=red];\n";
            } else {
                fp << "    S" << i + 1 << " -> T" << i + 1 
                   << " [style=solid, color=red];\n";
            }
        }
    };

    ofstream archivo("emparejamiento.txt");
    if (!archivo.is_open()) {
        cerr << "Error: no se pudo abrir el archivo para escritura.\n";
        return;
    }

    archivo << "digraph G {\n    rankdir=LR;\n";
    generarNodos(archivo, alineamientoS, "S");
    generarNodos(archivo, alineamientoT, "T");
    conectarNodos(archivo, alineamientoS.size(), "S");
    conectarNodos(archivo, alineamientoT.size(), "T");
    conectarAlineamiento(archivo, alineamientoS, alineamientoT);
    archivo << "}\n";
    archivo.close();

    system("dot -Tpng emparejamiento.txt -o emparejamiento.png");
}


// Algoritmo de Needleman-Wunsch---------------------------------------------------------------------------------------------------------------------------------------------------------
void needlemanWunsch(const string &S, const string &T, int V) {
    int n = S.size();
    int m = T.size();

    // Matriz de puntajes como un arreglo dinámico
    int **f = new int*[n + 1];
    for (int i = 0; i <= n; ++i) {
        f[i] = new int[m + 1];
    }

    // Llenar la matriz de puntajes 
    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            if (i == 0 && j == 0) {
                f[i][j] = 0;  
            } else if (i > 0 && j == 0) {
                f[i][j] = f[i - 1][j] + V;  
            } else if (i == 0 && j > 0) {
                f[i][j] = f[i][j - 1] + V;  
            } else if (i > 0 && j > 0) {
                f[i][j] = max({
                    f[i - 1][j] + V,  
                    f[i][j - 1] + V,  
                    f[i - 1][j - 1] + matrizEmparejamiento[indiceCaracter(S[i - 1])][indiceCaracter(T[j - 1])] // Emparejar
                });
            }
        }
    }

    imprimirMatrizPuntajes(f, n, m, S, T, 30, 30);

    // Puntaje máximo
    cout << "----------------------------------------------------------------------------------------\n";
    cout << "Puntaje maximo: " << f[n][m] << endl;
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

    // Mostrar el alineamiento
    cout << "Alineamiento de S: " << alineamientoS.substr(0, 1000) << endl;
    cout << "Alineamiento de T: " << alineamientoT.substr(0, 1000) << endl;
    
    visualizarAlineamiento(alineamientoS, alineamientoT);
    calcularPorcentajeSimilitud(alineamientoS, alineamientoT);

    // Liberar memoria d
    for (int i = 0; i <= n; ++i) {
        delete[] f[i];
    }
    delete[] f;

}


// Función principal---------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    cout << "Iniciando programa..." << endl;

    if (argc != 9) {
        cerr << "Uso: ./proyecto -C1 CYTB.txt -C2 CYTB2.txt -U funU.txt -V 1" << endl;
        return EXIT_FAILURE;
    }

    string archivoS, archivoT, archivoU;
    int V;

    //Relacionar argumentos con un valor
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

    //Entrega el archivo correspondiente a cada seguncia y matriz
    string S = leerSecuencia(archivoS);
    string T = leerSecuencia(archivoT);
    leerMatrizEmparejamiento(archivoU);

    //Depurar y llamar al algoritmo
    cout << "Needleman-Wunsh..." << endl;
    needlemanWunsch(S, T, V);

    cout << "Programa finalizado." << endl;
    return 0;
}
