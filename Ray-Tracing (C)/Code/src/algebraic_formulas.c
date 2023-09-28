#include "algebraic_formulas.h"

Triplet *orthonormal(Image *image)
{
    Triplet u, v, w;
    w = norm(substraction(image->cam.from, image->cam.at));
    u = norm(vector_product(image->cam.up, w));
    v = norm(vector_product(w, u));
    Triplet *res = (Triplet *)malloc(sizeof(Triplet) * 3);
    u.type = 1;
    res[0] = u;
    v.type = 1;
    res[1] = v;
    w.type = 1;
    res[2] = w;
    return res;
}

double radian_fov(double fov)
{
    return (fov * M_PI) / 180.0;
}

Doublet pixel_size(Image *image)
{
    Doublet size = {0.0, 0.0};

    if (!image->width || !image->height)
        goto errorUntimelyCall;
    if (!image->cam.fov)
        goto errorInvalidValue;

    double fovr = radian_fov((double)image->cam.fov);
    size.y = tan(fovr / 2.0);
    size.x = size.y * ((double)image->width / (double)image->height);

    return size;

errorUntimelyCall:
    set_error(ERROR_UNTIMELY_CALL, image, "Impossible d'appeler pixel_size avant que la taille de l'image ne soit définie.");
    return size;
errorInvalidValue:
    set_error(ERROR_INVALID_VALUE, image, "Rencontre d'une valeur non définie dans pixel_size : image->cam.fov (FOV).");
    return size;
}

Triplet pixel_direction_vector(Image *image, int i, int j, Triplet *ortho, Doublet size)
{
    double a = (size.x * ((double)i - ((double)image->width / 2) + 0.5)) / ((double)image->width / 2);
    double b = (size.y * ((double)j - ((double)image->height / 2) + 0.5)) / ((double)image->height / 2);
    Triplet d = norm(substraction(addition(scalar_multiplication(ortho[0], a), scalar_multiplication(ortho[1], b)), ortho[2]));
    d.type = 1;
    return d;
}

void min_dis_to_sphere_dis_0(double *minDist, Sphere *s, Image *image, int k, double t)
{
    if (*minDist < 0.0)
    {
        *minDist = t;
        *s = (image->obj->spheres)[k];
    }
    else if (*minDist >= 0.0 && t < *minDist)
    {
        *minDist = t;
        *s = (image->obj->spheres)[k];
    }
}

void min_dis_to_sphere_not_dis_0(double t1, double t2, Sphere *s, int k, double *minDist, Image *image)
{
    if (t2 >= 0)
    {
        if (*minDist < 0.0)
        {
            *minDist = t2;
            *s = (image->obj->spheres)[k];
        }
        else if (*minDist >= 0.0 && t2 < *minDist)
        {
            *minDist = t2;
            *s = (image->obj->spheres)[k];
        }
    }
    else if (t1 >= 0)
    {
        if (*minDist < 0.0)
        {
            *minDist = t1;
            *s = (image->obj->spheres)[k];
        }
        else if (*minDist >= 0.0 && t1 < *minDist)
        {
            *minDist = t1;
            *s = (image->obj->spheres)[k];
        }
    }
}

double min_dist_to_sphere(Image *image, Triplet d, Sphere *s, Triplet eye, Sphere self)
{
    double a_eq, b_eq, c_eq, dis, t, t1, t2, minDist = -1.0;

    for (int k = 0; k < image->obj->nbSpheres; k += 1)
    {
        a_eq = length(d) * length(d);
        b_eq = scalar_product(scalar_multiplication(substraction(eye, image->obj->spheres[k].center), 2.0), d);
        c_eq = scalar_product(substraction(eye, image->obj->spheres[k].center), substraction(eye, image->obj->spheres[k].center)) - (image->obj->spheres[k].radius * image->obj->spheres[k].radius);
        dis = (b_eq * b_eq) - (4.0 * a_eq * c_eq);
        if (dis == 0)
        {
            t = (-1 * b_eq) / (2.0 * a_eq);

            min_dis_to_sphere_dis_0(&minDist, s, image, k, t);
        }
        else if (dis > 0)
        {
            t2 = ((-1 * b_eq) - sqrt(dis)) / (2.0 * a_eq);
            if (t2 <= 0 && equals_sphere(self, image->obj->spheres[k]))
                continue;
            t1 = ((-1 * b_eq) + sqrt(dis)) / (2.0 * a_eq);

            min_dis_to_sphere_not_dis_0(t1, t2, s, k, &minDist, image);
        }
    }
    return minDist;
}

double distance_to_plane(Plane p, Image *image, Triplet d, Triplet eye)
{
    double denominator = scalar_product(d, p.normal);
    if (!denominator)
        return -1;
    double numerator = scalar_product(substraction(p.point, eye), p.normal);
    return numerator / denominator;
}

int point_belongs_to_triangle(Triangle t, Triplet point, Triplet n)
{
    if (scalar_product(vector_product(substraction(t.y, t.x), substraction(point, t.x)), n) < 0)
        return 0;
    if (scalar_product(vector_product(substraction(t.z, t.y), substraction(point, t.y)), n) < 0)
        return 0;
    if (scalar_product(vector_product(substraction(t.x, t.z), substraction(point, t.z)), n) < 0)
        return 0;
    return 1;
}

double distance_to_triangle(Triangle t, Image *image, Triplet d, Triplet eye)
{
    Triplet n = norm(vector_product(substraction(t.y, t.x), substraction(t.z, t.x)));
    n.type = 1;
    double dist = distance_to_plane((Plane){t.x, n, (Triplet){0, 0, 0, 2}}, image, d, eye);
    if (dist == -1)
        return -1;
    Triplet point = intersection_point(eye, d, dist);
    point.type = 0;
    if (point_belongs_to_triangle(t, point, n))
        return dist;
    return -1;
}

Triplet intersection_point(Triplet eye, Triplet pixelDirVec, double dist)
{
    return addition(eye, scalar_multiplication(pixelDirVec, dist));
}

Triplet normal_interpoint_sphere(Triplet intersectionPoint, Triplet circleCenter)
{
    return norm(substraction(intersectionPoint, circleCenter));
}

Triplet normal_interpoint_triangle(Triangle t)
{
    return norm(vector_product(substraction(t.y, t.x), substraction(t.z, t.x)));
}

Triplet light_direction(Triplet normPointDir, DirectionalLight dl)
{
    double max = scalar_product(normPointDir, norm(dl.vector)) > 0 ? scalar_product(normPointDir, norm(dl.vector)) : 0;
    return scalar_multiplication(dl.color, max);
}

Triplet light_source_color(LightPoint lightPoint, Triplet intersectionPoint, Triplet normPointDir)
{
    Triplet ldir = norm(substraction(lightPoint.point, intersectionPoint));
    ldir.type = 1;
    double max = scalar_product(normPointDir, ldir) > 0 ? scalar_product(normPointDir, ldir) : 0;
    return scalar_multiplication(lightPoint.color, max);
}

int **create_obstructed_lights(Image *im)
{
    int **tab = (int **)malloc(sizeof(int *) * 2);
    int max = im->lights->nbDirectionals > im->lights->nbPoints ? im->lights->nbDirectionals : im->lights->nbPoints;
    for (int i = 0; i < 2; i += 1)
    {
        tab[i] = (int *)malloc(sizeof(int) * max);
        for (int j = 0; j < max; j += 1)
        {
            tab[i][j] = 1;
        }
    }
    return tab;
}

void free_obstructed_lights(int **tab)
{
    if (!tab)
        return;
    for (int i = 0; i < 2; i += 1)
    {
        if (tab[i])
            free(tab[i]);
    }
    free(tab);
}

Triplet coul(Image *im, Triplet normalInterPoint, Triplet color, Triplet interPoint, int **obstructedLights)
{
    int i;
    Triplet coul = {0, 0, 0, 2};
    for (i = 0; i < im->lights->nbDirectionals; i++)
    {
        if (!obstructedLights[0][i])
            continue;
        coul = addition(coul, light_direction(normalInterPoint, im->lights->directionals[i]));
    }
    for (i = 0; i < im->lights->nbPoints; i++)
    {
        if (!obstructedLights[1][i])
            continue;
        coul = addition(coul, light_source_color(im->lights->points[i], interPoint, normalInterPoint));
    }
    coul = schur_product(coul, color);
    return addition(coul, im->data->ambient);
}

Triplet vector_from_to(Triplet a, Triplet b)
{
    return substraction(b, a);
}

Triplet pixel_color(int struc, Triplet interPoint, Sphere s, Triangle tr, Plane pl, int **obstructedLights, Image *im, int t) {
    Triplet color, n;
    if (t != -1) {
        if (struc == 0) {
            n = normal_interpoint_sphere(interPoint, s.center);
            color = s.color;
        } else if (struc == 1) {
            n = normal_interpoint_triangle(tr);
            color = tr.color;
        } else {
            n = pl.normal;
            color = pl.color;
        }
        color = coul(im, n, color, interPoint, obstructedLights);
    } else {
        color = (Triplet){0, 0, 0, 2};
    }
    return color;
}
