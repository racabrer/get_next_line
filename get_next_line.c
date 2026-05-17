#include "get_next_line.h"

static void	gnl_free_and_null(char **ptr)
{

}

static char	*gnl_get_line_from_saved(char *saved)
{
    /*
    Recibe saved.
    Extrae la parte hasta el primer \n y devuelve esa línea (malloc).
    Si no hay \n, devuelve NULL.
    */
}

static char	*gnl_read_and_join(char *saved, int fd)
{
    /*
    Recibe saved[fd] y el fd.
    Llama a read y va añadiendo datos a saved hasta que aparezca \n o EOF.
    Devuelve saved actualizado o NULL si error.
    */
}

char	*get_next_line(int fd)
{ 
    static char *saved[MAX_FD];

    if (!fd || BUFFER_SIZE <= 0)
        return (NULL);
    if (!saved[fd])
    {
        saved[fd] = malloc(sizeof(char) * BUFFER_SIZE + 1);
        if (!saved[fd])
            return (NULL);
        saved[fd][0] = '\0';
    }
    while(saved[fd] != '\n')
        gnl_read_and_join(saved[fd], fd);
    

    
    /*
    Devuelve NULL si hay error o EOF sin más líneas.
    Devuelve una línea (malloc) si todo va bien.
    */
}