#ifndef MAQUINA_H
#define MAQUINA_H

#include <string>

class Maquina
{
private:
    std::string nombre;
    int piezasProcesadas;
    int tiempoTrabajadoSeg;

protected:
    void registrarPieza();
    void agregarTiempo(int segundos);

public:
    explicit Maquina(const std::string& nombre);

    void procesar();

    const std::string& getNombre() const;
    int getPiezasProcesadas() const;
    int getTiempoTrabajadoSeg() const;
    void mostrarEstado() const;
};

#endif
