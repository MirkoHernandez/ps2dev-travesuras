# Timing

En PS2 hardware existen 4 timers pero solo se podrían usar un máximo
de 3 (dependiendo de como funciona PS2SDK).

## Método para calcular ciclos de cpu.

Mecanismo para poder medir el tiempo y la cantidad de ciclos de cpu.

Se configura un registro de timer llamado Tn_MODE, n corresponde al
registro. La configuración determina (entre otras cosas):

- cuantas veces  por segundo ocurre registro Tn_COUNT.
- interrupt de comparación (se puede utilizar para un valor arbitrario). 
- interrupt de overflow.

Tn_COUNT es de solo 16 bits, tiene overflow en 65536 ticks.

Un método para contar ciclos consiste en utilizar solamente el
interrupt de overflow (ignorar el de comparación).

Overflows se van contando con un variable u64.

Cantidad de ciclos se calcula fácilmente multiplicando la cantidad de
overflows por el máximo de ciclos, sumado a la cantidad de ciclos
leídos actuales.

## Referencias

https://forums.ps2dev.org/viewtopic.php?t=2842&highlight=clock+tick

https://psi-rockin.github.io/ps2tek/#eetimers

