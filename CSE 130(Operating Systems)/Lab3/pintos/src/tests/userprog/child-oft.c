/*********************************************************************
 *
 * Copyright (C) 2022 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 *
 *********************************************************************/

#include <stdio.h>
#include "tests/lib.h"

int main (int argc, char *argv[]) 
{
  exit(open(argv[1]));
}