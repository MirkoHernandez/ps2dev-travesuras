// SPDX-License-Identifier: MIT
/// @file  main.c
/// @brief Leer  y tocar WAV file.
/// @details Además ver que funciones disponibles hay para manipular audio.
/// Código esta basado en rpc/audsrv/samples/playwav.c de PS2SDK.

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <tamtypes.h>

#include <audsrv.h>

/// \defgroup  audio Audio: Lectura de WAV file(v1)
/// @{
/// @details Exactamente igual que para  la lectura de input se cargan
/// modulos IOP.Se necesita cargar audsrv.irx (hay que copiarlo desde PS2SDK).
static void
loadModules(void)
{
	int ret;
	SifInitRpc(0);

	printf("sample: kicking IRXs\n");
	ret = SifLoadModule("rom0:LIBSD", 0, NULL);
	printf("libsd loadmodule %d\n", ret);

	
	printf("DEBUG: loading audsrv\n");
	ret = SifLoadModule("host:audsrv.irx", 0, NULL);
	printf("audsrv loadmodule %d\n", ret);
}

/// @details Se lee WAV, sin  procesar header simplemente se pasa data
/// de  chunk  a  audsrv_play_audio.   Ejemplo  NO  procesa  WAV  file
/// simplemente se mueve posición de archivo y se pasan chunks.
int
main(int argc, char *argv[])
{
	loadModules();
	
	int ret = audsrv_init();
	
	if (ret != 0) {
		printf("sample: failed to initialize audsrv\n");
		printf("audsrv returned error string: %s\n", audsrv_get_error_string());
		return 1;
	}
	
	/// Se crea variable format para  configurar bits per sample y
	/// frecuencia.
	struct audsrv_fmt_t format;
	format.bits = 16;
	format.freq = 48000;
	format.channels = 2;
	int err = audsrv_set_format(&format);
	printf("set format returned %d\n", err);
	printf("audsrv returned error string: %s\n", audsrv_get_error_string());

	int volume = MAX_VOLUME;
	audsrv_set_volume(volume);

	//// test tiene 3 minutos de duracion.
	FILE *wav = fopen("host:test.wav", "rb");
	//// test2 es solo un sonido. 
	FILE *wav2 = fopen("host:test2.wav", "rb");
	char chunk[2048];
	char chunk2[2048];
	
	if (wav == NULL) {
		printf("failed to open wav file\n");
		audsrv_quit();
		return 1;
	}

	// Ubicar file  position indicator  en lugar  APROXIMADO en donde
	// comienza audio. 
	fseek(wav, 0x56, SEEK_SET);
	fseek(wav2, 0x56, SEEK_SET);
	
	printf("starting play loop\n");
	int chunk_data_read = 0;
	while (1) {
		ret = fread(chunk, 1, sizeof(chunk), wav);
		if (ret > 0) {
			
			audsrv_wait_audio(ret);
			audsrv_play_audio(chunk, ret);
		}

		//// Probando como funciona ajustar el volumen y tocar
		//// un segundo sonido.
		chunk_data_read +=ret;
		if (chunk_data_read > 600000) {
			volume -= 1;
			audsrv_set_volume(volume);

			int ret2 = fread(chunk2, 1, sizeof(chunk2), wav2);
			if (ret2 > 0) {
				audsrv_wait_audio(ret2);
				audsrv_play_audio(chunk2, ret2);
			}
			
		}
		
		if (chunk_data_read > 1100000) {
			volume = volume < 1 ? 1: volume;
			volume += 2;
			volume = volume > 100 ? 100 : volume;
			printf("volume %d \n", volume);
			audsrv_adpcm_set_volume(2,volume);
		}
		
		if (ret < sizeof(chunk)) {
			printf("DEBUG:NO more data\n");
			/* no more data */
			break;
		}

	}

	fclose(wav);

	printf("sample: stopping audsrv\n");
	audsrv_quit();

	printf("sample: ended\n");
	return 0;
}

/// @}
