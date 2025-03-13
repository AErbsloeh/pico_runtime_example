#ifndef TESTBENCH_H_
#define TESTBENCH_H_


#include <stdio.h>
#include "hardware/gpio.h"


// =============================== DEFINITIONS ===============================
#define TB_MODE_NONE    0x00
#define TB_MODE_MUX     0x01


// =============================== FUNCTIONS ===============================
/*! \brief Function for choosing a testbench mode
* \param mode   Unsigned integer for selecting the right testmode
* \param chnnl  Used channel of the CLARiSTOR processing channels
* \return       Boolean if test is done
*/
bool run_testbench(uint8_t mode, uint8_t chnnl);


#endif
