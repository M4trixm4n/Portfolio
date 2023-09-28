#include "triangles_plans.h"

void generate_tri_planes_image(Image *im)
{
    int i, j, k;
    double t, tempT;
    allocate_pixels(im);
    if (!(im)->pixels)
        return;

    bool isTriangle;
    Triangle tr;
    Plane pl;

    Triplet *ortho = orthonormal(im), interPoint, color, d;
    Doublet size = pixel_size(im);

    for (i = 0; i < im->width; i += 1)
    {
        for (j = 0; j < im->height; j += 1)
        {
            d = pixel_direction_vector(im, i, j, ortho, size);
            t = -1;
            isTriangle = true;
            for (k = 0; k < im->obj->nbTriangles; k += 1)
            {
                tempT = distance_to_triangle(im->obj->triangles[k], im, d, im->cam.from);
                if (tempT == -1)
                    continue;
                if (tempT < t || t == -1)
                {
                    t = tempT;
                    tr = im->obj->triangles[k];
                }
            }
            for (k = 0; k < im->obj->nbPlanes; k += 1)
            {
                tempT = distance_to_plane(im->obj->planes[k], im, d, im->cam.from);
                if (tempT == -1)
                    continue;
                if (tempT < t || t == -1)
                {
                    t = tempT;
                    isTriangle = false;
                    pl = im->obj->planes[k];
                }
            }

            Triplet n;

            interPoint = intersection_point(im->cam.from, d, t);
            if (t != -1)
            {
                if (isTriangle)
                {
                    n = norm(vector_product(substraction(tr.y, tr.x), substraction(tr.z, tr.x)));
                    color = tr.color;
                }
                else
                {
                    n = pl.normal;
                    color = pl.color;
                }
                color = coul(im, n, color, interPoint, create_obstructed_lights(im));
            }
            else
            {
                color = (Triplet){0, 0, 0};
            }
            im->pixels[i][j].x = color.x * 255;
            im->pixels[i][j].y = color.y * 255;
            im->pixels[i][j].z = color.z * 255;
        }
    }

    if (ortho)
        free(ortho);
}

void check_shadow_spheres(Triplet interPoint, Image *im, int **obstructedLights, Sphere s)
{
    double t;
    Triplet d;
    Sphere s2;

    for (int i = 0; i < im->lights->nbDirectionals; i += 1)
    {
        t = min_dist_to_sphere(im, norm(im->lights->directionals[i].vector), &s2, interPoint, s);
        if (t != -1 && !equals_sphere(s, s2) && t > 0)
            goto nextD;
        continue;
    nextD:
        obstructedLights[0][i] = 0;
    }

    for (int i = 0; i < im->lights->nbPoints; i += 1)
    {
        d = norm(vector_from_to(interPoint, im->lights->points[i].point));
        t = min_dist_to_sphere(im, d, &s2, interPoint, s);
        if (t != -1 && !equals_sphere(s, s2) && t > 0)
            goto nextP;
        continue;
    nextP:
        obstructedLights[1][i] = 0;
    }
}

void check_shadow_triangles(Triplet interPoint, Image *im, int **obstructedLights, Triangle tr)
{
    double t;
    Triplet d;
    int j;

    for (int i = 0; i < im->lights->nbDirectionals; i += 1)
    {
        for (j = 0; j < im->obj->nbTriangles; j += 1)
        {
            t = distance_to_triangle(im->obj->triangles[j], im, norm(((Triplet) {-im->lights->directionals[i].vector.x, -im->lights->directionals[i].vector.y, -im->lights->directionals[i].vector.z, 1})), interPoint);
            if (t != -1 && !equals_triangle(tr, im->obj->triangles[j]) && t > 0)
                goto nextD;
        }
        continue;
    nextD:
        obstructedLights[0][i] = 0;
    }

    for (int i = 0; i < im->lights->nbPoints; i += 1)
    {
        d = norm(vector_from_to(im->lights->points[i].point, interPoint));
        d = scalar_multiplication(d, -1);
        for (j = 0; j < im->obj->nbTriangles; j += 1)
        {
            t = distance_to_triangle(im->obj->triangles[j], im, d, interPoint);
            if (t != -1 && !equals_triangle(tr, im->obj->triangles[j]) && t > 0)
                goto nextP;
        }
        continue;
    nextP:
        obstructedLights[1][i] = 0;
    }
}

void apply_shadow (Image *im, Triplet interPoint, int **obstructedLights, Sphere s, Triangle tr) {
    if (im->data->shadow) {
        check_shadow_spheres(interPoint, im, obstructedLights, s);
        check_shadow_triangles(interPoint, im, obstructedLights, tr);
    }
}
