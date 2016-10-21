/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Header that contains all strings used in the User interface
 */

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

#define STR_FILE_SYSTEM_TITLE        "\n                    ******* FILE SYSTEM *******\n\n"
#define STR_SELECT_OPTION            "Selecciona la opcion deseada:\n\n"
#define STR_MAIN_MENU_OPTIONS        "1.-  Crear un archivo\n" \
                                     "2.-  Abrir un archivo\n" \
                                     "3.-  Cerrar un archivo\n" \
                                     "4.-  Borrar un archivo\n"   \
                                     "5.-  Escribir a un archivo\n"   \
                                     "6.-  Leer de un archivo\n"  \
                                     "7.-  Obtener puntero de lectura (tell)\n" \
                                     "8.-  Establecer puntero de lectura (seek)\n" \
                                     "9.-  Imprimir un archivo en la consola\n" \
                                     "10.- Imprimir todos los archivos en la consola\n" \
                                     "11.- Salir\n\n"
#define STR_OPTION_SELECTED          "Opcion Seleccionada: "
#define STR_SUCCESS                  "\nOperacion exitosa!!!\n"
#define STR_FAIL                     "\nHubo un ERROR mientras se ejecutaba el programa\n"
#define STR_FILE_NOT_FOUND           "\nArchivo no encontrado\n"
#define STR_FILE_ALREADY_EXIST       "\nEl archivo ya existe\n"
#define STR_FILE_IS_ALREADY_OPENED   "\nEl archivo ya habia sido abierto\n"
#define STR_FILE_IS_ALREADY_CLOSED   "\nEl archivo ya habia sido cerrado\n"
#define STR_THERE_ARE_NOT_FILES      "\nNo hay archivos por imprimir\n"
#define STR_GET_FILE_NAME            "\nDame el nombre del archivo: "
#define STR_WRITE_FILE               "\nIntroduce la informacion que se escribira en el archivo\n\n"
#define STR_READ_FILE                "\nIntroduce el numero de bytes a leer del archivo: "
#define STR_TELL_FILE                "\nPosicion de lectura del archivo: %d"
#define STR_SEEK_FILE                "\nIntroduce la nueva posicion de lectura del archivo: "
#define STR_PRINT_DATA               "\nDeseas imprimir los datos del archivo[s, n]: "
#define STR_FILE_NAME                "\nNombre del archivo: %s"
#define STR_FILE_DATA                "\nInformacion del archivo\n\n%s"
#define STR_FILE_DATE                "\nFecha de creacion: %s"
#define STR_FILE_SIZE                "\nTamaño del archivo: %d"
#define STR_FILE_R_POINT             "\nPuntero de lectura: %d"
#define STR_FILE_W_POINT             "\nPuntero de escritura: %d"
#define STR_FILE_IS_OPENED           "\nEl archivo esta abierto: %s\n"
#define STR_YES                      "Si"
#define STR_NO                       "No"

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif
