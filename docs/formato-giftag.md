# Formato GIFTAG

128 bits.

- Aca se establece que registros se usaran

Primitive data se almacena en batches.

 1 REGS: 64 bits. Se puede considerar array de 16 elementos. 
 2 NREG: Número de registros utilizados.
 3 FLG: Formato de primitivo data
 4 PRIM: Primitivo
 5 EOP: Ultimo GIFTAG
 6 PRE: Pasar PRIM de registro a GS.
 7 NLOOP: Cuantas veces leer data especificada en registros. 

# Ejemplo

NREG:6
PRIM:8 -> 1000 Solo establece shading.

```c
q->dw[0] = 0x600000000000    8       0     0    1;
```

# Código de PS2SDK

Macros que demuestran como se deben colocar los registros.

```c
PACK_GIFTAG(q, GIF_SET_TAG(4, 1, 0, 0, 0, 1),GIF_REG_AD);

#define GIF_SET_TAG(NLOOP,EOP,PRE,PRIM,FLG,NREG) \
	(u64)((NLOOP) & 0x00007FFF) <<  0 | (u64)((EOP)  & 0x00000001) << 15 | \
	(u64)((PRE)   & 0x00000001) << 46 | (u64)((PRIM) & 0x000007FF) << 47 | \
	(u64)((FLG)   & 0x00000003) << 58 | (u64)((NREG) & 0x0000000F) << 60

/** Address+Data */
#define GIF_REG_AD				0x0E
```


