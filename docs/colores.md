# Colores

## Cuando el registro de color esta seteado en REGS:

Colores están en 128 bit.

Un dword para red y green, otro para blue y alpha.

 - 8 bits por pixel.

```c
q->dw[0] = (red&0xff) | (green&0xff)<<32;
q->dw[1] = (blue&0xff) | (alpha&0xff)<< 32;
```

## Utilizando GIF_REG_AD

Cuando se especifica que se leerá address+data (GIF_REG_AD) entonces el
color esta en 64 bits, se tiene que indicar que se usara registro de
color(GIF_REG_RGBAQ equivalente a 5).


El siguiente macro facilita es establecer colores.

```c
#define GS_SET_RGBAQ(R,G,B,A,Q) \
	(u64)((R) & 0x000000FF) <<  0 | (u64)((G) & 0x000000FF) <<  8 | \
	(u64)((B) & 0x000000FF) << 16 | (u64)((A) & 0x000000FF) << 24 | \
	(u64)((Q) & 0xFFFFFFFF) << 32
``` 


