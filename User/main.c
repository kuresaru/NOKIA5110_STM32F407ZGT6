#include "stm32f4xx.h"
#include "nokia5110.h"

int main() {
  NOKIA5110_Init();

  NOKIA5110_SetPos(0, 0);
  NOKIA5110_WriteData(0xFF);

  NOKIA5110_SetPos(2, 0);
  NOKIA5110_WriteData(0xFF);

  NOKIA5110_SetPos(0, 2);
  NOKIA5110_WriteData(0xFF);

  while (1);
}
