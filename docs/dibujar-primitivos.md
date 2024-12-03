# Dibujar primitivos

Existen varias formas de dibujar primitivos utilizando giftags y
primitive data.

## Forma más fácil de dibujar.

No especificar campo REGS, dejarlo simplemente en GIF_REG_AD y
utilizar NLOOP para indicar la cantidad de data que se va a leer.
Luego cada qword tiene que indicar el tipo de data leída.

```c
// Indicar que se leera Address+Data 7 veces.
PACK_GIFTAG(q, GIF_SET_TAG(7, 1, 0, 0, 0, 1), GIF_REG_AD);
q++;
// Indicar primitivo
PACK_GIFTAG(q, GIF_SET_PRIM(GS_PRIM_TRIANGLE, 1, 0, 0, 0, 0, 0, 0, 0), GIF_REG_PRIM);
q++;

// Leer color y coordenada (repetir 3 veces para triangulo Gouraud)
PACK_GIFTAG(q, GIF_SET_RGBAQ(red, green,  blue, 128, 128), GIF_REG_RGBAQ);
q++;
PACK_GIFTAG(q, GIF_SET_XYZ(ftoi4(-400 +OFFSET),ftoi4(-300+OFFSET),1), GIF_REG_XYZ2);
q++;
```

## Forma más complicada de dibujar.
Especificar los registros manualmente. Macros draw facilitan esto,
parecer el único caso de uso sería encontrar formas de estrujar
rendimiento.

```c
// Leer 6 registros, EOP(8),  nloop una sola vez.
q->dw[0] = 0x6000000000008001;
// Especificar registros - col, pos, col, pos, col, pos
q->dw[1] = 0x0000000000515151;
q++;
```

# Requisitos para poder dibujar.


##  FLG tiene que corresponder con el tipo de data de GS primitive.

En packed mode es un qword por registro.

##  En primitive data se debe enviar qword que describa registro PRIM.

```c
q->dw[0] = GS_SET_PRIM(GS_PRIM_TRIANGLE, 0, 0, 0, 0, 0, 0, 0, 0);
q->dw[1] = GS_REG_PRIM;
```

##  El numero de qwords que se leen tiene que ser el correcto.

Giftag debe ser seguido de data para 7 registros (7 qwords en packed mode).

```c
PACK_GIFTAG(q, GIF_SET_TAG(7, 1, 0, 0, 0, 1), GIF_REG_AD);
```

##  El formato de color tiene que ser el correcto.

Al parecer el formato de color es diferente cuando se utiliza
GIF_REG_AD. En este caso el color se empaca en un solo dword, en el
otro se indica que se usara el registro de color GIF_REG_RGBAQ.

```c
 PACK_GIFTAG(q, GIF_SET_RGBAQ(red, green,  blue, 128, 128), GIF_REG_RGBAQ);
```

Cuando se especifica un registro de color en REGS (5) el formato es el siguiente.

| Writes to RGBAQ register (Q is unchanged) |        |
|-------------------------------------------|--------|
| 0-7                                       | R      |
| 8-31                                      | Unused |
| 32-39                                     | G      |
| 40-63                                     | Unused |
| 64-71                                     | B      |
| 72-95                                     | Unused |
| 96-103                                    | A      |
| 104-127                                   | Unused |

##  El formato de las coordenadas tiene que ser el correcto. 

Mitad de la pantalla es en (2048.0f 2048.0f), coordenadas en formato
fixed point 12.4. Esquina superior para 800x600 seria (ftoi4
(-(800/2) + 2048.0f).

# Conclusión

Cualquier error en alguno de estos puntos anteriores puede hacer que no se dibuje
nada en pantalla.

Los macros GIF_SET_RGBAQ, GIF_SET_XYZ y ftoi4 facilitan bastante el
proceso.

