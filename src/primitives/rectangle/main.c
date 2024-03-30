// SPDX-License-Identifier: MIT
/// @file  main.c
/// @brief  Mostrar rectángulo.

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
// ps2sdk
#include <draw.h>
#include <graph.h>
#include <gs_psm.h>
#include <gs_gp.h>
#include <dma.h>

#define OFFSET 2048.0f

int init_gs(framebuffer_t *frame, zbuffer_t *z, int width, int height, int psm, int psmz)
{
	frame->width = width;
	frame->height = height;
	frame->psm = psm;
	frame->mask = 0;
        frame->address = graph_vram_allocate(width, height, psm, GRAPH_ALIGN_PAGE);
	z->address = graph_vram_allocate(width, height, psmz, GRAPH_ALIGN_PAGE);
	z->enable = 0;
	z->method = 0;
	z->zsm = 0;
	z->mask = 0;

	graph_initialize(frame->address,frame->width,frame->height,frame->psm,0,0);

	return 0;
}

void init_drawing_environment(framebuffer_t *frame, zbuffer_t *z, qword_t *buffer)
{
	qword_t *q = buffer;
	q = draw_setup_environment(q, 0, frame, z);
	q = draw_primitive_xyoffset(q, 0, 2048 - (frame->width / 2), 2048 - (frame->height / 2));
	q = draw_finish(q);
	dma_channel_send_normal(DMA_CHANNEL_GIF, buffer, q - buffer, 0, 0);
	draw_wait_finish();
}

qword_t *draw_rectangle(qword_t *q)
{
    int red = 0;
    int green = 255;
    int blue = 0;

    // Giftag
    PACK_GIFTAG(q, GIF_SET_TAG(4, 1, 0, 0, 0, 1), GIF_REG_AD);
    q++;
  
    // primitive data
    PACK_GIFTAG(q, GIF_SET_PRIM(GS_PRIM_SPRITE, 1, 0, 0, 0, 0, 0, 0, 0), GIF_REG_PRIM);
    q++;
    PACK_GIFTAG(q, GIF_SET_RGBAQ(red, green,  blue, 128, 128), GIF_REG_RGBAQ);
    q++;
    PACK_GIFTAG(q, GIF_SET_XYZ(ftoi4(-400 +OFFSET),
			       ftoi4(-300+OFFSET),
			       1), GIF_REG_XYZ2);
    q++;
    PACK_GIFTAG(q, GIF_SET_XYZ(ftoi4(0+OFFSET),
			       ftoi4(0+OFFSET),
			       1), GIF_REG_XYZ2);
    q++;
    return q;
}

int main()
{
    framebuffer_t frame;
    zbuffer_t z;

    qword_t *buffer;
    buffer = malloc(1600);

    dma_channel_initialize(DMA_CHANNEL_GIF, 0, 0);
    dma_channel_fast_waits(DMA_CHANNEL_GIF);

    init_gs(&frame, &z, 800, 600, GS_PSM_32, GS_PSMZ_24);
    init_drawing_environment(&frame,&z, buffer);

    dma_wait_fast();
    while (1) {
	qword_t *q = buffer;
	q = draw_disable_tests(q, 0, &z);
	q = draw_clear(q, 0, OFFSET - (frame.width / 2), OFFSET - (frame.height / 2), frame.width, frame.height, 20, 20, 20);
	q = draw_enable_tests(q, 0, &z);
	q = draw_rectangle(q);
	q = draw_finish(q);

	dma_channel_send_normal(DMA_CHANNEL_GIF, buffer, q - buffer, 0, 0);
	draw_wait_finish();
	graph_wait_vsync();

	sleep(2);
    }
    return 0; 
}
