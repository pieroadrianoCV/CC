#include <iostream>
#include <string>
#include <fstream>
#include <xlsxio_read.h>

using namespace std;

string obtenerTipoDato(xlsxioreadersheet hoja, int col_idx) {
    const XlsxIOCell* celda = xlsxioread_sheet_cell_by_index(hoja, col_idx);
    if (!celda) {
        return "desconocido";
        break;
    }

    switch (celda->celltype) {
        case CELLTYPE_STRING:
            return "texto";
        case CELLTYPE_INT:
            return "entero";
        case CELLTYPE_FLOAT:
            return "flotante";
        default:
            return "desconocido";
    }
}

int main() {
    

    const char* archivo_excel = "Titanic control 2024.xlsx";
    const char* archivo_txt = "datos.txt";

    xlsxioreader archivo = xlsxioread_open(archivo_excel);
    if (!archivo) {
        cerr << "Error al abrir el archivo Excel." << endl;
        return 1;
    }

    ofstream archivo_salida(archivo_txt);
    if (!archivo_salida.is_open()) {
        cerr << "Error al abrir el archivo de salida." << endl;
        xlsxioread_close(archivo);
        return 1;
    }

    xlsxioreadersheet hoja = xlsxioread_sheet_open(archivo, XLSXIOREAD_SKIP_EMPTY_ROWS);
    if (!hoja) {
        cerr << "Error al abrir la hoja del archivo Excel." << endl;
        xlsxioread_close(archivo);
        archivo_salida.close();
        return 1;
    }

    const int num_columnas = xlsxioread_sheet_cols(hoja);
    for (int col_idx = 0; col_idx < num_columnas; ++col_idx) {
        const char* celda = xlsxioread_sheet_cell_value_by_index(hoja, col_idx);
        if (!celda) {
            break;  
        }
        string tipo_dato = obtenerTipoDato(hoja, col_idx);
        if (col_idx > 0) {
            archivo_salida << "#";
        }
        archivo_salida << celda << "#" << tipo_dato;
    }

    xlsxioread_sheet_close(hoja);
    xlsxioread_close(archivo);
    archivo_salida.close();

    cout << "Archivo de texto creado exitosamente." << endl;
    return 0;
}



