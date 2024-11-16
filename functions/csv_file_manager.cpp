#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <functional>

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

// Función para escribir los datos en un archivo CSV
template<typename T>
void escribirCSV(const string& nombreArchivo, const T& dato, string (*toCSV)(const T&), bool append = true) {
    ofstream archivo;
    if (append) {
        archivo.open(nombreArchivo, ios::app);
    } else {
        archivo.open(nombreArchivo);
    }

    if (archivo.is_open()) {
        archivo << toCSV(dato) << endl;
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
    }
}

// Funciones para convertir cada estructura a una línea CSV
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

// Función para leer y procesar cada línea de un archivo CSV
template<typename T>
void leerYProcesarCSV(const string& nombreArchivo, void (*procesarLinea)(const T&)) {
    ifstream archivo(nombreArchivo);
    string linea;

    if (archivo.is_open()) {
        // Leer la primera línea (cabecera)
        getline(archivo, linea);

        // Leer cada línea del archivo
        while (getline(archivo, linea)) {
            T dato = parseFunc(linea);
            procesarLinea(dato);
        }
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
    }
}

// Funciones para procesar cada tipo de estructura
void procesarLineaVehiculo(const string& linea) {
    Vehiculo v = parseVehiculo(linea);
    // Aquí puedes procesar el vehículo leído
    cout << "Vehiculo: " << v.modelo << ", " << v.marca << ", " << v.placa << endl;
}

void procesarLineaCliente(const string& linea) {
    Cliente c = parseCliente(linea);
    // Aquí puedes procesar el cliente leído
    cout << "Cliente: " << c.cedula << ", " << c.nombre << ", " << c.apellido << endl;
}

void procesarLineaRepuesto(const string& linea) {
    Repuesto r = parseRepuesto(linea);
    // Aquí puedes procesar el repuesto leído
    cout << "Repuesto: " << r.modelo << ", " << r.marca << ", " << r.nombre << endl;
}

// Función para borrar un registro de un archivo CSV
template<typename T>
void borrarRegistroCSV(const string& nombreArchivo, function<bool(const T&)> criterio, T (*parseFunc)(const string&), string (*toCSV)(const T&)) {
    ifstream archivo(nombreArchivo);
    ofstream temp("temp.csv");
    string linea;

    if (archivo.is_open() && temp.is_open()) {
        // Leer la primera línea (cabecera)
        getline(archivo, linea);
        temp << linea << endl;

        // Leer y copiar cada línea que no cumpla con el criterio de borrado
        while (getline(archivo, linea)) {
            T dato = parseFunc(linea);
            if (!criterio(dato)) {
                temp << toCSV(dato) << endl;
            }
        }
        archivo.close();
        temp.close();

        // Reemplazar el archivo original con el archivo temporal
        remove(nombreArchivo.c_str());
        rename("temp.csv", nombreArchivo.c_str());
    } else {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
    }
}

// Función para actualizar un registro en un archivo CSV
template<typename T>
void actualizacionregistroCSV(const string& nombreArchivo, bool (*criterio)(const T&), void (*actualizar)(T&), T (*parseFunc)(const string&), string (*toCSV)(const T&)) {
    ifstream archivo(nombreArchivo);
    ofstream temp("temp.csv");
    string linea;

    if (archivo.is_open() && temp.is_open()) {
        // Leer la primera línea (cabecera)
        getline(archivo, linea);
        temp << linea << endl;

        // Leer y actualizar cada línea que cumpla con el criterio de actualización
        while (getline(archivo, linea)) {
            T dato = parseFunc(linea);
            if (criterio(dato)) {
                actualizar(dato);
            }
            temp << toCSV(dato) << endl;
        }
        archivo.close();
        temp.close();

        // Reemplazar el archivo original con el archivo temporal
        remove(nombreArchivo.c_str());
        rename("temp.csv", nombreArchivo.c_str());
    } else {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
    }
}

// Ejemplo de criterios y funciones de actualización
bool criterioBorrarVehiculo(const Vehiculo& v) {
    return v.placa == "ABC123";
}

void actualizarVehiculo(Vehiculo& v) {
    if (v.placa == "XYZ789") {
        v.color = "Rojo";
    }
}

void mostrarMenu() {
    cout << "Seleccione una operación:" << endl;
    cout << "1. Leer archivos" << endl;
    cout << "2. imprimir registro de usuarios" << endl;
    cout << "3. imprimir registro de vehiculos" << endl;
    cout << "4. imprimir registro de repuestos" << endl;
    cout << "2. Borrar un registro" << endl;
    cout << "3. Actualizar un registro" << endl;
    cout << "4. Insertar un registro" << endl;
    cout << "5. Salir" << endl;
    cout << "Opción: ";
}

int main() {
    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;
        cin.ignore(); // Limpiar el buffer de entrada

        switch (opcion) {
            case 1:
                // Leer y procesar los archivos CSV
                leerYProcesarCSV("vehiculos.csv", procesarLineaVehiculo);
                leerYProcesarCSV("clientes.csv", procesarLineaCliente);
                leerYProcesarCSV("repuestos.csv", procesarLineaRepuesto);
                break;
            case 2: {
                cout << "Seleccione una opci'on:" << endl;
                cout << "1. Borrar un registro de clientes" << endl << "2. Borrar un registro de vehiculos" << endl << "3. Borrar un registro de repuestos" << endl;
                cin >> opcion;
                switch (opcion)
                {
                case 1:
                    borrarRegistroCSV<Cliente>("clientes.csv", criterioBorrarCliente, parseCliente, clienteToCSV);
                    break;
                case 2:
                    borrarRegistroCSV<Vehiculo>("vehiculos.csv", criterioBorrarVehiculo, parseVehiculo, vehiculoToCSV);
                    break;
                case 3:
                    borrarRegistroCSV<Repuesto>("repuestos.csv", criterioBorrarRepuesto, parseRepuesto, repuestoToCSV);
                    break;
                default:
                    cout << "Opción no válida. Intente de nuevo." << endl;
                    break;
                }
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
                actualizacionregistroCSV<Vehiculo>("vehiculos.csv", criterioActualizar, actualizar, parseVehiculo, vehiculoToCSV);
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
                escribirCSV("vehiculos.csv", nuevoVehiculo, vehiculoToCSV);
                break;
            }
            case 5:
                
                break;
            default:
                cout << "Opción no válida. Intente de nuevo." << endl;
                break;
        }
    } while (opcion != 5);

    return 0;
}