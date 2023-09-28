#include "comparateur_images.h"

#include "error_handler.h"

int initialise_freeimage_components(FIBITMAP **image_1, FIBITMAP **image_2, FIBITMAP **image, char *file_1, char *file_2)
{
    *image_1 = FreeImage_Load(FIF_PNG, file_1, PNG_DEFAULT);
    if (!*image_1)
    {
        set_error(ERROR_LOADING_FAILED, NULL, "La première image n'a pas pu être chargée avec FreeImage");
        return 0;
    }
    unsigned int width = FreeImage_GetWidth(*image_1);
    unsigned int height = FreeImage_GetHeight(*image_1);
    *image_2 = FreeImage_Load(FIF_PNG, file_2, PNG_DEFAULT);
    if (!image_2)
    {
        set_error(ERROR_LOADING_FAILED, NULL, "La deuxième image n'a pas pu être chargée avec FreeImage");
        return 0;
    }
    if (width != FreeImage_GetWidth(*image_2) || height != FreeImage_GetHeight(*image_2))
    {
        set_error(ERROR_DIFFERENT_SIZE, NULL, "Les tailles des images données pour comparaison sont différentes l'une de l'autre.");
        return 0;
    }
    *image = FreeImage_Allocate(width, height, FreeImage_GetBPP(*image_1), 0, 0, 0);
    if (!image)
    {
        set_error(ERROR_ALLOCATION_FAILED, NULL, "Erreur lors de l'allocation de l'image (FIBITMAP FreeImage)");
        return 0;
    }
    return 1;
}

void set_color(RGBQUAD *color, RGBQUAD color_1, RGBQUAD color_2)
{
    color->rgbBlue = color_1.rgbBlue - color_2.rgbBlue < 0 ? color_2.rgbBlue - color_1.rgbBlue : color_1.rgbBlue - color_2.rgbBlue;
    color->rgbGreen = color_1.rgbGreen - color_2.rgbGreen < 0 ? color_2.rgbGreen - color_1.rgbGreen : color_1.rgbGreen - color_2.rgbGreen;
    color->rgbRed = color_1.rgbRed - color_2.rgbRed < 0 ? color_2.rgbRed - color_1.rgbRed : color_1.rgbRed - color_2.rgbRed;
}

void compare(char *file_1, char *file_2)
{
    // Counter for the number of different pixels
    int counter = 0;
    RGBQUAD color, color_1, color_2;
    FIBITMAP *image_1, *image_2, *image;

    FreeImage_Initialise(FALSE);

    if (!initialise_freeimage_components(&image_1, &image_2, &image, file_1, file_2))
        goto end;

    unsigned int width = FreeImage_GetWidth(image_1);
    unsigned int height = FreeImage_GetHeight(image_1);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            FreeImage_GetPixelColor(image_1, i, j, &color_1);
            FreeImage_GetPixelColor(image_2, i, j, &color_2);
            set_color(&color, color_1, color_2);
            if (color.rgbBlue != 0 || color.rgbGreen != 0 || color.rgbRed != 0)
                counter += 1;
            FreeImage_SetPixelColor(image, i, j, &color);
        }
    }

    if (!FreeImage_Save(FIF_PNG, image, "diff.png", 0))
    {
        set_error(ERROR_SAVING_FAILED, NULL, "Erreur lors de la sauvegarde de l'image via FreeImage");
        goto end;
    }

    // If there are less than 1000 different pixels, we consider that the images are the same
    if (counter <= 1000)
    {
        printf("OK\n%d\n", counter);
    }
    else
    {
        printf("KO\n%d\n", counter);
    }

end:
    FreeImage_Unload(image_1);
    FreeImage_Unload(image_2);
    FreeImage_Unload(image);
    FreeImage_DeInitialise();
}
