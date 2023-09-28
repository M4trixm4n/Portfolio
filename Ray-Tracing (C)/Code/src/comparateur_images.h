#ifndef COMPARATEUR_IMAGES_H

#define COMPARATEUR_IMAGES_H

#include <FreeImage.h>

#include <stdio.h>

#include <stdlib.h>

/**
 * @brief Compare two images in PNG format, and create a new one (each pixel is the absolute value of the substraction of the two image's pixel)
 *
 * @param file_1 The name of the first file
 * @param file_2 The name of the second file
 */
void compare(char *file_1, char *file_2);

#endif
