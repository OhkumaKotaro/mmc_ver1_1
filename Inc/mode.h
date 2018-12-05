#ifndef __MODE_H
#define __MODE_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"


int8_t Mode_select(void);
void Mode_mouse(int8_t mode);
void Mode_Adachi(void);
void Mode_Circuit(void);


#ifdef __cplusplus
}
#endif
#endif