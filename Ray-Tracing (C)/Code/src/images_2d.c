#include "images_2d.h"

#include "algebraic_formulas.h"

void generate_2d_image(Image **im)
{
    int i, j;
    allocate_pixels(*im);
    if (!(*im)->pixels)
        return;

    // calculation of the benchmark

    Triplet *repere = orthonormal(*im);
    Sphere s;

    // calculation of the pixels dimension in the benchmark

    Doublet size = pixel_size(*im);

    for (i = 0; i < (*im)->width; i++)
    {
        for (j = 0; j < (*im)->height; j++)
        {
            double dist = min_dist_to_sphere(*im, pixel_direction_vector(*im, i, j, repere, size), &s, (*im)->cam.from,  (Sphere) {(Triplet){0, 0, 0, 0}, 0, (Triplet){-1, -1, -1, 2}});
            // calcul couleur
            if (dist >= 0)
            {
                ((*im)->pixels)[i][j].x = (*im)->data->ambient.x * 255.0;
                ((*im)->pixels)[i][j].y = (*im)->data->ambient.y * 255.0;
                ((*im)->pixels)[i][j].z = (*im)->data->ambient.z * 255.0;
            }
        }
    }
    if (repere)
    {
        free(repere);
    }
}
