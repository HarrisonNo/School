/*********************************************************************
 *
 * Copyright (C) 2022 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 *
 *********************************************************************/

#include <syscall.h>
#include <stdio.h>

int main (int argc, char *argv[]) 
{
  pid_t c1 = exec("child-simple");
  pid_t c2 = exec("child-simple");
  wait(c1);
  pid_t c3 = exec("child-simple");
  wait(c2);
  wait(c3);
}