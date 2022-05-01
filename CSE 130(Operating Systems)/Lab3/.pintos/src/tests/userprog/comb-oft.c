/*********************************************************************
 *
 * Copyright (C)2022 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 *
 *********************************************************************/

/*
 * The first files opened by mutiple processes should have 
 * file descriptors of 3 as 0,1,2 are reserved for stdin,
 * stdout, and stderr.
 */

#include <syscall.h>
#include "tests/lib.h"
#include "tests/main.h"

void
test_main (void) 
{
  create("oft1.txt", 32);
  create("oft2.txt", 32);
  create("oft3.txt", 32);
  create("oft4.txt", 32);
  create("oft5.txt", 32);
  create("oft6.txt", 32);
  pid_t c1 = exec("child-oft oft1.txt");
  pid_t c2 = exec("child-oft oft2.txt");
  pid_t c3 = exec("child-oft oft3.txt");
  int r1 = wait(c1);
  int r2 = wait(c2);
  pid_t c4 = exec("child-oft oft4.txt");
  int r3 = wait(c3);
  int r4 = wait(c4);
  pid_t c5 = exec("child-oft oft5.txt");
  int r5 = wait(c5);
  pid_t c6 = exec("child-oft oft6.txt");
  int r6 = wait(c6);
  msg("fie decriptors%smatch", 
    r1 >= 3 && r2 >= 3 && r3 >= 3 && r4 >= 3 && r5 >= 3 && r6 >= 3 && 
    r1 == r2 && r2 == r3 && r3 == r4 && r4 == r5 && r5 == r6 ? " " : " DO NOT ");
}