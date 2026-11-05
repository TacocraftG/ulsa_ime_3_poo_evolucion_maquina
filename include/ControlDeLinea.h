#ifndef CONTROLDELINEA_H
#define CONTROLDELINEA_H

#include "Maquina.h"

#include <vector>

class ControlDeLinea
{
private:
    std::vector<Maquina*> maquinas;

public:
    bool agregar(Maquina* maquina);
    void ejecutarTurno(int ciclos);
    void imprimirReporte() const;
};

#endif
