#ifndef MAQUINA_H
#define MAQUINA_H

#include <string>

// Clase base abstracta de todas las maquinas de la planta.
// Las derivadas implementan el trabajo fisico y el tiempo de ciclo; la base
// valida el tiempo y registra una pieza por cada ciclo procesado.
class Maquina
{
private:
    std::string nombre;
    int piezasProcesadas;
    int tiempoTrabajadoSeg;

    void registrarPieza();
    void agregarTiempo(int segundos);
    int tiempoCicloValidado() const;
    virtual void ejecutarCiclo() = 0;

public:
    explicit Maquina(const std::string& nombre);
    virtual ~Maquina() = default;

    void procesar();
    virtual int tiempoCicloSeg() const = 0;
    double capacidadPorHora() const;

    const std::string& getNombre() const;
    int getPiezasProcesadas() const;
    int getTiempoTrabajadoSeg() const;
    void mostrarEstado() const;
};

#endif
