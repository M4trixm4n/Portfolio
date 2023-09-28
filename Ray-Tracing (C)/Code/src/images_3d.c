#include "images_3d.h"

void generate_3d_image(Image *im)
{
    int i, j;
    double t;
    allocate_pixels(im);
    if (!(im)->pixels)
        return;
    Sphere s;

    Triplet *repere = orthonormal(im), interPoint, normalInterPoint, color;
    Doublet size = pixel_size(im);

    for (i = 0; i < im->width; i++)
    {
        for (j = 0; j < im->height; j++)
        {
            t = min_dist_to_sphere(im, pixel_direction_vector(im, i, j, repere, size), &s, im->cam.from,  (Sphere) {(Triplet){0, 0, 0, 0}, 0, (Triplet){-1, -1, -1, 2}});
            if (t == -1.0)
                continue;
            interPoint = intersection_point(im->cam.from, pixel_direction_vector(im, i, j, repere, size), t);
            normalInterPoint = normal_interpoint_sphere(interPoint, s.center);
            color = coul(im, normalInterPoint, s.color, interPoint, create_obstructed_lights(im));
            im->pixels[i][j].x = color.x * 255;
            im->pixels[i][j].y = color.y * 255;
            im->pixels[i][j].z = color.z * 255;
        }
    }

    if (repere)
    {
        free(repere);
    }
}
