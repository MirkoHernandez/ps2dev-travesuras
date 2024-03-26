# PACK_GIFTAG 

Este es probablemente el macro mas importante en PS2SDK.

Sirve para llenar un qword. 

```c
#define PACK_GIFTAG(Q,D0,D1) \
	Q->dw[0] = (u64)(D0), \
	Q->dw[1] = (u64)(D1)
```

A pesar del nombre, sirve para llenar qwords que no sean un GIFTAG.

```c
// set giftag
PACK_GIFTAG(q, GIF_SET_TAG(4, 1, 0, 0, 0, 1),GIF_REG_AD);
q++;
// set gs primitive data
PACK_GIFTAG(q, GIF_SET_PRIM(6, 0, 0, 0, 0, 0, 0, 0, 0), GIF_REG_PRIM);
q++;
PACK_GIFTAG(q, GIF_SET_RGBAQ((loop0 * 10), 0, 255 - (loop0 * 10), 0x80, 0x3F800000), GIF_REG_RGBAQ);
q++;
PACK_GIFTAG(q, GIF_SET_XYZ(( (loop0 * 20) << 4) + (2048<<4), ((loop0 * 10) << 4) + (2048<<4), 0), GIF_REG_XYZ2);
q++;
PACK_GIFTAG(q, GIF_SET_XYZ( (((loop0 * 20) + 100) << 4) + (2048<<4), (((loop0 * 10) + 100) << 4) + (2048<<4), 0), GIF_REG_XYZ2);
```
