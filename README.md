# ps2dev travesuras

Programas que hacen uso de PS2SDK. El objetivo es obtener una buena
documentación de PS2SDK.

## Sobre uso de Doxygen

Los programas están comentados con docstrings de Doxygen. Se utiliza
el siguiente comando para generar la documentación.

```bash
doxygen Doxyfile
```

También hay algunos documentos en markdown que se pueden leer
directamente (en directorio docs) sin la necesidad de tener Doxygen
instalado.

## Recomendación para navegar código

La documentación de PS2SDK es escasa, inevitablemente es necesario
navegar el código de las librerías. Una solución simple para ese
problema es utilizar gtags.

Suponiendo que el código de PS2SDK esta en ~/.local/share, obtener
tags en ese directorio con el siguiente comando.

```bash
cd ~/.local/share/ps2sdk 
gtags .
```
Luego se puede exportar GTAGSLIBPATH para obtener acceso a esos tags.

```bash
export GTAGSLIBPATH=$HOME/.local/share/ps2sdk
export GTAGSTHROUGH=true 
```

Se debe utilizar global con la opción -T para encontrar tags en
GTAGSLIBPATH. La variable GTAGSTHROUGH permite usar esa opción por
defecto; puede tener cualquier valor (se le asigna true en el
ejemplo).

Para conveniencia se incluye un archivo envrc de muestra con estas
variables ya configuradas.

# Recursos PS2DEV

* [PS2DEV](https://github.com/ps2dev) Herramientas homebrew para PS2.
* [PS2SDK samples](https://github.com/ps2dev/ps2sdk) PS2SDK tiene varios directorios samples, no solo uno.
* [Lukasz D.K.](https://github.com/lukaszdk) Tutoriales, incluidos los de HS Fortuna (Muy buenos pero no refieren a PS2SDK).
* [Guilherme Lampert](https://glampert.com) Excelente artículo sobre PS2 hardware.
* [Tom Marks](https://github.com/phy1um/ps2-homebrew-livestreams) Streams en donde se programan gráficos sin el uso de algunas librerías de PS2SDK.
* [ps2tek](https://psi-rockin.github.io/ps2tek/#gs) Unica documentación disponible de formatos de data (que pude encontrar).
* [gamedev.net](https://gamedev.net/reference/articles/article2027.asp) Buen artículo sobre una posible abstracción de rendering code.
* [forums.ps2dev.org](https://forums.ps2dev.org/viewtopic.php?t=2842&highlight=clock+tick) Post sobre uso de registros para timer (esencial para poder entender registros de timer).
* [Tyra](https://github.com/h4570/tyra) Tyra Game Engine.
* [PS2DEV-World](https://github.com/playstation2-development/ps2dev-world) Repositorio que lista decenas de recursos para PS2DEV.
