// SPDX-License-Identifier: MIT
/// @file  main.c
/// @brief  Mostrar textura en 2D.
/// @details  Se utilizan  funciones  similares a  las encontradas  en
/// samples/draw/texture.

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
// ps2sdk
#include <packet.h>
#include <draw.h>
#include <graph.h>
#include <gs_psm.h>
#include <gs_gp.h>
#include <dma.h>
#include <kernel.h>


unsigned char sprite[] __attribute__((aligned(16))) = {
	0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,
	0x00,0x00,0x00, 0xcc,0x3b,0x24, 0xcc,0x3b,0x24, 0xcc,0x3b,0x24,
	0xcc,0x3b,0x24, 0xcc,0x3b,0x24, 0x00,0x00,0x00, 0x00,0x00,0x00, 							     
	0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,
								     
	0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,
        0xcc,0x3b,0x24, 0xcc,0x3b,0x24, 0xcc,0x3b,0x24, 0xcc,0x3b,0x24,
	0xcc,0x3b,0x24, 0xcc,0x3b,0x24, 0xcc,0x3b,0x24, 0xcc,0x3b,0x24,  
	0xcc,0x3b,0x24, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,

	0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,
        0x2b,0x00,0x00, 0x2b,0x00,0x00, 0x2b,0x00,0x00, 0xf4,0xbe,0x98,
        0xf4,0xbe,0x98, 0x2b,0x00,0x00, 0xf4,0xbe,0x98, 0x00,0x00,0x00,
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,

	0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x2b,0x00,0x00,  
        0xf4,0xbe,0x98, 0x2b,0x00,0x00, 0xf4,0xbe,0x98, 0xf4,0xbe,0x98,
        0xf4,0xbe,0x98, 0x2b,0x00,0x00, 0xf4,0xbe,0x98, 0xf4,0xbe,0x98,  
        0xf4,0xbe,0x98, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,  

	0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x2b,0x00,0x00,  
        0xf4,0xbe,0x98, 0x2b,0x00,0x00, 0x2b,0x00,0x00, 0xf4,0xbe,0x98,
        0xf4,0xbe,0x98, 0xf4,0xbe,0x98, 0x2b,0x00,0x00, 0xf4,0xbe,0x98,  
        0xf4,0xbe,0x98, 0xf4,0xbe,0x98, 0x00,0x00,0x00, 0x00,0x00,0x00,  

	0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x2b,0x00,0x00,  
        0x2b,0x00,0x00, 0xf4,0xbe,0x98, 0xf4,0xbe,0x98, 0xf4,0xbe,0x98,
        0xf4,0xbe,0x98, 0x2b,0x00,0x00, 0x2b,0x00,0x00, 0x2b,0x00,0x00,  
        0x2b,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,  

	0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,  
        0x00,0x00,0x00, 0xf4,0xbe,0x98, 0xf4,0xbe,0x98, 0xf4,0xbe,0x98,
        0xf4,0xbe,0x98, 0xf4,0xbe,0x98, 0xf4,0xbe,0x98, 0xf4,0xbe,0x98, 
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,  

	0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,  
        0x2b,0x00,0x00, 0x2b,0x00,0x00, 0xcc,0x3b,0x24, 0x2b,0x00,0x00,
	0x2b,0x00,0x00, 0x2b,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 							  
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,  

	0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x2b,0x00,0x00,  
        0x2b,0x00,0x00, 0x2b,0x00,0x00, 0x2b,0x00,0x00, 0xcc,0x3b,0x24, 
	0xcc,0x3b,0x24, 0x2b,0x00,0x00, 0x2b,0x00,0x00, 0x00,0x00,0x00, 							  
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,  

	0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x2b,0x00,0x00,  
        0x2b,0x00,0x00, 0x2b,0x00,0x00, 0xcc,0x3b,0x24, 0xcc,0x3b,0x24, 
	0xff,0xfd,0x55, 0xcc,0x3b,0x24, 0xcc,0x3b,0x24, 0xf4,0xbe,0x98,  
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,  

	0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x2b,0x00,0x00,  
        0x2b,0x00,0x00, 0x2b,0x00,0x00, 0x2b,0x00,0x00, 0xcc,0x3b,0x24, 
	0xcc,0x3b,0x24, 0xcc,0x3b,0x24, 0xcc,0x3b,0x24, 0xcc,0x3b,0x24,  
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,  

	0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0xcc,0x3b,0x24,  
        0x2b,0x00,0x00, 0x2b,0x00,0x00, 0xf4,0xbe,0x98, 0xf4,0xbe,0x98,  
	0xf4,0xbe,0x98, 0xcc,0x3b,0x24, 0xcc,0x3b,0x24, 0xcc,0x3b,0x24,  
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,

	0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,  
        0xcc,0x3b,0x24, 0x2b,0x00,0x00, 0xf4,0xbe,0x98, 0xf4,0xbe,0x98,  
	0xcc,0x3b,0x24, 0xcc,0x3b,0x24, 0xcc,0x3b,0x24,0x00,0x00,0x00,  
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,

	0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,  
        0x00,0x00,0x00, 0xcc,0x3b,0x24, 0xcc,0x3b,0x24, 0xcc,0x3b,0x24,  
        0x2b,0x00,0x00, 0x2b,0x00,0x00, 0x2b,0x00,0x00, 0x00,0x00,0x00, 								  
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,

	0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,
        0x00,0x00,0x00, 0x2b,0x00,0x00, 0x2b,0x00,0x00, 0x2b,0x00,0x00,  
        0x2b,0x00,0x00, 0x2b,0x00,0x00, 0x2b,0x00,0x00, 0x2b,0x00,0x00,  
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,

	0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,
        0x00,0x00,0x00, 0x2b,0x00,0x00, 0x2b,0x00,0x00, 0x2b,0x00,0x00,
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,
        0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00,
};

#define OFFSET 2048.0f
	
void init_gs(framebuffer_t *frame, zbuffer_t *z)
{
	// Define a 32-bit 640x512 framebuffer.
	frame->width = 640;
	frame->height = 512;
	frame->mask = 0;
	frame->psm = GS_PSM_32;
	frame->address = graph_vram_allocate(frame->width,frame->height, frame->psm, GRAPH_ALIGN_PAGE);

	// Enable the zbuffer.
	z->enable = DRAW_ENABLE;
	z->mask = 0;
	z->method = ZTEST_METHOD_GREATER_EQUAL;
	z->zsm = GS_ZBUF_32;
	z->address = graph_vram_allocate(frame->width,frame->height,z->zsm, GRAPH_ALIGN_PAGE);

	// Initialize the screen and tie the first framebuffer to the read circuits.
	graph_initialize(frame->address,frame->width,frame->height,frame->psm,0,0);

}

/// \defgroup texture Dibujar: Textura, inicializar texbuf.
/// @{
/// @details  Similar a la init normal de frame y z buffers.
/// Es exactamente igual a memory allocation para frame buffer.
void init_texbuf(texbuffer_t *texbuf)
{
	// Allocate some vram for the texture buffer
	texbuf->width = 16;
	texbuf->psm = GS_PSM_24;
	texbuf->address = graph_vram_allocate(16,16,GS_PSM_24,GRAPH_ALIGN_BLOCK);
}

	
/// @}

void init_drawing_environment_old(framebuffer_t *frame, zbuffer_t *z, qword_t *buffer)
{
	qword_t *q = buffer;
	q = draw_setup_environment(q, 0, frame, z);
	q = draw_primitive_xyoffset(q, 0, 2048 - (frame->width / 2), 2048 - (frame->height / 2));
	q = draw_finish(q);
	dma_channel_send_normal(DMA_CHANNEL_GIF, buffer, q - buffer, 0, 0);
	draw_wait_finish();
}


void init_drawing_environment(framebuffer_t *frame, zbuffer_t *z)
{

	packet_t *packet = packet_init(20,PACKET_NORMAL);

	// This is our generic qword pointer.
	qword_t *q = packet->data;

	// This will setup a default drawing environment.
	q = draw_setup_environment(q,0,frame,z);

	// Now reset the primitive origin to 2048-width/2,2048-height/2.
	q = draw_primitive_xyoffset(q,0,(2048-320),(2048-256));

	// Finish setting up the environment.
	q = draw_finish(q);

	// Now send the packet, no need to wait since it's the first.
	dma_channel_send_normal(DMA_CHANNEL_GIF,packet->data,q - packet->data, 0, 0);
	dma_wait_fast();

	packet_free(packet);

}

/// \addtogroup  texture Dibujar: Textura, se realiza transferencia de data de sprite a vram. 
/// @{

/// @brief Traspasar data de textura a vram.
void load_texture(texbuffer_t *texbuf, unsigned char *sprite)
{
	packet_t *packet = packet_init(50,PACKET_NORMAL);

	qword_t *q = packet->data;

	q = packet->data;

	q = draw_texture_transfer(q,sprite,16,16,GS_PSM_24,texbuf->address,texbuf->width);
	q = draw_texture_flush(q);

	dma_channel_send_chain(DMA_CHANNEL_GIF,packet->data, q - packet->data, 0,0);
	dma_wait_fast();
	
	packet_free(packet);
}

/// @brief Establecer información de textura en GS.
/// @details clut y lod pueden quedar en 0 (para dibujar en 2d).
/// draw_texture_sampling  parece   ser  opcional.  draw_texturebuffer
/// utiliza registros 0x06 o 0x07 (si context es 1) para establecer información de la textura.
/// Sin este paso dibujar textura no funciona.	
void setup_texture(texbuffer_t *texbuf)
{
	packet_t *packet = packet_init(10,PACKET_NORMAL);
	qword_t *q = packet->data;

	// Using a texture involves setting up a lot of information.
	clutbuffer_t clut = {0};

	lod_t lod = {0};
	
	texbuf->info.width = draw_log2(16);
	texbuf->info.height = draw_log2(16);
	texbuf->info.components = TEXTURE_COMPONENTS_RGB;
	texbuf->info.function = TEXTURE_FUNCTION_DECAL;

	q = draw_texture_sampling(q,0,&lod);
	q = draw_texturebuffer(q,0,texbuf,&clut);

	// Now send the packet, no need to wait since it's the first.
	dma_channel_send_normal(DMA_CHANNEL_GIF,packet->data,q - packet->data, 0, 0);
	dma_wait_fast();

	packet_free(packet);

}

/// @brief  Dibujar  textura utilizando  draw_rect_textured.
/// @details Coordenadas  de textura comienzan  en  0 para simplificar;  en código  real  address se  especificaría
/// address de textura en GS. 
qword_t *draw_sprite(qword_t *q)
{
	texrect_t trect;
	
	color_t col;

	col.r = 100;
	col.g = 100;
	col.b = 200;
	col.q = 0;
	col.a = 1;
	
	trect.v0.x = -128;
	trect.v0.y = 0;
	trect.v0.z = 0;
	
	trect.t0.u = 0.0f;
	trect.t0.v = 0.0f ;
	
	trect.v1.x = 0;
	trect.v1.y = 128;
	trect.v1.z = 0;
	
	trect.t1.u = 16.0f;
	trect.t1.v = 16.0f ;
	
	trect.color = col;

	q = draw_rect_textured(q,0,&trect);
	return 	q;
}
/// @}

int main()
{
	framebuffer_t frame;
	zbuffer_t z;
	texbuffer_t texbuf;

	
	qword_t *buffer;
	buffer = malloc(1600);

	dma_channel_initialize(DMA_CHANNEL_GIF, 0, 0);
	dma_channel_fast_waits(DMA_CHANNEL_GIF);

	init_gs(&frame, &z);
	
	/* allocate memory para texture buffer */
	init_texbuf(&texbuf);
	init_drawing_environment(&frame,&z);

	load_texture(&texbuf,sprite);
	setup_texture(&texbuf);
	
	dma_wait_fast();
	sleep(1);
	
	while (1) {
		qword_t *q = buffer;
		q = draw_disable_tests(q, 0, &z);
		q = draw_clear(q, 0, OFFSET - (frame.width /2),
			       OFFSET - (frame.height / 2),
			       frame.width, frame.height, 100, 100, 100);
		
		q = draw_enable_tests(q, 0, &z);
		
		q = draw_sprite(q);
		q = draw_finish(q);
		
		dma_channel_send_normal(DMA_CHANNEL_GIF, buffer, q - buffer, 0, 0);
		draw_wait_finish();
		graph_wait_vsync();

		sleep(1);
	}
	return 0;
}
