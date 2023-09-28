#include "ray_tracing.h"

int init_image(Image *im)
{
    im->height = 0;
    im->width = 0;

    im->name = (char *)malloc(sizeof(char) * strlen("output.png "));
    if (!im->name)
        goto errorAllocation;
    strcpy(im->name, "output.png");

    im->data = (ParseData *)malloc(sizeof(ParseData));
    if (!im->data)
        goto errorAllocation;
    im->data->maxverts = 0;
    im->data->vertex = NULL;
    im->data->shadow = false;
    im->data->diffuse = (Triplet){1, 1, 1, 2};
    im->data->ambient = (Triplet){0, 0, 0, 2};

    im->obj = (Objects *)malloc(sizeof(Objects));
    if (!im->obj)
        goto errorAllocation;
    im->obj->nbPlanes = 0;
    im->obj->nbSpheres = 0;
    im->obj->nbTriangles = 0;
    im->obj->triangles = NULL;
    im->obj->planes = NULL;
    im->obj->spheres = NULL;

    im->pixels = NULL;

    im->lights = (Lights *)malloc(sizeof(Lights));
    if (!im->lights)
        goto errorAllocation;
    im->lights->directionals = NULL;
    im->lights->points = NULL;
    im->lights->nbDirectionals = 0;
    im->lights->nbPoints = 0;
    return 0;

errorAllocation:
    set_error(ERROR_ALLOCATION_FAILED, im, "Problème d'allocation dans init_image");
    return 1;
}

int create_object_triangle(Image *im, Triplet x, Triplet y, Triplet z)
{
    if (!im->obj->triangles)
        im->obj->triangles = (Triangle *)malloc(sizeof(Triangle) * ++im->obj->nbTriangles);
    else
        im->obj->triangles = (Triangle *)realloc(im->obj->triangles, sizeof(Triangle) * ++im->obj->nbTriangles);
    if (!im->obj->triangles)
        goto errorAllocation;

    Triangle t = {x, y, z, (Triplet){im->data->diffuse.x, im->data->diffuse.y, im->data->diffuse.z, 2}};
    (im->obj->triangles)[im->obj->nbTriangles - 1] = t;
    return 0;

errorAllocation:
    set_error(ERROR_ALLOCATION_FAILED, im, "Problème d'allocation pendant la création d'un triangle.");
    return 1;
}

int create_object_sphere(Image *im, Triplet center, double radius)
{
    if (!im->obj->spheres)
        im->obj->spheres = (Sphere *)malloc(sizeof(Sphere) * ++im->obj->nbSpheres);
    else
        im->obj->spheres = (Sphere *)realloc(im->obj->spheres, sizeof(Sphere) * ++im->obj->nbSpheres);
    if (!im->obj->spheres)
        goto errorAllocation;

    Sphere s = {center, radius, (Triplet){im->data->diffuse.x, im->data->diffuse.y, im->data->diffuse.z, 2}};
    im->obj->spheres[im->obj->nbSpheres - 1] = s;
    return 0;

errorAllocation:
    set_error(ERROR_ALLOCATION_FAILED, im, "Problème d'allocation pendant la création d'une sphère.");
    return 1;
}

int create_object_plane(Image *im, Triplet point, Triplet vector)
{
    if (!im->obj->planes)
        im->obj->planes = (Plane *)malloc(sizeof(Plane) * ++im->obj->nbPlanes);
    else
        im->obj->planes = (Plane *)realloc(im->obj->planes, sizeof(Plane) * ++im->obj->nbPlanes);
    if (!im->obj->planes)
        goto errorAllocation;

    Plane p = {point, vector, (Triplet){im->data->diffuse.x, im->data->diffuse.y, im->data->diffuse.z, 2}};
    im->obj->planes[im->obj->nbPlanes - 1] = p;
    return 0;

errorAllocation:
    set_error(ERROR_ALLOCATION_FAILED, im, "Problème d'allocation pendant la création d'un plan.");
    return 1;
}

int create_light_point(Image *im, Triplet point, Triplet color)
{
    if (!im->lights->points)
        im->lights->points = (LightPoint *)malloc(sizeof(LightPoint) * ++im->lights->nbPoints);
    else
        im->lights->points = (LightPoint *)realloc(im->lights->points, sizeof(LightPoint) * ++im->lights->nbPoints);
    if (!im->lights->points)
        goto errorAllocation;

    LightPoint p = {point, color};
    im->lights->points[im->lights->nbPoints - 1] = p;
    return 0;

errorAllocation:
    set_error(ERROR_ALLOCATION_FAILED, im, "Problème d'allocation pendant la création d'un plan.");
    return 1;
}

int create_light_directional(Image *im, Triplet vector, Triplet color)
{
    if (!im->lights->directionals)
        im->lights->directionals = (DirectionalLight *)malloc(sizeof(DirectionalLight) * ++im->lights->nbDirectionals);
    else
        im->lights->directionals = (DirectionalLight *)realloc(im->lights->directionals, sizeof(DirectionalLight) * ++im->lights->nbDirectionals);
    if (!im->lights->directionals)
        goto errorAllocation;

    DirectionalLight p = {vector, color};
    im->lights->directionals[im->lights->nbDirectionals - 1] = p;
    return 0;

errorAllocation:
    set_error(ERROR_ALLOCATION_FAILED, im, "Problème d'allocation pendant la création d'un plan.");
    return 1;
}

void print_camera(Camera cam)
{
    printf("Camera : \n");
    printf("from : ");
    print_triplet(cam.from);
    printf("at : ");
    print_triplet(cam.at);
    printf("up : ");
    print_triplet(cam.up);
    printf("fov : %d\n", cam.fov);
}

void print_image(Image *im)
{
    printf("Image : \n");
    printf("width : %d, height : %d\n", im->width, im->height);
    if (im->name)
        printf("name : %s\n", im->name);
    print_camera(im->cam);
    if (im->data)
        print_parse_data(im->data);
}

void free_pixels(Image *im)
{
    int i;
    if ((im)->pixels)
    {
        for (i = 0; i < (im)->width; i++)
        {
            if (((im)->pixels)[i])
                free(((im)->pixels)[i]);
        }
        free((im)->pixels);
    }
}

void free_objects (Image *im) {
    if (!im->obj)
        return;
    if (im->obj->spheres)
        free(im->obj->spheres);
    if (im->obj->planes)
        free(im->obj->planes);
    if (im->obj->triangles)
        free(im->obj->triangles);
    free(im->obj);
}

void free_lights (Image *im) {
    if (!im->lights)
        return;
    if (im->lights->points)
        free(im->lights->points);
    if (im->lights->directionals)
        free(im->lights->directionals);
    free(im->lights);
}

void free_image(Image *im)
{
    if (!im)
        return;
    if (im->name)
        free(im->name);
    if (im->data)
        free_parse_data(im->data);
    free_objects(im);
    free_lights(im);
    free_pixels(im);
    free(im);
}

void allocate_pixels(Image *im)
{
    int i;
    if (!(im)->pixels)
    {
        (im)->pixels = (Triplet **)malloc((im)->width * sizeof(Triplet *));
        if (!(im)->pixels)
        {
            goto erroralloc;
        }
        for (i = 0; i < (im)->width; i++)
        {
            ((im)->pixels)[i] = (Triplet *)malloc((im)->height * sizeof(Triplet));
            if (!((im)->pixels)[i])
            {
                goto erroralloc;
            }
        }
    }

    return;

erroralloc:
    set_error(ERROR_ALLOCATION_FAILED, im, "Erreur lors de l'allocation des pixels de l'image");
    free_pixels(im);
}

int equals_sphere(Sphere s1, Sphere s2)
{
    if (!equals_triplet(s1.center, s2.center))
        return 0;
    if (!equals_triplet(s1.color, s2.color))
        return 0;
    if (s1.radius != s2.radius)
        return 0;
    return 1;
}

int equals_triangle(Triangle t1, Triangle t2)
{
    if (!equals_triplet(t1.x, t2.x))
        return 0;
    if (!equals_triplet(t1.y, t2.y))
        return 0;
    if (!equals_triplet(t1.z, t2.z))
        return 0;
    if (!equals_triplet(t1.color, t2.color))
        return 0;
    return 1;
}

int equals_plane(Plane p1, Plane p2)
{
    if (!equals_triplet(p1.point, p2.point))
        return 0;
    if (!equals_triplet(p1.normal, p2.normal))
        return 0;
    if (!equals_triplet(p1.color, p2.color))
        return 0;
    return 1;
}

void print_triangle (Triangle t) {
    printf("Triangle : \n");
    t.x.error = 0;
    t.y.error = 0;
    t.z.error = 0;
    t.color.error = 0;
    print_triplet(t.x);
    print_triplet(t.y);
    print_triplet(t.z);
    print_triplet(t.color);
}

void print_sphere (Sphere s) {
    s.center.error = 0;
    printf("Sphère: \n");
    print_triplet(s.center);
    printf("Radius : %lf\n", s.radius);
    print_triplet(s.color);
}