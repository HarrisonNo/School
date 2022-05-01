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
  pid_t c1 = exec("child-corwc 1");
  pid_t c2 = exec("child-corwc 2 2");
  pid_t c3 = exec("child-corwc 3 3 3");
  int r1 = wait(c1);
  int r2 = wait(c2);
  pid_t c4 = exec("child-corwc 4 4 4 4");
  int r3 = wait(c3);
  int r4 = wait(c4);
  pid_t c5 = exec("child-corwc 5 5 5 5 5");
  int r5 = wait(c5);
  pid_t c6 = exec("child-corwc 6 6 6 6 6 6");
  int r6 = wait(c6);
  msg("c1 => %d", r1);
  msg("c2 => %d", r2);
  msg("c3 => %d", r3);
  msg("c4 => %d", r4);
  msg("c5 => %d", r5);
  msg("c6 => %d", r6);
}
