#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

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
 línea CSV
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

template<typename T>
vector<T> leerCSV(const string& nombreArchivo, T (*parseFunc)(const string&)) {
    vector<T> datos;
    ifstream archivo(nombreArchivo);
    string linea;

    if (archivo.is_open()) {
        getline(archivo, linea);

        while (getline(archivo, linea)) {
            datos.push_back(parseFunc(linea));
        }
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
    }

    return datos;
}

template<typename T>
void escribirCSV(const string& nombreArchivo, const vector<T>& datos, string (*toCSV)(const T&)) {
    ofstream archivo(nombreArchivo);

    if (archivo.is_open()) {
        archivo << "modelo,marca,placa,color,año,kilometraje,rentado,motor,precio_renta,ced_cliente,fecha_de_entrega" << endl;

        for (const auto& dato : datos) {
            archivo << toCSV(dato) << endl;
        }
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
    }
}

template<typename T>
void borrarRegistro(vector<T>& datos, function<bool(const T&)> criterio) {
    datos.erase(remove_if(datos.begin(), datos.end(), criterio), datos.end());
}

template<typename T>
void actualizarRegistro(vector<T>& datos, function<bool(const T&)> criterio, function<void(T&)> actualizar) {
    for (auto& dato : datos) {
        if (criterio(dato)) {
            actualizar(dato);
        }
    }
}

template<typename T>
void insertarRegistro(vector<T>& datos, const T& nuevoDato) {
    datos.push_back(nuevoDato);
}

template<typename T>
void mostrarRegistro(const T& dato, string (*toCSV)(const T&)) {
    cout << toCSV(dato) << endl;
}
void agregarCliente(vector<Cliente>& clientes, const Cliente& cliente) {
    clientes.push_back(cliente);
}

void actualizarCliente(vector<Cliente>& clientes, const string& cedula, const Cliente& clienteActualizado) {
    for (auto& cliente : clientes) {
        if (cliente.cedula == cedula) {
            cliente = clienteActualizado;
            return;
        }
    }
    cout << "Cliente no encontrado." << endl;
}

void borrarCliente(vector<Cliente>& clientes, const string& cedula) {
    clientes.erase(remove_if(clientes.begin(), clientes.end(), [&](const Cliente& cliente) {
        return cliente.cedula == cedula;
    }), clientes.end());
}

void consultarCliente(const vector<Cliente>& clientes, const string& cedula) {
    for (const auto& cliente : clientes) {
        if (cliente.cedula == cedula) {
            cout << "Cedula: " << cliente.cedula << endl;
            cout << "Nombre: " << cliente.nombre << endl;
            cout << "Apellido: " << cliente.apellido << endl;
            cout << "Email: " << cliente.email << endl;
            cout << "Direccion: " << cliente.direccion << endl;
            cout << "Cantidad de vehiculos rentados: " << cliente.cantidad_vehiculos_rentados << endl;
            cout << "Activo: " << cliente.activo << endl;
            return;
        }
    }
    cout << "Cliente no encontrado." << endl;
}

void agregarRepuesto(vector<Repuesto>& repuestos, const Repuesto& repuesto) {
    repuestos.push_back(repuesto);
}

void actualizarRepuesto(vector<Repuesto>& repuestos, const string& modelo, const string& marca, const string& nombre, const string& modelo_carro, int nuevasExistencias) {
    for (auto& repuesto : repuestos) {
        if (repuesto.modelo == modelo && repuesto.marca == marca && repuesto.nombre == nombre && repuesto.modelo_carro == modelo_carro) {
            repuesto.existencias = nuevasExistencias;
            return;
        }
    }
    cout << "Repuesto no encontrado." << endl;
}

void borrarRepuesto(vector<Repuesto>& repuestos, const string& modelo) {
    repuestos.erase(remove_if(repuestos.begin(), repuestos.end(), [&](const Repuesto& repuesto) {
        return repuesto.modelo == modelo;
    }), repuestos.end());
}

void consultarRepuesto(const vector<Repuesto>& repuestos, const string& modelo_carro, const string& nombre_repuesto) {
    bool encontrado = false;
    for (const auto& repuesto : repuestos) {
        if (repuesto.modelo_carro == modelo_carro && repuesto.nombre == nombre_repuesto) {
            cout << "Modelo: " << repuesto.modelo << endl;
            cout << "Marca: " << repuesto.marca << endl;
            cout << "Nombre: " << repuesto.nombre << endl;
            cout << "Modelo del carro: " << repuesto.modelo_carro << endl;
            cout << "Año del carro: " << repuesto.año_carro << endl;
            cout << "Existencias: " << repuesto.existencias << endl;
            cout << "Precio: " << repuesto.precio << endl;
            encontrado = true;
        }
    }
    if (!encontrado) {
        cout << "Repuesto no encontrado." << endl;
    }
}
void mostrarMenu() {
    cout << "Seleccione una operación:" << endl;
    cout << "1. Leer datos" << endl;
    cout << "2. Agregar datos" << endl;
    cout << "3. Actualizar datos" << endl;
    cout << "4. Borrar datos" << endl;
    cout << "5. Consultar datos" << endl;
    cout << "6. Confirmar cambios" << endl;
    cout << "7. Salir sin confirmar" << endl;
    cout << "8. Salir del programa" << endl;
    cout << "Opción: ";
}
void mostrarSubMenu() {
    cout << "Seleccione una opción:" << endl;
    cout << "1. Vehículos" << endl;
    cout << "2. Clientes" << endl;
    cout << "3. Repuestos" << endl;
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
        cin.ignore();

        switch (opcion) {
            case 1:

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

                mostrarSubMenu();
                int subOpcion;
                cin >> subOpcion;
                cin.ignore();
                switch (subOpcion) {
                    case 1: {

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
                    case 2: {

                        Cliente nuevoCliente;
                        cout << "Ingrese cedula: ";
                        getline(cin, nuevoCliente.cedula);
                        cout << "Ingrese nombre: ";
                        getline(cin, nuevoCliente.nombre);
                        cout << "Ingrese apellido: ";
                        getline(cin, nuevoCliente.apellido);
                        cout << "Ingrese email: ";
                        getline(cin, nuevoCliente.email);
                        cout << "Ingrese direccion: ";
                        getline(cin, nuevoCliente.direccion);
                        cout << "Ingrese cantidad de vehiculos rentados: ";
                        cin >> nuevoCliente.cantidad_vehiculos_rentados;
                        cout << "Ingrese activo (1 para activo, 0 para inactivo): ";
                        cin >> nuevoCliente.activo;
                        cin.ignore(); // Ignorar el salto de línea después de la entrada numérica
                        agregarCliente(clientes, nuevoCliente);
                        cambiosConfirmados = false;
                        break;
                    }
                    case 3: {

                        Repuesto nuevoRepuesto;
                        cout << "Ingrese modelo: ";
                        getline(cin, nuevoRepuesto.modelo);
                        cout << "Ingrese marca: ";
                        getline(cin, nuevoRepuesto.marca);
                        cout << "Ingrese nombre: ";
                        getline(cin, nuevoRepuesto.nombre);
                        cout << "Ingrese modelo del carro: ";
                        getline(cin, nuevoRepuesto.modelo_carro);
                        cout << "Ingrese año del carro: ";
                        cin >> nuevoRepuesto.año_carro; cin.ignore();
                        cout << "Ingrese existencias: ";
                        cin >> nuevoRepuesto.existencias; cin.ignore();
                        cout << "Ingrese precio: ";
                        cin >> nuevoRepuesto.precio; cin.ignore();
                        agregarRepuesto(repuestos, nuevoRepuesto);
                        cambiosConfirmados = false;
                        break;
                    }
                    default:
                        cout << "Opción no válida. Intente de nuevo." << endl;
                        break;
                }
                break;
            }
            case 3: {

                mostrarSubMenu();
                int subOpcion;
                cin >> subOpcion;
                cin.ignore();
                switch (subOpcion) {
                    case 1: {

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
                    case 2: {

                        string cedula;
                        cout << "Ingrese cedula del cliente a actualizar: ";
                        getline(cin, cedula);
                        Cliente clienteActualizado;
                        cout << "Ingrese nuevo nombre: ";
                        getline(cin, clienteActualizado.nombre);
                        cout << "Ingrese nuevo apellido: ";
                        getline(cin, clienteActualizado.apellido);
                        cout << "Ingrese nuevo email: ";
                        getline(cin, clienteActualizado.email);
                        cout << "Ingrese nueva direccion: ";
                        getline(cin, clienteActualizado.direccion);
                        cout << "Ingrese nueva cantidad de vehiculos rentados: ";
                        cin >> clienteActualizado.cantidad_vehiculos_rentados;
                        cout << "Ingrese nuevo estado activo (1 para activo, 0 para inactivo): ";
                        cin >> clienteActualizado.activo;
                        cin.ignore();
                        actualizarCliente(clientes, cedula, clienteActualizado);
                        cambiosConfirmados = false;
                        break;
                    }
                    case 3: {

                        string modelo, marca, nombre, modelo_carro;
                        int nuevasExistencias;
                        cout << "Ingrese modelo del repuesto a actualizar: ";
                        getline(cin, modelo);
                        cout << "Ingrese marca del repuesto a actualizar: ";
                        getline(cin, marca);
                        cout << "Ingrese nombre del repuesto a actualizar: ";
                        getline(cin, nombre);
                        cout << "Ingrese modelo del carro del repuesto a actualizar: ";
                        getline(cin, modelo_carro);
                        cout << "Ingrese nuevas existencias: ";
                        cin >> nuevasExistencias; cin.ignore();
                        actualizarRepuesto(repuestos, modelo, marca, nombre, modelo_carro, nuevasExistencias);
                        cambiosConfirmados = false;
                        break;
                    }
                    default:
                        cout << "Opción no válida. Intente de nuevo." << endl;
                        break;
                }
                break;
            }
            case 4: {

                mostrarSubMenu();
                int subOpcion;
                cin >> subOpcion;
                cin.ignore();
                switch (subOpcion) {
                    case 1: {

                        string placa;
                        cout << "Ingrese la placa del vehículo a borrar: ";
                        getline(cin, placa);
                        auto criterioBorrar = [placa](const Vehiculo& v) { return v.placa == placa; };
                        borrarRegistro(vehiculos, function<bool(const Vehiculo&)>(criterioBorrar));
                        cambiosConfirmados = false;
                        break;
                    }
                    case 2: {

                        string cedula;
                        cout << "Ingrese cedula del cliente a borrar: ";
                        getline(cin, cedula);
                        borrarCliente(clientes, cedula);
                        cambiosConfirmados = false;
                        break;
                    }
                    case 3: {

                        string modelo;
                        cout << "Ingrese modelo del repuesto a borrar: ";
                        getline(cin, modelo);
                        borrarRepuesto(repuestos, modelo);
                        cambiosConfirmados = false;
                        break;
                    }
                    default:
                        cout << "Opción no válida. Intente de nuevo." << endl;
                        break;
                }
                break;
            }
            case 5: {

                mostrarSubMenu();
                int subOpcion;
                cin >> subOpcion;
                cin.ignore();
                switch (subOpcion) {
                    case 1: {

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
                    case 2: {
 
                        string cedula;
                        cout << "Ingrese cedula del cliente a consultar: ";
                        getline(cin, cedula);
                        consultarCliente(clientes, cedula);
                        break;
                    }
                    case 3: {

                        string modelo_carro, nombre_repuesto;
                        cout << "Ingrese modelo del carro: ";
                        getline(cin, modelo_carro);
                        cout << "Ingrese nombre del repuesto: ";
                        getline(cin, nombre_repuesto);
                        consultarRepuesto(repuestos, modelo_carro, nombre_repuesto);
                        break;
                    }
                    default:
                        cout << "Opción no válida. Intente de nuevo." << endl;
                        break;
                }
                break;
            }
            case 6:

                escribirCSV("../datos_vehiculos.csv", vehiculos, vehiculoToCSV);
                escribirCSV("../datos_clientes.csv", clientes, clienteToCSV);
                escribirCSV("../datos_repuestos.csv", repuestos, repuestoToCSV);
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
                        return 0;
                    } else {
                        opcion = 0;
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