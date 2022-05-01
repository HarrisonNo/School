/*********************************************************************
 *
 * Copyright (C) 2022 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 *
 *********************************************************************/

#include <syscall.h>
#include "tests/lib.h"
#include "tests/main.h"

void
test_main (void) 
{
  pid_t c1 = exec("child-nested");
  pid_t c2 = exec("child-nested");
  wait(c1);
  pid_t c3 = exec("child-nested");
  wait(c2);
  wait(c3);
}