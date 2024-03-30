# Registros

Son 16 tipos de registros que se pueden marcar en un giftag. Esto es,
indicar que registros serán utilizados por GS, luego se envía la data
para cada uno.

## Ejemplo

En el siguiente ejemplo se marcan 6 registros:

 - GIFTAG: NREG es 6, EOP es 8,  NLOOP es  1
 - REGS: 3 pares compuestos por coordenada (5) y posición (1)  (515151)

```c
q->dw[0] = 0x6000000000008001;
q->dw[1] = 0x0000000000515151;
```

# REGS

Constantes para REGS en sdk.

```c
/** Drawing primitive setting. */
#define GIF_REG_PRIM			0x00
/** Vertex color setting. */
#define GIF_REG_RGBAQ			0x01
/** Specification of vertex texture coordinates. */
#define GIF_REG_ST				0x02
/** Specification of vertex texture coordinates. */
#define GIF_REG_UV				0x03
/** Setting for vertex coordinate values. */
#define GIF_REG_XYZF2			0x04
/** Setting for vertex coordinate values. */
#define GIF_REG_XYZ2			0x05
/** Texture information setting. */
#define GIF_REG_TEX0			0x06
/** Texture information setting. (Context 1) */
#define GIF_REG_TEX0_1			0x06
/** Texture information setting. (Context 2) */
#define GIF_REG_TEX0_2			0x07
/** Texture wrap mode. */
#define GIF_REG_CLAMP			0x08
/** Texture wrap mode. (Context 1) */
#define GIF_REG_CLAMP_1			0x08
/** Texture wrap mode. (Context 2) */
#define GIF_REG_CLAMP_2			0x09
/** Vertex fog value setting. */
#define GIF_REG_FOG				0x0A
/** Setting for vertex coordinate values. (Without Drawing Kick) */
#define GIF_REG_XYZF3			0x0C
/** Setting for vertex coordinate values. (Without Drawing Kick) */
#define GIF_REG_XYZ3			0x0D
/** GIFtag Address+Data */
#define GIF_REG_AD				0x0E
/** GIFtag No Operation */
#define GIF_REG_NOP				0x0F
```
