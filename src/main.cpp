/**
 * @file main.cpp
 * @author Juan Burgos
 * @brief
 * @version 0.1
 * @date 2021-11-07
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <cstdio>

#include "headers.hpp"

/**
 * @brief 
 * 
 * @return int 
 */
int main(void)
{
  SDL_SetMainReady();
  WindowManager mgr;

  int rc = mgr.init();
  if (rc != 0)
  {
    fprintf(stderr, "Failed to initialize program!\n");
    return rc;
  }
  mgr.start();

  return 0;
}
