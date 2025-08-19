#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <iomanip>
#include <chrono>
#include <sys/resource.h>
#include "generador.cpp"
#include "persona.h"
#include "generador_c.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[])
{
    srand(time(nullptr));
    size_t n = 100000;
    if (argc > 1)
    {
        try
        {
            long input = stol(argv[1]);
            if (input > 0)
            {
                n = static_cast<size_t>(input);
            }
            else
            {
                cerr << "Advertencia: El valor debe ser positivo. Usando valor por defecto.\n";
            }
        }
        catch (const invalid_argument &)
        {
            cerr << "Advertencia: Entrada no válida. Usando valor por defecto.\n";
        }
        catch (const out_of_range &)
        {
            cerr << "Advertencia: Valor demasiado grande. Usando valor por defecto.\n";
        }
    }

    cout << "Generando " << n << " personas...\n";
    vector<Persona> personas = generarColeccion(n);
    PersonaC *personas_c = generarColeccionC(n);

    vector<tuple<string, long, long>> results;

    // C++ Apuntadores
    cout << "Procesando C++ con apuntadores...\n";
    auto start = high_resolution_clock::now();
    process_data(personas, true);
    auto end = high_resolution_clock::now();
    long mem_cpp_ptr = get_peak_memory();
    results.emplace_back("C++ Apuntadores", duration_cast<milliseconds>(end - start).count(), mem_cpp_ptr);

    // C++ Valores
    cout << "Procesando C++ con valores...\n";
    start = high_resolution_clock::now();
    process_data(personas, false);
    end = high_resolution_clock::now();
    long mem_cpp_val = get_peak_memory();
    results.emplace_back("C++ Valores", duration_cast<milliseconds>(end - start).count(), mem_cpp_val);

    // C Apuntadores
    cout << "Procesando C con apuntadores...\n";
    start = high_resolution_clock::now();
    process_data_c(personas_c, n, 1);
    end = high_resolution_clock::now();
    long mem_c_ptr = get_peak_memory();
    results.emplace_back("C Apuntadores", duration_cast<milliseconds>(end - start).count(), mem_c_ptr);

    // C Valores
    cout << "Procesando C con valores...\n";
    start = high_resolution_clock::now();
    process_data_c(personas_c, n, 0);
    end = high_resolution_clock::now();
    long mem_c_val = get_peak_memory();
    results.emplace_back("C Valores", duration_cast<milliseconds>(end - start).count(), mem_c_val);

    // Liberar memoria de C
    freeColeccionC(personas_c);

    return 0;
}
