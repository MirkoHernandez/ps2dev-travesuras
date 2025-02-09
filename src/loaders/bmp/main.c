// SPDX-License-Identifier: MIT
/// @file  main.c
/// @brief Loader básico para formato BMP. 
/// @details Solo soporta 24 bits per pixel.

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
#include <sys/stat.h>

#define OFFSET 2048.0f

#if __DOXYGEN__
#define FORCE_INLINE
#endif

/// \addtogroup  loaders Loaders: Loader basico para formato BMP.
/// @{
typedef struct __attribute__((__packed__)) bmp_header {
	uint16_t file_type;
	uint32_t file_size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t bitmap_offset;
	uint32_t size;
	int32_t width;
	int32_t height;
	uint16_t planes;
	uint16_t bits_per_pixel;
	uint32_t compression;
	uint32_t size_of_bitmap;
	int32_t x_per_meter;
	int32_t y_per_meter;
	uint32_t colors_used;
	uint32_t colors_important;
	uint32_t redMask;
	uint32_t greenMask;
	uint32_t blueMask;
} bmp_header;

typedef struct loaded_bitmap
{
	uint32_t width;
	uint32_t height;
	uint32_t *pixels;
} loaded_bitmap;


/// @details Loader solo funciona para imágenes de 24 bit.
/// Es necesario invertir cada  pixel (pasar R a B y B  a R) y también
/// invertir la imagen en vertical.
loaded_bitmap load_bmp(char *path)
{
	
        bmp_header  header = {};
	FILE *file_path = fopen(path, "rb");

	loaded_bitmap lb = {};

	if (file_path == NULL) {
		printf("Failed to load bmp file: %s\n", file_path);
		return lb;
	}
	
	fread(&header, 1, sizeof(bmp_header), file_path);

	// Loader solo soporta 24 bits per pixel.
	if (header.bits_per_pixel != 24) {
		return lb;
	}	
	
        uint32_t bitmap_size = header.width * header.height * 3;
	uint8_t *pixels = malloc (bitmap_size);
	
	fseek(file_path, header.bitmap_offset, SEEK_SET);
	fread(pixels, 1, bitmap_size, file_path);
	
	// Invertir R y B.
	for (int i = 0; i < bitmap_size; i +=3) {
		uint8_t tmp = pixels[i];
		pixels[i] = pixels[i+2];
		pixels[i+2] = tmp;
	}
	
	lb.width = header.width;
	lb.height = header.height;

	// Invertir Verticalmente.
	for (int y = 0; y < lb.height/2; y++) {
		uint8_t *row = pixels + (y * lb.width * 3); 
		uint8_t *row2 = pixels + ((lb.height - 1 - y) * lb.width * 3); 
		for (int x = 0; x < lb.width * 3; x++) {
			uint8_t tmp = row[x];
			row[x] = row2[x];
			row2[x] = tmp;
		}
	}

	lb.pixels = pixels;
	return lb;
}
/// @}
	
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


void init_texbuf(texbuffer_t *texbuf, int psm, int width)
{
	// Allocate some vram for the texture buffer
	texbuf->width = width;
	texbuf->psm = psm;
	texbuf->address = graph_vram_allocate(16,16,texbuf->psm,GRAPH_ALIGN_BLOCK);
}

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

void load_texture(texbuffer_t *texbuf, uint8_t *sprite)
{
	packet_t *packet = packet_init(10,PACKET_NORMAL);
	
	qword_t *q = packet->data;

	q = packet->data;
	
	q = draw_texture_transfer(q,sprite,texbuf->width,texbuf->width,texbuf->psm,texbuf->address,texbuf->width);
	q = draw_texture_flush(q);

	dma_channel_send_chain(DMA_CHANNEL_GIF,packet->data, q - packet->data, 0,0);
	dma_wait_fast();
	
	packet_free(packet);
}

void setup_texture(texbuffer_t *texbuf)
{
	packet_t *packet = packet_init(10,PACKET_NORMAL);
	qword_t *q = packet->data;

	// Using a texture involves setting up a lot of information.
	clutbuffer_t clut = {0};
	/* clutbuffer_t clut = NULL; */

	lod_t lod = {0};
	
	texbuf->info.width = draw_log2(texbuf->width);
	texbuf->info.height = draw_log2(texbuf->width);
	/* texbuf->info.components = TEXTURE_COMPONENTS_RGB; */
	texbuf->info.components = TEXTURE_COMPONENTS_RGBA;
	texbuf->info.function = TEXTURE_FUNCTION_DECAL;

	q = draw_texture_sampling(q,0,&lod);
	q = draw_texturebuffer(q,0,texbuf,&clut);

	// Now send the packet, no need to wait since it's the first.
	dma_channel_send_normal(DMA_CHANNEL_GIF,packet->data,q - packet->data, 0, 0);
	dma_wait_fast();

	packet_free(packet);

}

qword_t *draw_sprite(qword_t *q)
{
	texrect_t trect;
	color_t col;

	col.r = 0;
	col.g = 0;
	col.b = 0;
	col.q = 0;
	col.a = 255;
	
	trect.v0.x = -128.0;
	trect.v0.y = 0;
	trect.v0.z = 0;
	
	trect.v1.x = 0.0;
	trect.v1.y = 128.0;
	trect.v1.z = 0;
	
	trect.t0.u = 0.0f;
	trect.t0.v = 0.0f ;
	
	trect.t1.u = 16.0f;
	trect.t1.v = 16.0f;
	
	trect.color = col;
	
	q = draw_rect_textured(q,0,&trect);
	return 	q;
}

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

	init_texbuf(&texbuf, GS_PSM_24, 16);
	init_drawing_environment(&frame,&z);
	
	loaded_bitmap lb = load_bmp("host:test.bmp");

	load_texture(&texbuf, lb.pixels);
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

