# get_next_line

*Este proyecto ha sido realizado como parte del currículo de 42 por raqcabre.*

## Qué hace el proyecto

`get_next_line` es una función en C que lee una línea completa desde un file descriptor cada vez que se llama.  
Su objetivo es ir devolviendo el contenido del archivo poco a poco, línea a línea, sin cargarlo entero en memoria.

### Prototipo

```c
char    *get_next_line(int fd);
```

### Qué devuelve

La función devuelve la siguiente línea disponible del descriptor `fd`, incluyendo el salto de línea `\n` si existe.

Devuelve `NULL` cuando:

- se llega al final del archivo y ya no queda nada por leer;
- ocurre un error de lectura o un fallo al reservar memoria.

### Dónde funciona

Esta función debe funcionar con:

- archivos normales;
- entrada estándar (`stdin`);
- pipes;
- cualquier descriptor que se pueda leer.

En la parte bonus, también debe poder trabajar con varios descriptores al mismo tiempo.

## Cómo funciona

La clave del proyecto es que `read()` no devuelve una línea completa, sino solo trozos de texto.  
Por eso, la función tiene que leer por bloques hasta encontrar un `\n` o hasta llegar al final del archivo.

La idea es sencilla:

1. Guardar en una variable estática lo que sobra de la lectura anterior.
2. Leer más datos con `read()` en bloques de tamaño `BUFFER_SIZE`.
3. Ir juntando lo nuevo con lo que ya había guardado.
4. Cuando encuentra un salto de línea, separar la línea para devolverla y guardar el resto para la siguiente llamada.
5. Si se llega al EOF, devolver lo último que quede almacenado.

Este enfoque usa solo la memoria necesaria y evita leer carácter por carácter.  
Además, permite trabajar con archivos grandes sin intentar cargar todo el contenido de golpe.

En la versión bonus, usar un almacenamiento distinto para cada `fd` permite alternar entre varios archivos sin mezclar sus datos.

## Compilación

### Con Makefile

```make
CC      = cc
CFLAGS  = -Wall -Wextra -Werror
NAME    = gnl_test

SRCS    = get_next_line.c get_next_line_utils.c main.c
OBJS    = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
```

### Compilando con BUFFER_SIZE

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 
get_next_line.c get_next_line_utils.c main.c -o gnl_test
```

## Ejecución

### Leyendo desde un archivo

```bash
./gnl_test file.txt
```

### Leyendo desde stdin

```bash
./gnl_test
```

Después puedes escribir líneas y pulsar `Ctrl+D` en Linux/macOS o `Ctrl+Z` y luego `Enter` en Windows para terminar la entrada.

## Ejemplo de uso

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int main(void)
{
    int     fd;
    char    *line;

    fd = open("file.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    line = get_next_line(fd);
    while (line)
    {
        printf("%s", line);
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    return (0);
}
```

## Recursos

### Documentación y referencias
- [42 ft_get_next_line](https://42-cursus.gitbook.io/guide/1-rank-01/get_next_line) — Guía general del proyecto de `get_next_line`.
- [42 Get Next Line Guide (String Approach) ](https://42-cursus.gitbook.io/guide/1-rank-01/get_next_line) — entender el proyecto y el funcionamiento del gnl    

### Uso de IA
- Se empleó para validar la estructura del documento, proponer mejoras en la sección de Recursos y asegurar que el contenido respondiera a los requisitos del proyecto.
- No se utilizó IA para implementar la lógica del proyecto ni para sustituir el trabajo de programación.