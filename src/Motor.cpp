#include "Motor.h"

#include <algorithm>
#include <stdexcept>


namespace planta
{
Motor::Motor(double velocidadMaxRpm)
    : velocidadMaxRpm(velocidadMaxRpm), velocidadActualRpm(0.0)
{
    if (velocidadMaxRpm <= 0.0)
    {
        throw std::invalid_argument("La velocidad maxima debe ser mayor que cero");
    }
}

void Motor::fijarVelocidad(double rpm)
{
    velocidadActualRpm = std::clamp(rpm, 0.0, velocidadMaxRpm);
}

double Motor::getVelocidad() const
{
    return velocidadActualRpm;
}

}  // namespace planta
