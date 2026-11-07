#include "ControlDeLinea.h"

#include <iostream>


namespace planta
{
bool ControlDeLinea::agregar(Maquina* maquina)
{
    if (maquina == nullptr)
    {
        return false;
    }

    maquinas.push_back(maquina);
    return true;
}

void ControlDeLinea::ejecutarTurno(int ciclos)
{
    for (Maquina* maquina : maquinas)
    {
        for (int i = 0; i < ciclos; i++)
        {
            maquina->procesar();
        }
    }
}

void ControlDeLinea::imprimirReporte() const
{
    std::cout << "--- Reporte de la linea ---\n";
    if (maquinas.empty())
    {
        std::cout << "La linea no tiene maquinas\n";
        return;
    }

    int totalPiezas = 0;
    for (const Maquina* maquina : maquinas)
    {
        maquina->mostrarEstado();
        totalPiezas += maquina->getPiezasProcesadas();
    }

    std::cout << "Total de piezas: " << totalPiezas << '\n';
}

}  // namespace planta
