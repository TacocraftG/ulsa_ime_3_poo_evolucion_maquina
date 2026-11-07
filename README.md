# Práctica Unidad IV: La evolución de una máquina

Programación Orientada a Objetos · Ingeniería Mecatrónica · Tercer semestre

Llena cada espacio conforme avances en las fases de [PRACTICA.md](PRACTICA.md).

## Fase 1. Entender el problema

**1.1 El problema con mis propias palabras**

Se necesita mejorar un sistema para poder controlar una linea de manufactura, en esta linea hay diferentes tipos de maquinas que son parte de un proceso. Existe la clase Maquina que funciona de forma muy superficial, el objetivo es implementar una actualizacion empleando los visto en clase.

**1.2 Reglas que siempre deben cumplirse en una máquina** (escribe al menos tres)

1. La cantidad de piezas que se procesan y el tiempo de proceso de cada maquina sera mayor o igual a cero.
2. El nombre de las maquinas debe ser una cadena de texto que no este vacia y que no contenga solo caracteres nulos.
3. El tiempo que tarda un ciclo debe ser mayor a cero para evitar divisiones entre cero.

**1.3 ¿Quién más usa la clase `Maquina`? ¿Qué podría salir mal si otro equipo crea una máquina nueva sin conocer tus reglas?**

La clase sera utlizada por otros desarroladores (mis compañeros), esta clase sera un punto de partida para heredar de esta y programar nuevas maquinas. Si un equipo nuevo de desarrolladores crearar una maquina nueva sin leer mis rerglas, podrian toparse con problemas que impidan el poder compilar, asi como cometer errores que hagan al sistema principal fallar.

## Fase 2. Diseñar la solución

**2.1 Diagrama inicial (Etapa 0)**

![Diagrama inicial](diseno_inicial.png)

**2.2 Diagrama final (Etapa 9)**

![Diagrama final](diseno_solucion.png)

**2.3 ¿Qué cambió entre los dos diagramas?**

| Elemento | Etapa 0 | Etapa 9 | Etapa que lo cambió |
| --- | --- | --- | --- |
| Visibilidad de los atributos de `Maquina` | publicos | privados | Etapa 1 |
| `procesar()` | Metodo publico no virtual | Metodo publico no virtual de la base que aplica el contrato | Etapa 7 |
| `tiempoCicloSeg()` | No existia | Virtual puro, implementado por cada maquina | Etapa 2 y Etapa 5 |
| Destructor de `Maquina` | No existia | Virtual predeterminado | Etapa 4 |
| ¿Se pueden crear objetos `Maquina`? | Si | No, es abstracta | Etapa 5 |
| Relación `ControlDeLinea` - `Maquina` | No existia `ControlDeLinea` | Agregacion mediante punteros | Etapa 3 |
| Relación `Torno` - `Motor` | No existia el motor | Composicion: `Torno` contiene un `Motor` | Etapa 4 |
| Relación `BandaTransportadora` - `Motor` | No existia la banda | Composicion: `BandaTransportadora` contiene un `Motor` | Etapa 8 |

## Fase 3. Implementar

**3.0 Etapa 0. Una máquina sin reglas**

a) ¿Qué imprime `ejecutarTurno()` para el torno y la fresadora? ¿Por qué no imprime "torneando" ni "fresando"?

Ya que el metodo Procesar no fue declarado como virtual.

b) ¿Por qué el torno acumula 65 s y no 135 s?

Por que se ejecuto el metodo de maquina y no el de torno

**3.1 Etapa 1. Invariantes y constructores explicit**

a) ¿Qué regla de la sección 1.2 rompió `torno.piezasProcesadas = -5;`?

rompio la regla de que la cantidad de piezas procesadas no puede ser negativa.
al asignar -5 el objeto quedo en un estado invalido.

b) ¿De dónde salió la máquina "Fantasma"? ¿Qué hizo el compilador sin que se lo pidieras?

El constructor de Maquina aceptaba un string y no estaba declarado explicit, por eso, el compilador convirtio automaticamente "Fantasma" en una Maquina temporal para poder pasarla a imprimirFicha.

c) ¿Qué tuviste que cambiar en `Torno` y `Fresadora` y por qué?

Como los atributos de Maquina ahora son privados, Torno y Fresadora ya no pueden modificarlos directamente, cambie sus metodos procesar para obtener el nombre con getNombre y actualizar piezas y tiempo con registrarPieza y agregarTiempo. 
Tambien declare sus constructores explicit para evitar conversiones automaticas no intencionales.

**3.2 Etapa 2. Virtual y override**

a) La fresadora acumula 180 s, pero reporta 360 pzas/h. ¿Por qué un cálculo usó 60 s y el otro 10 s?

El metodo tiempoCicloSeg de Fresadora no tenia const, asi que no sobrescribia el metodo de Maquina.

b) ¿Tu compilador mostró alguna advertencia antes de usar `override`? ¿Qué mensaje dio después de agregar `override`?

No recuerdo.

**3.3 Etapa 3. La línea de producción**

a) ¿Por qué la línea procesó todo como "generico" si `procesar()` ya es virtual?

Porque ControlDeLinea guardaba las maquinas por valor en un vector de Maquina. Al copiarlas se perdio la parte derivada de Torno y Fresadora, asi que se produjo slicing y se ejecuto el metodo de Maquina. No entendi esto.

b) ¿Por qué el torno y la fresadora originales quedaron con 0 piezas?

Porque ControlDeLinea guardaba copias de las maquinas y el turno modifico esas copias, no los objetos originales.

c) La relación entre `ControlDeLinea` y `Maquina`, ¿es composición o agregación? ¿Por qué?

Es agregacion,la linea no las crea ni las destruye, asi que pueden seguir existiendo cuando ControlDeLinea desaparece.

**3.4 Etapa 4. Destructores**

| Momento | Mensajes de destrucción que aparecieron |
| --- | --- |
| `delete temporal;` sin destructor virtual | Destruyendo Maquina |
| `delete temporal;` con destructor virtual | Destruyendo Torno, Destruyendo Motor, Destruyendo Maquina |
| Al terminar `main` (torno y fresadora) | Destruyendo Maquina, Destruyendo Torno, Destruyendo Motor, Destruyendo Maquina |

a) ¿Qué advertencia mostró el compilador? ¿Por qué decimos que ya te estaba avisando del problema?

El compilador advirtio que borrar un objeto de tipo derivado mediante un puntero a Maquina, podia causar comportamiento indefinido. 

La advertencia anticipaba que no se destruirian correctamente las partes de Torno y su Motor.

b) ¿Por qué el torno de `main` sí se destruyó completo aunque el destructor no fuera virtual? ¿A qué máquina corresponde el primer "Destruyendo Maquina" después de "Fin de main"?

El torno de main se destruyo directamente como objeto local de tipo Torno, no a traves de un puntero a Maquina.

 El primer mensaje despues de Fin de main corresponde a la fresadora, que se creo despues del torno y se destruye primero al salir de main.

**3.5 Etapa 5. La clase abstracta**

a) Copia los mensajes de error y explica con tus palabras qué dice cada uno.

El compilador indica:

error: cannot declare variable 'pulidora' to be of abstract type 'Pulidora'
note: because the following virtual functions are pure within 'Pulidora':
note: virtual void Maquina::procesar()

error: cannot declare variable 'generica' to be of abstract type 'Maquina'
note: because the following virtual functions are pure within 'Maquina':
note: virtual void Maquina::procesar()
note: virtual int Maquina::tiempoCicloSeg() const

no se puede crear pulidora porque le falta implementar procesar. 

no se puede crear Maquina directamente porque es abstracta.

b) ¿Por qué es mejor este error de compilación que el comportamiento "generico" de antes?

Asi no se agregan maquinas que no hacen un proceso real ni se producen piezas con un comportamiento generico por accidente.

**3.6 Etapa 6. El contrato que el compilador no ve**

a) ¿Qué partes del contrato violó `InspeccionVisual`?

No registraba la pieza ni agregaba el tiempo trabajado, a parte de devolver 0 segundos, teniendo en cuenta que el tiempo de ciclo debe ser mayor que cero.

b) ¿Por qué el compilador no lo detectó?

El compilador verifica que los nombres, parametros y tipos coincidan, pero no puede saber si el comportamiento cumple las reglas del contrato.

c) ¿Qué pasaría si el próximo equipo tampoco lee el comentario del contrato?

El programa puede reportar piezas o tiempos incorrectos.

**3.7 Etapa 7. La base hace cumplir el contrato**

a) ¿Cómo puede una clase derivada sobrescribir un método que es privado en la base?

la derivada puede sobrescribir el metodo virtual privado porque private impide llamarlo directamente.

b) ¿Qué ocurre ahora con una máquina cuyo tiempo de ciclo es 0? Compáralo con la Etapa 6.

La base detecta que el tiempo no es valido y lanza una excepcion con el nombre de la maquina. 

En la etapa 6 se podia calcular una capacidad infinita y el error llegaba al reporte, ahora se detiene el procesamiento con un mensaje claro.

c) ¿Por qué la línea ignoró el "atajo" del torno, pero la llamada directa no? ¿Cómo corregiste el diseño?

La linea usa una referencia o puntero a maquina y llama al procesar no virtual de la base, por eso sigue las reglas centrales.

**3.8 Etapa 8. Herencia mal usada**

a) Da al menos dos razones por las que `BandaTransportadora : public Motor` es un error de diseño.

Una banda no es un motor. 

la herencia expone fijarVelocidad() para que cualquier parte del programa cambie directamente la velocidad de la banda.

b) La relación entre `BandaTransportadora` y `Motor`, ¿es composición o agregación? ¿Por qué?

es composicion el motor forma parte de la banda y no se comparte como un objeto externo.

**3.9 Etapa 9. La librería y la prueba final**

a) Después de agregar la `Cortadora`, ¿qué archivos mostró `git status`? ¿Cuántos archivos de la librería tuviste que modificar?

Despues de confirmar primero el commit de la libreria, git status debe mostrar src/main.cpp como modificado y include/Cortadora.h y src/Cortadora.cpp como archivos nuevos. ehhh???

No se modifica ningun archivo existente de la libreria para agregar la Cortadora.

b) ¿Qué pasó al escribir `-lplanta` antes de `src/main.cpp`? ¿Por qué?

 al encontrar la libreria antes del codigo que la necesita, aun no tenia referencias pendientes que resolver. 
 
 La libreria debe ir despues de src/main.cpp.

**3.10 Bitácora de dudas**

| # | Etapa | Duda | Cómo la resolví | Fuente |
| --- | --- | --- | --- | --- |
| 1 | 3 | Por que se perdia el comportamiento de Torno y Fresadora al copiarlas como Maquina | Comprendi que el vector guardaba copias de la clase base y se perdia la parte derivada; la linea debe guardar punteros a las maquinas existentes | PRACTICA.md, Etapa 3 |
| 2 | 7 | Como una derivada puede sobrescribir un metodo privado de la base | Distingui el control de acceso de la sobrescritura virtual: private impide llamarlo directamente, pero la base puede invocarlo mediante enlace dinamico | PRACTICA.md, Etapa 7 |
| 3 | 9 | Por que importa el orden de la libreria al enlazar | Aprendi que el enlazador busca dependencias de izquierda a derecha y por eso libplanta.a va despues del codigo que la utiliza | PRACTICA.md, Etapa 9 |

## Fase 4. Probar y mejorar

**4.1 Tabla de pruebas**

| # | Caso | Resultado esperado | Resultado obtenido | ¿Pasa? |
| --- | --- | --- | --- | --- |
| 1 | Máquina con nombre vacío | Excepción con mensaje claro | _____ | _____ |
| 2 | Reporte de una línea vacía | "La linea no tiene maquinas" | _____ | _____ |
| 3 | `agregar(nullptr)` | Devuelve `false` | _____ | _____ |
| 4 | Reporte antes de ejecutar el turno | 0 piezas, 0 s y capacidad calculada | _____ | _____ |
| 5 | Turno de 0 ciclos | Nada cambia | _____ | _____ |
| 6 | Torno con 3 ciclos | 3 piezas, 135 s, 80 pzas/h | _____ | _____ |
| 7 | Máquina con `tiempoCicloSeg()` igual a 0 | Excepción con mensaje claro | _____ | _____ |
| 8 | Máquina con `tiempoCicloSeg()` negativo | Excepción con mensaje claro | _____ | _____ |
| 9 | La misma máquina agregada dos veces | _____ | _____ | _____ |

**4.2 Bitácora de mejoras**

| # | Falla o mejora detectada | Qué cambié | Por qué |
| --- | --- | --- | --- |
| 1 | _____ | _____ | _____ |
| 2 | _____ | _____ | _____ |

Retos opcionales que intenté: _____

## Fase 5. Publicar en GitHub

**5.1 Enlace a mi fork**

https://github.com/TacocraftG/ulsa_ime_3_poo_evolucion_maquina.git

## Cierre y reflexión

**6.1 Errores que ya no pueden ocurrir**

| Error | Etapa en que apareció | Restricción que lo impide ahora | ¿Quién lo detecta: el compilador o el programa? |
| --- | --- | --- | --- |
| Contadores con valores inválidos | 1 | Los atributos son privados y solo la base modifica el estado mediante sus metodos | El compilador impide el acceso directo |
| Máquina "fantasma" creada a escondidas | 1 | El constructor de Maquina es explicit y no permite conversiones implicitas | El compilador |
| Método que no sobrescribe por un const | 2 | Las derivadas usan override, que exige que la firma coincida | El compilador |
| Máquinas copiadas y "rebanadas" | 3 | ControlDeLinea guarda punteros a Maquina en vez de copias por valor | El compilador y el diseño de la linea |
| Destrucción incompleta | 4 | Maquina tiene destructor virtual | El programa al destruir objetos mediante un puntero a la base |
| Máquina sin procesar propio | 5 | Maquina es abstracta y sus metodos ejecutarCiclo y tiempoCicloSeg son virtuales puros | El compilador |
| Tiempo de ciclo inválido | 7 | La base valida que el ciclo sea mayor que cero antes de procesar o calcular capacidad | El programa mediante una excepcion |
| Derivada que no registra sus piezas | 7 | procesar no es virtual y la base registra cada pieza y el tiempo del ciclo | El diseno de la base y el compilador impiden saltarse el proceso |

**6.2 ¿Cuál de los errores fue el más peligroso: el de compilación, la advertencia o el que compilaba sin problemas? ¿Por qué?**

El mas peligroso fue el que compilaba sin problemas, porque podia producir reportes incorrectos sin avisar, un error de compilacion detiene el programa y una advertencia al menos senala que hay un riesgo que se debe revisar.

**6.3 ¿Qué aprendiste en esta práctica?**

Se supone que aprendi que el diseno de las clases puede prevenir errores: encapsular el estado, usar metodos virtuales y override correctamente, y hacer que la clase base controle las reglas comunes. Tambien aprendi a usar composicion cuando una clase tiene un componente y agregacion cuando solo conoce objetos que existen por separado.

**6.4 ¿Qué cambiarías de tu proceso la próxima vez?**

Establecer las todas las reglas de las clases.