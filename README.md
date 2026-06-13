*Este proyecto `ft_printf` ha sido creado como parte del currículo de 42 por `<raqcabre>`.*

# Get Next Line

__Get_next_line__ es un proyecto cuyo objetivo es implementar una función en C capaz de leer, de forma eficiente, una línea completa desde un fichero cada vez que se llama o en el caso de la parte bonus, varios ficheros.

__La función principal del proyecto es:__

    char    *get_next_line(int fd);
Su comportamiento es:

* Devuelve la siguiente línea disponible del descriptor de fichero fd, incluyendo el salto de línea ('\n') si existe.
* Devuelve NULL cuando:
    * Se alcanza el final de archivo (EOF) y no quedan caracteres por devolver.
    * Se produce un error de lectura o de asignación de memoria.
* Debe funcionar:
    * Con cualquier tipo de fichero legible (archivos, stdin, pipes, etc.).
    * Con múltiples descriptores de fichero abiertos de forma simultánea.
    * Con un BUFFER_SIZE configurable en tiempo de compilación.

El objetivo es profundizar en:

 * Manejo de descriptores de fichero y la llamada al sistema read().
 * Gestión de memoria dinámica y fugas de memoria.
 * Manipulación de cadenas y manejo de buffers.
 * Diseño de un algoritmo incremental que conserva estado entre llamadas.

__Instrucciones__

## Estructura del proyecto
(REVISAR ESTA PARTE)

### Compilación:

    CC      = ccCFLAGS  = -Wall -Wextra -WerrorNAME    = gnl_testSRCS    = get_next_line.c get_next_line_utils.c main.cOBJS    = $(SRCS:.c=.o)all: $(NAME)$(NAME): $(OBJS)	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)clean:	rm -f $(OBJS)fclean: clean	rm -f $(NAME)re: fclean all
Para compilar con un BUFFER_SIZE específico:
    cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \   get_next_line.c get_next_line_utils.c main.c -o gnl_test

### Ejecución

Ejemplo leyendo desde un archivo:

    bash

    ./gnl_test file.txt

Ejemplo leyendo desde stdin:

    bash

    ./gnl_testEscribe algunas líneas y pulsa Ctrl+D (Linux/macOS) o Ctrl+Z (Windows + Enter) para EOF
 
Ejemplo de uso:

    #include <fcntl.h>
    #include <stdio.h>
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

## Explicación y justificación del algoritmo

El problema principal es que read():

- No garantiza leer hasta el final de una línea.
- Devuelve como máximo BUFFER_SIZE bytes por llamada.
- No “recuerda” lo que ya se ha leído entre llamadas.

Por ello, get_next_line debe:

- Mantener un buffer estático por descriptor de fichero con los datos excedentes (lo que se leyó de más después del salto de línea).
- Leer en bloques usando read() hasta:
- Encontrar un '\n', o
- Llegar a EOF.

Construir la línea a devolver concatenando:
- Lo que ya había en el buffer estático.
- Lo nuevo que se ha leído.
- Guardar el resto (después del '\n') en el buffer estático para la siguiente llamada.
- Estructura típica del algoritmo
- Estado por descriptor
- Se suele utilizar una variable estática:


static char *stash[FD_MAX]; 
// una "reserva" por fd
o una única static char *stash si la norminette/proyecto exige solo un descriptor, pero el bonus requiere soporte múltiple. El objetivo es que cada fd tenga su propio “resto no procesado”.

__Lectura incremental__

Mientras en stash[fd] no haya '\n' y read() no devuelva 0:

- Reservar un buffer temporal de tamaño BUFFER_SIZE.
- Llamar a read(fd, buffer, BUFFER_SIZE).
- Añadir (join) este bloque a stash[fd].
- De esta forma, se leen mínimos bloques necesarios para completar una línea o determinar EOF.

__Justificación:__ (REVISAR)

- Minimiza el número de llamadas a read().
- Evita leer más de lo necesario.
- Permite gestionar archivos muy grandes sin cargar todo en memoria.
- Extracción de la línea

Una vez que hay '\n' o EOF: (REVISAR)

- Buscar el índice del primer '\n' en stash[fd].
- Si existe '\n':
- Reservar memoria para line desde el inicio de stash[fd] hasta el '\n' incluido.
- Si no existe '\n' (EOF sin salto de línea final):
- Devolver todo el contenido de stash[fd] como última línea.

__Justificación:__ (REVISAR)

- Se garantiza que cada llamada devuelve exactamente una línea o NULL.
- Se conserva el comportamiento esperado para el último fragmento sin salto de línea final.
- Actualización del stash

Tras construir line: (REVISAR)

- Crear una nueva cadena con lo que hay después del '\n'.
- Liberar la antigua stash[fd].
- Asignar la nueva cadena a stash[fd].
- Si no queda nada, stash[fd] se pone a NULL.

__Justificación:__ (REVISAR)

    El contenido ya devuelto no se almacena más.
    Solo se guarda lo justo para la próxima llamada.
    Se reduce el riesgo de fugas de memoria y de acumulación innecesaria.
    Por qué este algoritmo es adecuado

__Eficiencia en memoria:__ (REVISAR)

Solo mantiene en memoria los datos necesarios (el fragmento restante de la última lectura).

Eficiencia en E/S:
Agrupa lecturas en bloques (BUFFER_SIZE), lo que es más eficiente que leer carácter a carácter.

Escalabilidad:
Funciona correctamente incluso con archivos muy grandes porque:

No intenta cargar el archivo entero.
Trabaja por trozos y libera lo que ya no se necesita.
Soporte para múltiples descriptores (en la versión bonus):
Usar un stash por fd permite alternar llamadas a get_next_line sobre diferentes descriptores sin mezclar estados.

Simplicidad relativa:

Aunque implica varias funciones auxiliares (ft_strlen, ft_strchr, ft_strjoin, ft_substr, etc.), la lógica principal es clara:

1. Leer y acumular.
2. Encontrar el salto de línea.
3. Devolver la línea.
4. Guardar el resto.


## Recursos

### Documentación y referencias
- [42 ft_get_next_line](https://42-cursus.gitbook.io/guide/1-rank-01/get_next_line) — Guía general del proyecto de `get_next_line`.
- [42 Get Next Line Guide (String Approach) ](https://42-cursus.gitbook.io/guide/1-rank-01/get_next_line) — entender el proyecto y el funcionamiento del gnl    

### Uso de IA
- Se empleó para validar la estructura del documento, proponer mejoras en la sección de Recursos y asegurar que el contenido respondiera a los requisitos del proyecto.
- No se utilizó IA para implementar la lógica del proyecto ni para sustituir el trabajo de programación.