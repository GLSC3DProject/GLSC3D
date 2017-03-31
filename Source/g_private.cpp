#include<stdio.h>
#include<stdlib.h>
#include "glsc3d_private.h"

size_t GLSC3D_Data_Buffer_Size = 1<<20;
void *GLSC3D_Data_Buffer;

void * GLSC3D_Array_Buffer(int Array_Size)
{
    while (Array_Size > GLSC3D_Data_Buffer_Size) {
        GLSC3D_Data_Buffer_Size *= 2;
    }
    if (GLSC3D_Data_Buffer != NULL)
        GLSC3D_Data_Buffer = realloc(GLSC3D_Data_Buffer, GLSC3D_Data_Buffer_Size);
    else
        GLSC3D_Data_Buffer = malloc(GLSC3D_Data_Buffer_Size);
    if (GLSC3D_Data_Buffer == NULL) {
        printf("Memory is ... in GLSC3D_Array_Buffer\n");
        exit(0);
    }
    
    return GLSC3D_Data_Buffer;
}
