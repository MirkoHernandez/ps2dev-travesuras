// SPDX-License-Identifier: MIT
/// @file  main.c
/// @brief  Leer input usando  un  mínimo  de funciones de ps2sdk.
/// @details  En este  programa se  muestra triangulo  y se  cambia su
/// color dependiendo del botón presionado.
/// Código esta basado en sample/rpc/pad.c de PS2SDK,
/// en este archivo se intenta describir como funciona.

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>

#include <kernel.h> /// SleepThread
#include <libpad.h> /// Set de instrucciones para usar con IOP module.
#include <sifrpc.h> /// SifInitRpc, comunicar con IOP module.
#include <loadfile.h> /// SifLoadModule, cargar IOP module.

// drawing
#include <draw.h>
#include <graph.h>
#include <gs_psm.h>
#include <gs_gp.h>
#include <dma.h>

//// Defines
#define OFFSET 2048.0f

//// Globals
int red = 0;
int green = 255;
int blue = 0;

// @Brief Pad code
static char padBuf[256] __attribute__((aligned(64)));
static char actAlign[6];
static int actuators;

/// \defgroup  input-load-iop Input: Cargar IOP  module (v1).
/// @details Para poder  leer  input se  necesita
/// cargar un  IOP module,  generalmente se cargan  los ya  provisto por
/// PS2SDK. En este caso se copian  modulos SIO2MAN y PADMAN  a IOP
/// por rpc. Al parecer el modulo PADMAN incluye las funciones que se
/// pueden llamar usando la librería libpad.
/// @{
static void
loadModules(void)
{
	int ret;
	ret = SifLoadModule("rom0:SIO2MAN", 0, NULL);
	if (ret < 0) {
		printf("sifLoadModule sio failed: %d\n", ret);
		SleepThread();
	}

	ret = SifLoadModule("rom0:PADMAN", 0, NULL);
	if (ret < 0) {
		printf("sifLoadModule pad failed: %d\n", ret);
		SleepThread();
	}
}
/// @}


/// \defgroup input-read-pad Input: Leer input de pad (v1).
/// @brief Usando libpad.
/// @details Slot debería ser 0 siempre a menos que se este usando multitap.
/// @{

/// @brief Leer  estado de pad.
/// @details Los estados  disponibles son
/// disconnected,  findpad,  findctp1,   execcmd,  stable,  error.  Se
/// utiliza  padGetState para  leer  estado. waitPadReady  simplemente
/// espera hasta que estado de pad sea stable.
static int waitPadReady(int port, int slot) {
	int state;
	int lastState;
	char stateString[16];

	state = padGetState(port, slot);
	lastState = -1;
	while((state
	       != PAD_STATE_STABLE) && (state != PAD_STATE_FINDCTP1)) {
		if (state != lastState) {
			padStateInt2String(state,        stateString);
			printf("Please wait, pad(%d,%d) is in state
			       %s\n",
			       port, slot, stateString);
		}
		lastState = state;
		state=padGetState(port, slot);
	}
	// Were the pad ever 'out of sync'?
	if (lastState != -1) {
		printf("Pad OK!\n");
	}
	return 0;
}


/// @brief inicializar actuators de dual shock controller.
/// @details
/// A pesar del nombre ésta función solo inicia actuators de dual shock controller.
/// padInfoMode utiliza padGetDmaStr para obtener toda (al parecer)
/// la data de pad y de eso obtiene pdata->nrOfModes; si cuarto parámetro es -1,
/// de lo contrario  información especifica de algún modo.
/// Si se confirma que pad es dual shock se establece padSetMainMode a
/// PAD_MMODE_DUALSHOCK (al  parecer es necesario verificar posteriormente  estado de
/// pad). Luego  se  obtiene numero  de actuators  con
/// padInfoAct y se establece estado inicial de actuators con
/// padSetActAlign.
 
static int
initializePad(int port, int slot) {

	int ret;
	int modes;
	int i;

	waitPadReady(port, slot);

	// How many different  modes can this device  operate in? i.e.
	// get # entrys in the modetable
	modes   =   padInfoMode(port, slot, PAD_MODETABLE, -1);
	printf("The device has %d modes\n", modes);

	if (modes > 0) {
		printf("( ");
		for (i = 0; i < modes; i++) {
			printf("%d", padInfoMode(port, slot,
						 PAD_MODETABLE, i));
		}
		printf(")");
	}

	printf("It is currently using mode %d\n",
	       padInfoMode(port, slot, PAD_MODECURID, 0));

	// If modes == 0, this is  not a Dual shock controller (it has
	// no actuator engines)
	if (modes == 0) {
		printf("This is a digital controller?\n");
		return 1;
	}

	// Verify that the controller has a DUAL SHOCK mode
	i = 0;
	do {
		if (padInfoMode(port, slot, PAD_MODETABLE, i) == PAD_TYPE_DUALSHOCK)
			break;
		i++;
	} while (i < modes);
	if (i >= modes) {
		printf("This is no Dual Shock controller\n");
		return 1;
	}

	// If ExId != 0x0 => This controller has actuator engines
	// This check should always pass if the Dual Shock test above passed
	ret = padInfoMode(port, slot, PAD_MODECUREXID, 0);
	if (ret == 0) {
		printf("This is no Dual Shock controller??\n");
		return 1;
	}
	
	printf("Enabling dual shock functions\n");

	// When using MMODE_LOCK, user cant change mode with Select button
	padSetMainMode(port, slot, PAD_MMODE_DUALSHOCK, PAD_MMODE_LOCK);

	waitPadReady(port, slot);
	printf("infoPressMode: %d\n", padInfoPressMode(port, slot));

	waitPadReady(port, slot);
	printf("moderateness: %d\n", padEnterPressMode(port, slot));

	waitPadReady(port, slot);
	actuators = padInfoAct(port, slot, -1, 0);
	printf("# of actuators: %d\n",actuators);

	if (actuators != 0) {
		actAlign[0] = 0;   // Enable small engine
		actAlign[1] = 1;   // Enable big engine
		actAlign[2] = 0xff;
		actAlign[3] = 0xff;
		actAlign[4] = 0xff;
		actAlign[5] = 0xff;

		waitPadReady(port, slot);
		printf("padSetActAlign: %d\n",
		       padSetActAlign(port, slot, actAlign));
	}
	else {
		printf("Did not find any actuators.\n");
	}

	waitPadReady(port, slot);

	return 1;
}
/// @}

int
init_gs(framebuffer_t *frame, zbuffer_t *z, int width, int height, int psm, int psmz)
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

void
init_drawing_environment(framebuffer_t *frame, zbuffer_t *z, qword_t *buffer)
{
	qword_t *q = buffer;
	q = draw_setup_environment(q, 0, frame, z);
	q = draw_primitive_xyoffset(q, 0, 2048 - (frame->width / 2), 2048 - (frame->height / 2));
	q = draw_finish(q);
	dma_channel_send_normal(DMA_CHANNEL_GIF, buffer, q - buffer, 0, 0);
	draw_wait_finish();
}

qword_t *
draw_triangle(qword_t *q)
{
	// Giftag
	PACK_GIFTAG(q, GIF_SET_TAG(7, 1, 0, 0, 0, 1), GIF_REG_AD);
	q++;
  
	// primitive data
	PACK_GIFTAG(q, GIF_SET_PRIM(GS_PRIM_TRIANGLE, 1, 0, 0, 0, 0, 0, 0, 0), GIF_REG_PRIM);
  
	q++;
	PACK_GIFTAG(q, GIF_SET_RGBAQ(red, green,  blue, 128, 128), GIF_REG_RGBAQ);
	q++;
	PACK_GIFTAG(q, GIF_SET_XYZ(ftoi4(-400 +OFFSET),
				   ftoi4(-300+OFFSET),
				   1), GIF_REG_XYZ2);
	q++;
	PACK_GIFTAG(q, GIF_SET_RGBAQ(red, green,  blue, 128, 128), GIF_REG_RGBAQ);
	q++;
	PACK_GIFTAG(q, GIF_SET_XYZ(ftoi4(0+OFFSET),
				   ftoi4(0+OFFSET),
				   1), GIF_REG_XYZ2);
	q++;
	PACK_GIFTAG(q, GIF_SET_RGBAQ(red, green,  blue, 128, 128), GIF_REG_RGBAQ);
	q++;
	PACK_GIFTAG(q, GIF_SET_XYZ(ftoi4(120 +OFFSET),
				   ftoi4(-180+OFFSET),
				   1), GIF_REG_XYZ2);
	q++;
	return q;
}

/// \defgroup input-main Input: Main (v1)
/// @brief Función main que incluye manejo de input básica.
/// @details 1 Inicializar pad:
/// Primero que nada se inicia SIF con SifInitRpc,
/// luego se carga el modulo IOP para pad.
/// padInit:
/// - Utiliza variable global padInitialised para ver si pad esta inicializado
/// - SifBindRpc realiza "verdadera" inicialización con rpc a IOP.
/// - Finalmente padPortInit establece valores de pad_state a 0.
/// padPortOpen:
/// - Requiere que buffer padbuf este alineado en 16-byte.
/// - Realiza cast de pad_data para llenar padbuf.
/// - Se sincroniza padbuf y variable global buffer.
/// - Se establecen valores en  variables globales buffer y padsif.
/// - Esas dos variables se utilizan en SifCallRpc.
/// - Variable global PadState se establece con padArea y buffer.
/// Luego se inicializan actuators con initializePad.
/// 2 Lectura de Pad. Se utiliza padGetState y padRead para leer input de pad.
///  se establecen valores de variables locales buttons 
///  y paddata. La estructura padButtonStatus almacena estado de todos los botones.


/// @{
int
main()
{

	int ret;
	int port, slot;
	struct padButtonStatus buttons;
	u32 paddata;
	u32 old_pad = 0;
	u32 new_pad;

	SifInitRpc(0);
	loadModules();
	padInit(0);

	port = 0; // 0 -> Connector 1, 1 -> Connector 2
	slot = 0; // Always zero if not using multitap

	printf("PortMax: %d\n", padGetPortMax());
	printf("SlotMax: %d\n", padGetSlotMax(port));

	if((ret = padPortOpen(port, slot, padBuf)) == 0) {
		printf("padOpenPort failed: %d\n", ret);
		SleepThread();
	}
	
	if(!initializePad(port, slot)) {
		printf("pad initalization failed!\n");
		SleepThread();
	}
  
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
		//// Procesar input
		ret=padGetState(port, slot);
		while((ret != PAD_STATE_STABLE) && (ret != PAD_STATE_FINDCTP1)) {
			if(ret==PAD_STATE_DISCONN) {
				printf("Pad(%d, %d) is disconnected\n", port, slot);
			}
			ret=padGetState(port, slot);
		}

		ret = padRead(port, slot, &buttons); // port, slot, buttons

		if (ret != 0) {
			paddata = 0xffff ^ buttons.btns;
			
			new_pad = paddata & ~old_pad;
			old_pad = paddata;

			
			// Directions
			if(new_pad & PAD_LEFT) {
				red = 255;
				green = 0;
				blue = 0;
				printf("LEFT\n");
			}
			if(new_pad & PAD_DOWN) {
				red = 0;
				green = 0;
				blue = 255;
				printf("DOWN\n");
			}
			if(new_pad & PAD_RIGHT) {
				printf("RIGHT\n");
				/*
				  padSetMainMode(port, slot,
				  PAD_MMODE_DIGITAL, PAD_MMODE_LOCK));
				*/
			}
			if(new_pad & PAD_UP) {
				printf("UP\n");
			}
			if(new_pad & PAD_START) {
				printf("START\n");
			}
			if(new_pad & PAD_R3) {
				printf("R3\n");
			}
			if(new_pad & PAD_L3) {
				printf("L3\n");
			}
			if(new_pad & PAD_SELECT) {
				printf("SELECT\n");
			}
			if(new_pad & PAD_SQUARE) {
				printf("SQUARE\n");
			}
			if(new_pad & PAD_CROSS) {
				padEnterPressMode(port, slot);
				printf("CROSS - Enter press mode\n");
			}
			if(new_pad & PAD_CIRCLE) {
				padExitPressMode(port, slot);
				printf("CIRCLE - Exit press mode\n");
			}
			if(new_pad & PAD_TRIANGLE) {
				// Check for the reason below..
				printf("TRIANGLE (press mode disabled, see code)\n");
			}
			if(new_pad & PAD_R1) {
				actAlign[0] = 1; // Start small engine
				padSetActDirect(port, slot, actAlign);
				printf("R1 - Start little engine\n");
			}
			if(new_pad & PAD_L1) {
				actAlign[0] = 0; // Stop engine 0
				padSetActDirect(port, slot, actAlign);
				printf("L1 - Stop little engine\n");
			}
			if(new_pad & PAD_R2) {
				red = 0;
				green = 255;
				blue = 0;
				printf("R2\n");
			}
			if(new_pad & PAD_L2) {

				red = 255;
				green = 255;
				blue = 0;
				printf("L2\n");
			}

			// Test the press mode
			if(buttons.triangle_p) {
				printf("TRIANGLE %d\n", buttons.triangle_p);
			}
			// Start little engine if we move right analogue stick right
			if(buttons.rjoy_h > 0xf0)
			{
				// Stupid check to see if engine is already running,
				// just to prevent overloading the IOP with requests
				if (actAlign[0] == 0) {
					actAlign[0] = 1;
					padSetActDirect(port, slot, actAlign);
				}
			}
		}
    
		qword_t *q = buffer;
		q = draw_disable_tests(q, 0, &z);
		q = draw_clear(q, 0, OFFSET - (frame.width / 2), OFFSET - (frame.height / 2), frame.width, frame.height, 20, 20, 20);
		q = draw_enable_tests(q, 0, &z);
		q = draw_triangle(q);
		q = draw_finish(q);

		dma_channel_send_normal(DMA_CHANNEL_GIF, buffer, q - buffer, 0, 0);
		draw_wait_finish();
		graph_wait_vsync();

	}
  
	return 0; 
}
/// @}
