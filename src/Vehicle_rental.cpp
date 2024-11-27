#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <functional>
#include <algorithm>

using namespace std;

class Vehiculo {
public:
    string modelo, marca, placa, color, motor, ced_cliente, fecha_de_entrega;
    int año, kilometraje, rentado;
    double precio_renta;

    static Vehiculo parse(const string& linea) {
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

    string toCSV() const {
        stringstream ss;
        ss << modelo << "," << marca << "," << placa << "," << color << "," << año << "," << kilometraje << "," << rentado << "," << motor << "," << precio_renta << "," << ced_cliente << "," << fecha_de_entrega;
        return ss.str();
    }
};

class Cliente {
public:
    string cedula, nombre, apellido, email, direccion;
    int cantidad_vehiculos_rentados, activo;

    static Cliente parse(const string& linea) {
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

    string toCSV() const {
        stringstream ss;
        ss << cedula << "," << nombre << "," << apellido << "," << email << "," << cantidad_vehiculos_rentados << "," << direccion << "," << activo;
        return ss.str();
    }
};

class Repuesto {
public:
    string modelo, marca, nombre, modelo_carro;
    int año_carro, existencias;
    double precio;

    static Repuesto parse(const string& linea) {
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

    string toCSV() const {
        stringstream ss;
        ss << modelo << "," << marca << "," << nombre << "," << modelo_carro << "," << año_carro << "," << precio << "," << existencias;
        return ss.str();
    }
};

template<typename T>
T* leerCSV(const string& nombreArchivo, T (*parseFunc)(const string&), int& cantidad) {
    ifstream archivo("bin/" + nombreArchivo);
    string linea;
    cantidad = 0;

    if (archivo.is_open()) {
        getline(archivo, linea);

        while (getline(archivo, linea)) {
            cantidad++;
        }

        archivo.clear();
        archivo.seekg(0, ios::beg);
        getline(archivo, linea);

        T* datos = new T[cantidad];
        int index = 0;
        while (getline(archivo, linea)) {
            datos[index++] = parseFunc(linea);
        }
        archivo.close();
        return datos;
    } else {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return nullptr;
    }
}

template<typename T>
void escribirCSV(const string& nombreArchivo, T* datos, int cantidad, string (T::*toCSV)() const) {
    ofstream archivo("bin/" + nombreArchivo);

    if (archivo.is_open()) {
        archivo << "modelo,marca,placa,color,año,kilometraje,rentado,motor,precio_renta,ced_cliente,fecha_de_entrega" << endl;

        for (int i = 0; i < cantidad; ++i) {
            archivo << (datos[i].*toCSV)() << endl;
        }
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
    }
}

template<typename T>
void borrarRegistro(T*& datos, int& cantidad, function<bool(const T&)> criterio) {
    T* nuevosDatos = new T[cantidad];
    int nuevoCantidad = 0;

    for (int i = 0; i < cantidad; ++i) {
        if (!criterio(datos[i])) {
            nuevosDatos[nuevoCantidad++] = datos[i];
        }
    }

    delete[] datos;
    datos = nuevosDatos;
    cantidad = nuevoCantidad;
}

template<typename T>
void actualizarRegistro(T* datos, int cantidad, function<bool(const T&)> criterio, function<void(T&)> actualizar) {
    for (int i = 0; i < cantidad; ++i) {
        if (criterio(datos[i])) {
            actualizar(datos[i]);
        }
    }
}

template<typename T>
void insertarRegistro(T*& datos, int& cantidad, const T& nuevoDato) {
    T* nuevosDatos = new T[cantidad + 1];
    for (int i = 0; i < cantidad; ++i) {
        nuevosDatos[i] = datos[i];
    }
    nuevosDatos[cantidad] = nuevoDato;
    delete[] datos;
    datos = nuevosDatos;
    cantidad++;
}

template<typename T>
void mostrarRegistro(const T& dato, string (T::*toCSV)() const) {
    cout << (dato.*toCSV)() << endl;
}

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
    int cantidadVehiculos, cantidadClientes, cantidadRepuestos;
    Vehiculo* vehiculos = leerCSV("datos_vehiculos.csv", Vehiculo::parse, cantidadVehiculos);
    Cliente* clientes = leerCSV("datos_clientes.csv", Cliente::parse, cantidadClientes);
    Repuesto* repuestos = leerCSV("datos_repuestos.csv", Repuesto::parse, cantidadRepuestos);

    bool cambiosConfirmados = false;
    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                for (int i = 0; i < cantidadVehiculos; ++i) {
                    mostrarRegistro(vehiculos[i], &Vehiculo::toCSV);
                }
                for (int i = 0; i < cantidadClientes; ++i) {
                    mostrarRegistro(clientes[i], &Cliente::toCSV);
                }
                for (int i = 0; i < cantidadRepuestos; ++i) {
                    mostrarRegistro(repuestos[i], &Repuesto::toCSV);
                }
                break;
            case 2: {
                string placa;
                cout << "Ingrese la placa del vehículo a borrar: ";
                getline(cin, placa);
                auto criterioBorrar = [placa](const Vehiculo& v) { return v.placa == placa; };
                borrarRegistro(vehiculos, cantidadVehiculos, function<bool(const Vehiculo&)>(criterioBorrar));
                cambiosConfirmados = false;
                break;
            }
            case 3: {
                string placa;
                cout << "Ingrese la placa del vehículo a actualizar: ";
                getline(cin, placa);
                auto criterioActualizar = [placa](const Vehiculo& v) { return v.placa == placa; };
                auto actualizar = [](Vehiculo& v) {
                    cout << "Ingrese el nuevo color: ";
                    getline(cin, v.color);
                };
                actualizarRegistro(vehiculos, cantidadVehiculos, function<bool(const Vehiculo&)>(criterioActualizar), function<void(Vehiculo&)>(actualizar));
                cambiosConfirmados = false;
                break;
            }
            case 4: {
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
                insertarRegistro(vehiculos, cantidadVehiculos, nuevoVehiculo);
                cambiosConfirmados = false;
                break;
            }
            case 5: {
                string placa;
                cout << "Ingrese la placa del vehículo a consultar: ";
                getline(cin, placa);
                auto criterioConsultar = [placa](const Vehiculo& v) { return v.placa == placa; };
                for (int i = 0; i < cantidadVehiculos; ++i) {
                    if (criterioConsultar(vehiculos[i])) {
                        mostrarRegistro(vehiculos[i], &Vehiculo::toCSV);
                    }
                }
                break;
            }
            case 6:
                escribirCSV("datos_vehiculos.csv", vehiculos, cantidadVehiculos, &Vehiculo::toCSV);
                escribirCSV("datos_clientes.csv", clientes, cantidadClientes, &Cliente::toCSV);
                escribirCSV("datos_repuestos.csv", repuestos, cantidadRepuestos, &Repuesto::toCSV);
                cout << "Cambios confirmados y guardados." << endl;
                cambiosConfirmados = true;
                break;
            case 7:
                cout << "Saliendo sin confirmar cambios." << endl;
                cambiosConfirmados = false;
                break;
            case 8:
                if (!cambiosConfirmados) {
                    cout << "Tiene cambios sin confirmar. ¿Desea salir sin guardar? (s/n): ";
                    char respuesta;
                    cin >> respuesta;
                    if (respuesta == 's' || respuesta == 'S') {
                        cout << "Saliendo sin confirmar cambios." << endl;
                        delete[] vehiculos;
                        delete[] clientes;
                        delete[] repuestos;
                        return 0;
                    } else {
                        opcion = 0;
                    }
                } else {
                    cout << "Saliendo del programa." << endl;
                    delete[] vehiculos;
                    delete[] clientes;
                    delete[] repuestos;
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