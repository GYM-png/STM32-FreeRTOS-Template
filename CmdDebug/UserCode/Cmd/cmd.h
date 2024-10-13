#ifndef __CDM_H
#define __CDM_H

#include "main.h"

// typedef struct
// {
//     void (*print)(const uint8_t * data, uint8_t len);
// }print_t


typedef struct
{
    uint16_t index;
    char * cmd;
    void (*func)(void);
}cmd_t;

typedef uint8_t (*debugPrint_f)(uint8_t * , uint16_t );

void my_printf(const char *format, ...); 

void cmdInit();
void findCommand(char * cmd);



#endif
