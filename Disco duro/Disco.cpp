#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

string determinarTipoDato(const string& valor) {
    bool soloDigitos = true;
    bool puntoDecimal = false;

    for (char c : valor) {
        if (isdigit(c)) {
            continue;
        } else if (c == '.' && !puntoDecimal) {
            puntoDecimal = true;
        } else {
            soloDigitos = false;
            break;
        }
    }

    if (soloDigitos) {
        if (puntoDecimal) {
            return "float";
        } else {
            return "int";
        }
    } else {
        return "string";
    }
}

string toLowercase(string str) {
    transform(str.begin(), str.end(), str.begin(),
              [](unsigned char c) { return tolower(c); });
    return str;
}

int main() {
    const string archivo_csv = "Titanic control 2024.csv";
    const string archivo_esquemas = "Esquemas.txt";

    ofstream archivo_salida_esquemas(archivo_esquemas);
    if (!archivo_salida_esquemas.is_open()) {
        cerr << "Error al abrir el archivo de salida: " << archivo_esquemas << endl;
        return 1;
    }

    ifstream archivo_entrada(archivo_csv);
    if (!archivo_entrada.is_open()) {
        cerr << "Error al abrir el archivo CSV." << endl;
        archivo_salida_esquemas.close();
        return 1;
    }

    string linea;
    if (getline(archivo_entrada, linea)) {
        stringstream ss(linea);
        string valor;
        bool primer_elemento = true;
        while (getline(ss, valor, ';')) {
            if (!primer_elemento) {
                archivo_salida_esquemas << "#";
            }
            archivo_salida_esquemas << valor;
            primer_elemento = false;
        }
        archivo_salida_esquemas << endl;

        ss.clear();
        ss.str(linea);
        vector<string> etiquetas;
        while (getline(ss, valor, ';')) {
            etiquetas.push_back(valor);
        }

        archivo_salida_esquemas << "usuario#"; 
        for (const string& etiqueta : etiquetas) {
            if (etiqueta == "Id" || etiqueta == "Name" || etiqueta == "Sex" || etiqueta == "Age") {
                archivo_salida_esquemas << etiqueta << "#" << determinarTipoDato(etiqueta) << "#";
            }
        }
        archivo_salida_esquemas << endl;

        archivo_salida_esquemas << "Datos-extras#";
        for (const string& etiqueta : etiquetas) {
            if (etiqueta != "Id" && etiqueta != "Name" && etiqueta != "Sex" && etiqueta != "Age") {
                archivo_salida_esquemas << etiqueta << "#" << determinarTipoDato(etiqueta) << "#";
            }
        }
        archivo_salida_esquemas << endl;
    }

    archivo_entrada.close();
    archivo_salida_esquemas.close();

    ifstream archivo_esquema_entrada(archivo_esquemas);
    if (!archivo_esquema_entrada.is_open()) {
        cerr << "Error al abrir el archivo de esquemas." << endl;
        return 1;
    }

    vector<string> etiquetas_esquemas;
    if (getline(archivo_esquema_entrada, linea)) {
        stringstream ss(linea);
        string etiqueta;
        while (getline(ss, etiqueta, '#')) {
            etiquetas_esquemas.push_back(toLowercase(etiqueta)); 
        }
    }

    string nombre_txt;
    while (getline(archivo_esquema_entrada, linea)) {
        stringstream ss(linea);
        if (getline(ss, nombre_txt, '#')) {
            nombre_txt.erase(remove_if(nombre_txt.begin(), nombre_txt.end(), ::isspace), nombre_txt.end());
            nombre_txt = toLowercase(nombre_txt) + ".txt"; 

            ofstream archivo_salida(nombre_txt);
            if (!archivo_salida.is_open()) {
                cerr << "Error al abrir el archivo de salida: " << nombre_txt << endl;
                archivo_esquema_entrada.close();
                return 1;
            }

            archivo_salida.close();
        }
    }

    archivo_esquema_entrada.close();

    cout << "Archivos de texto creados exitosamente." << endl;
    return 0;
}
