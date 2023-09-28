#include "generator.h"

void generate_image(Image *im)
{
    int i, j;

    int width = im->width;
    int height = im->height;

    if (!im->pixels)
    {
        goto end;
    }

    FreeImage_Initialise(FALSE);

    RGBQUAD color;

    FIBITMAP *image = FreeImage_Allocate(width, height, 24, 0, 0, 0);

    if (!image)
    {
        set_error(ERROR_ALLOCATION_FAILED, im, "Erreur lors de l'allocation de l'image (FIBITMAP FreeImage)");
        goto end;
    }

    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {
            color.rgbRed = (im->pixels)[i][j].x;
            color.rgbGreen = (im->pixels)[i][j].y;
            color.rgbBlue = (im->pixels)[i][j].z;
            FreeImage_SetPixelColor(image, i, j, &color);
        }
    }

    if (!FreeImage_Save(FIF_PNG, image, im->name, 0))
    {
        set_error(ERROR_SAVING_FAILED, im, "Erreur lors de la sauvegarde de l'image via FreeImage");
        goto end;
    }

end:
    FreeImage_Unload(image);
    FreeImage_DeInitialise();
}

Image *generate_all(char *filename)
{
    Image *im = parse_start(filename);

    int i, j, k;
    double t, tempT;
    allocate_pixels(im);
    if (!(im)->pixels)
        return im;

    int struc; // 0 sphere, 1 triangle, 2 plane
    Triangle tr;
    Plane pl;
    Sphere s;

    Triplet *ortho = orthonormal(im), interPoint, color, d;
    Doublet size = pixel_size(im);

    for (i = 0; i < im->width; i += 1)
    {
        for (j = 0; j < im->height; j += 1)
        {
            d = pixel_direction_vector(im, i, j, ortho, size);
            t = -1;
            struc = 0;

            tempT = min_dist_to_sphere(im, d, &s, im->cam.from, (Sphere) {(Triplet){0, 0, 0, 0}, 0, (Triplet){-1, -1, -1, 2}});
            if ((tempT < t || t == -1) && tempT > 0)
            {
                t = tempT;
                struc = 0;
            }

            for (k = 0; k < im->obj->nbTriangles; k += 1)
            {
                tempT = distance_to_triangle(im->obj->triangles[k], im, d, im->cam.from);
                if (tempT <= 0)
                    continue;
                if (tempT < t || t == -1)
                {
                    t = tempT;
                    struc = 1;
                    tr = im->obj->triangles[k];
                }
            }

            for (k = 0; k < im->obj->nbPlanes; k += 1)
            {
                tempT = distance_to_plane(im->obj->planes[k], im, d, im->cam.from);
                if (tempT <= 0)
                    continue;
                if (tempT < t || t == -1)
                {
                    t = tempT;
                    struc = 2;
                    pl = im->obj->planes[k];
                }
            }

            if (!struc) {
                tr.color.x = -1;
                pl.color.x = -1;
            } else if (struc == 1) {
                s.color.x = -1;
                pl.color.x = -1;
            } else {
                s.color.x = -1;
                tr.color.x = -1;
            }

            interPoint = intersection_point(im->cam.from, d, t);
            int **obstructedLights = create_obstructed_lights(im);

            apply_shadow(im, interPoint, obstructedLights, s, tr);
            color = pixel_color(struc, interPoint, s, tr, pl, obstructedLights, im, t);
            
            free_obstructed_lights(obstructedLights);
            im->pixels[i][j].x = color.x * 255.0 + 0.5;
            im->pixels[i][j].y = color.y * 255.0 + 0.5;
            im->pixels[i][j].z = color.z * 255.0 + 0.5;
        }
    }
    free(ortho);
    return im;
}

void test_scene(Image *im)
{
    printf("%s\n", im->name);
    printf("%d\n", im->height * im->width);
    printf("%d\n", im->obj->nbPlanes + im->obj->nbSpheres + im->obj->nbTriangles);
    printf("%d\n", im->lights->nbDirectionals + im->lights->nbPoints);
}
