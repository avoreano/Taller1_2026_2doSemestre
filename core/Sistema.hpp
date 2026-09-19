#ifndef SISTEMA_HPP
#define SISTEMA_HPP
#include <string>

class Sistema {
private:
    void cargarPacientesDesdeArchivo(const std::string& rutaArchivo);
    void mostrarColaPendientes() const;
    void atenderPacientes();
    void verDepartamento() const;
    void mostrarHistorialAtenciones() const;

public:
    Sistema();
    ~Sistema();
    void ejecutar();
};

#endif
