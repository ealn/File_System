/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: - Adrian Luna
 *
 * Porpuse: Header that contains the functions of User Interface
 */
#include <stdint.h>

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

int32_t showUI(void);
void showFileInfo(char *   name,
                  char *   data,
                  char *   date,
                  uint32_t size,
                  uint32_t w_point,
                  uint32_t r_point,
                  bool     is_opened,
                  bool     showData);

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif
