#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

// Estructuras para almacenar los datos
struct Vehiculo {
    string modelo, marca, placa, color, motor, ced_cliente, fecha_de_entrega;
    int año, kilometraje, rentado;
    double precio_renta;
};

struct Cliente {
    string cedula, nombre, apellido, email, direccion;
    int cantidad_vehiculos_rentados, activo;
};

struct Repuesto {
    string modelo, marca, nombre, modelo_carro;
    int año_carro, existencias;
    double precio;
};

// Funciones para parsear cada tipo de estructura
Vehiculo parseVehiculo(const string& linea) {
    Vehiculo v;
    stringstream ss(linea);
    string campo;

    getline(ss, v.modelo, ',');
    getline(ss, v.marca, ',');
    getline(ss, v.placa, ',');
    getline(ss, v.color, ',');
    ss >> v.año; ss.ignore();
    ss >> v.kilometraje; ss.ignore();
    ss >> v.rentado; ss.ignore();
    getline(ss, v.motor, ',');
    ss >> v.precio_renta; ss.ignore();
    getline(ss, v.ced_cliente, ',');
    getline(ss, v.fecha_de_entrega, ',');

    return v;
}

Cliente parseCliente(const string& linea) {
    Cliente c;
    stringstream ss(linea);
    string campo;

    getline(ss, c.cedula, ',');
    getline(ss, c.nombre, ',');
    getline(ss, c.apellido, ',');
    getline(ss, c.email, ',');
    ss >> c.cantidad_vehiculos_rentados; ss.ignore();
    getline(ss, c.direccion, ',');
    ss >> c.activo;

    return c;
}

Repuesto parseRepuesto(const string& linea) {
    Repuesto r;
    stringstream ss(linea);
    string campo;

    getline(ss, r.modelo, ',');
    getline(ss, r.marca, ',');
    getline(ss, r.nombre, ',');
    getline(ss, r.modelo_carro, ',');
    ss >> r.año_carro; ss.ignore();
    ss >> r.precio; ss.ignore();
    ss >> r.existencias;

    return r;
}

// Función para convertir cada estructura a una línea CSV
string vehiculoToCSV(const Vehiculo& v) {
    stringstream ss;
    ss << v.modelo << "," << v.marca << "," << v.placa << "," << v.color << "," << v.año << "," << v.kilometraje << "," << v.rentado << "," << v.motor << "," << v.precio_renta << "," << v.ced_cliente << "," << v.fecha_de_entrega;
    return ss.str();
}

string clienteToCSV(const Cliente& c) {
    stringstream ss;
    ss << c.cedula << "," << c.nombre << "," << c.apellido << "," << c.email << "," << c.cantidad_vehiculos_rentados << "," << c.direccion << "," << c.activo;
    return ss.str();
}

string repuestoToCSV(const Repuesto& r) {
    stringstream ss;
    ss << r.modelo << "," << r.marca << "," << r.nombre << "," << r.modelo_carro << "," << r.año_carro << "," << r.precio << "," << r.existencias;
    return ss.str();
}

// Función para leer un archivo CSV y almacenar los datos en un vector de estructuras
template<typename T>
vector<T> leerCSV(const string& nombreArchivo, T (*parseFunc)(const string&)) {
    vector<T> datos;
    ifstream archivo("bin/" + nombreArchivo);
    string linea;

    if (archivo.is_open()) {
        // Leer la primera línea (cabecera)
        getline(archivo, linea);

        // Leer cada línea del archivo
        while (getline(archivo, linea)) {
            datos.push_back(parseFunc(linea));
        }
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
    }

    return datos;
}

// Función para escribir los datos en un archivo CSV
template<typename T>
void escribirCSV(const string& nombreArchivo, const vector<T>& datos, string (*toCSV)(const T&)) {
    ofstream archivo("bin/" + nombreArchivo);

    if (archivo.is_open()) {
        // Escribir la cabecera
        archivo << "modelo,marca,placa,color,año,kilometraje,rentado,motor,precio_renta,ced_cliente,fecha_de_entrega" << endl;

        // Escribir cada dato
        for (const auto& dato : datos) {
            archivo << toCSV(dato) << endl;
        }
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
    }
}

// Función para borrar un registro de un vector
template<typename T>
void borrarRegistro(vector<T>& datos, function<bool(const T&)> criterio) {
    datos.erase(remove_if(datos.begin(), datos.end(), criterio), datos.end());
}

// Función para actualizar un registro en un vector
template<typename T>
void actualizarRegistro(vector<T>& datos, function<bool(const T&)> criterio, function<void(T&)> actualizar) {
    for (auto& dato : datos) {
        if (criterio(dato)) {
            actualizar(dato);
        }
    }
}

// Función para insertar un registro en un vector
template<typename T>
void insertarRegistro(vector<T>& datos, const T& nuevoDato) {
    datos.push_back(nuevoDato);
}

// Función para mostrar un registro
template<typename T>
void mostrarRegistro(const T& dato, string (*toCSV)(const T&)) {
    cout << toCSV(dato) << endl;
}

// Función para mostrar el menú
void mostrarMenu() {
    cout << "Seleccione una operación:" << endl;
    cout << "1. Leer archivos" << endl;
    cout << "2. Borrar un registro" << endl;
    cout << "3. Actualizar un registro" << endl;
    cout << "4. Insertar un registro" << endl;
    cout << "5. Consultar un registro" << endl;
    cout << "6. Confirmar cambios" << endl;
    cout << "7. Salir sin confirmar" << endl;
    cout << "8. Salir del programa" << endl;
    cout << "Opción: ";
}

int main() {
    vector<Vehiculo> vehiculos = leerCSV("datos_vehiculos.csv", parseVehiculo);
    vector<Cliente> clientes = leerCSV("datos_clientes.csv", parseCliente);
    vector<Repuesto> repuestos = leerCSV("datos_repuestos.csv", parseRepuesto);

    bool cambiosConfirmados = false;
    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;
        cin.ignore(); // Limpiar el buffer de entrada

        switch (opcion) {
            case 1:
                // Leer y procesar los archivos CSV
                for (const auto& v : vehiculos) {
                    mostrarRegistro(v, vehiculoToCSV);
                }
                for (const auto& c : clientes) {
                    mostrarRegistro(c, clienteToCSV);
                }
                for (const auto& r : repuestos) {
                    mostrarRegistro(r, repuestoToCSV);
                }
                break;
            case 2: {
                // Borrar un registro
                string placa;
                cout << "Ingrese la placa del vehículo a borrar: ";
                getline(cin, placa);
                auto criterioBorrar = [placa](const Vehiculo& v) { return v.placa == placa; };
                borrarRegistro(vehiculos, function<bool(const Vehiculo&)>(criterioBorrar));
                cambiosConfirmados = false;
                break;
            }
            case 3: {
                // Actualizar un registro
                string placa;
                cout << "Ingrese la placa del vehículo a actualizar: ";
                getline(cin, placa);
                auto criterioActualizar = [placa](const Vehiculo& v) { return v.placa == placa; };
                auto actualizar = [](Vehiculo& v) {
                    cout << "Ingrese el nuevo color: ";
                    getline(cin, v.color);
                };
                actualizarRegistro(vehiculos, function<bool(const Vehiculo&)>(criterioActualizar), function<void(Vehiculo&)>(actualizar));
                cambiosConfirmados = false;
                break;
            }
            case 4: {
                // Insertar un registro
                Vehiculo nuevoVehiculo;
                cout << "Ingrese los datos del nuevo vehículo:" << endl;
                cout << "Modelo: "; getline(cin, nuevoVehiculo.modelo);
                cout << "Marca: "; getline(cin, nuevoVehiculo.marca);
                cout << "Placa: "; getline(cin, nuevoVehiculo.placa);
                cout << "Color: "; getline(cin, nuevoVehiculo.color);
                cout << "Año: "; cin >> nuevoVehiculo.año; cin.ignore();
                cout << "Kilometraje: "; cin >> nuevoVehiculo.kilometraje; cin.ignore();
                cout << "Rentado (0 o 1): "; cin >> nuevoVehiculo.rentado; cin.ignore();
                cout << "Motor: "; getline(cin, nuevoVehiculo.motor);
                cout << "Precio de renta: "; cin >> nuevoVehiculo.precio_renta; cin.ignore();
                cout << "Cédula del cliente: "; getline(cin, nuevoVehiculo.ced_cliente);
                cout << "Fecha de entrega: "; getline(cin, nuevoVehiculo.fecha_de_entrega);
                insertarRegistro(vehiculos, nuevoVehiculo);
                cambiosConfirmados = false;
                break;
            }
            case 5: {
                // Consultar un registro
                string placa;
                cout << "Ingrese la placa del vehículo a consultar: ";
                getline(cin, placa);
                auto criterioConsultar = [placa](const Vehiculo& v) { return v.placa == placa; };
                for (const auto& v : vehiculos) {
                    if (criterioConsultar(v)) {
                        mostrarRegistro(v, vehiculoToCSV);
                    }
                }
                break;
            }
            case 6:
                // Confirmar cambios
                escribirCSV("datos_vehiculos.csv", vehiculos, vehiculoToCSV);
                escribirCSV("datos_clientes.csv", clientes, clienteToCSV);
                escribirCSV("datos_repuestos.csv", repuestos, repuestoToCSV);
                cout << "Cambios confirmados y guardados." << endl;
                cambiosConfirmados = true;
                break;
            case 7:
                // Salir sin confirmar
                cout << "Saliendo sin confirmar cambios." << endl;
                cambiosConfirmados = false;
                break;
            case 8:
                // Salir del programa
                if (!cambiosConfirmados) {
                    cout << "Tiene cambios sin confirmar. ¿Desea salir sin guardar? (s/n): ";
                    char respuesta;
                    cin >> respuesta;
                    if (respuesta == 's' || respuesta == 'S') {
                        cout << "Saliendo sin confirmar cambios." << endl;
                        return 0;
                    } else {
                        opcion = 0; // Para continuar en el bucle
                    }
                } else {
                    cout << "Saliendo del programa." << endl;
                    return 0;
                }
                break;
            default:
                cout << "Opción no válida. Intente de nuevo." << endl;
                break;
        }
    } while (true);

    return 0;
}