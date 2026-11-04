#include "Fresadora.h"
#include <iostream>

Fresadora::Fresadora(const std::string& nombre) : Maquina(nombre){}

void Fresadora::procesar()
{
    std::cout << getNombre() << ": fresando pieza\n";
    registrarPieza();
    agregarTiempo(60);
}