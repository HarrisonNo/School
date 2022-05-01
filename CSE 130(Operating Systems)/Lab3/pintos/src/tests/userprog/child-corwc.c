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
  char name[32];
  snprintf(name, 32, "hello.txt.%s", argv[1]);
  bool flag = create(name, 128);
  int fd = open(name);
  int w = write(fd, "hello", 6);
  char buf[6];
  int r = read(fd, buf, 6);
  close(fd);
  return flag && fd >= 3 && w == 6 && r == 6 ? argc -1 : -1;
}