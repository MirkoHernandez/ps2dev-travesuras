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
directamente sin la necesidad de tener Doxygen instalado.

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
export GTAGSLIBPATH=true 
```
Se debe utilizar global con la opción -T para encontrar tags en
GTAGSLIBPATH. GTAGSTHROUGH permite usar esa opción por defecto. 

# Recursos PS2DEV

* [PS2DEV](https://github.com/ps2dev) Herramientas homebrew para PS2.
* [Lukasz D.K.](https://github.com/lukaszdk) Tutoriales, incluidos los de HS Fortuna.
* [Guilherme Lampert](https://github.com/glampert) Excelente artículo sobre PS2 hardware.
* [Tom Marks](https://github.com/phy1um/ps2-homebrew-livestreams) Streams en donde se programan gráficos sin el uso de algunas librerías de PS2SDK.
* [ps2tek](https://psi-rockin.github.io/ps2tek/#gs) Única documentación disponible de formatos de data.
* [gamedev.net](https://gamedev.net/reference/articles/article2027.asp) Buen articulo sobre una posible abstracción de rendering code.
