#include "parse_scene.h"

int valid_float(double x)
{
    return (x >= 0 && x <= 1);
}

int valid_int(double x)
{
    return (!fmod(x, 1));
}

void free_args(char **args, int nbArgs)
{
    for (int i = 0; i < nbArgs; i += 1)
    {
        free(args[i]);
    }
    free(args);
}

int error_return(Image *im)
{
    free_image(im);
    return 1;
}

int parse_data_size(Image *im, char **args, int nbArgs)
{
    if (nbArgs < 3)
        goto errorMissingArgument;
    if (nbArgs > 3)
        goto errorTooManyArgs;

    if (im->width || im->height)
        goto errorAlreadyDefined;
    if (!valid_int(atof(args[1])) || !valid_int(atof(args[2])))
        goto errorInvalidType;

    im->width = atoi(args[1]);
    im->height = atoi(args[2]);
    return 0;

errorMissingArgument:
    set_error(ERROR_MISSING_ARGUMENT, im, "Il manque une ou plusieurs valeurs à la définition de la taille de l'image.");
    return 0;
errorTooManyArgs:
    set_error(ERROR_TOO_MANY_ARGUMENTS, im, "Il y a trop de valeurs à la définition de la taille de l'image.");
    return 0;
errorAlreadyDefined:
    set_error(ERROR_ALREADY_DEFINED, im, "Impossible de changer la taille après l'avoir définie une première fois.");
    return error_return(im);
errorInvalidType:
    set_error(ERROR_INVALID_TYPE, im, "La taille de l'image requiert un int et non un float.");
    return error_return(im);
}

int parse_data_output(Image *im, char **args, int nbArgs)
{
    if (nbArgs < 2)
        goto errorMissingArgument;
    if (nbArgs > 2)
        goto errorTooManyArgs;

    free(im->name);
    im->name = (char *)malloc(sizeof(char) * (strlen(args[1]) + 1));
    if (!im->name)
        goto errorAllocation;
    strcpy(im->name, args[1]);
    return 0;

errorMissingArgument:
    set_error(ERROR_MISSING_ARGUMENT, im, "Il manque une valeur à la définition du nom de l'image finale. La définition est ignorée, le nom est à \"output.png\" par défaut, le parse continue.");
    return 0;
errorTooManyArgs:
    set_error(ERROR_TOO_MANY_ARGUMENTS, im, "Il y a trop de valeurs à la définition du nom de l'image finale. La définition est ignorée, le nom est à \"output.png\" par défaut, le parse continue.");
    return 0;
errorAllocation:
    set_error(ERROR_ALLOCATION_FAILED, im, "Problème d'allocation du nom de l'image dans parse_data_output.");
    return error_return(im);
}

int parse_data_camera(Image *im, char **args, int nbArgs)
{
    if (nbArgs < 11)
        goto errorMissingArgument;
    if (nbArgs > 11)
        goto errorTooManyArgs;

    Camera c;
    c.from.x = atof(args[1]);
    c.from.y = atof(args[2]);
    c.from.z = atof(args[3]);
    c.from.type = 0;
    c.at.x = atof(args[4]);
    c.at.y = atof(args[5]);
    c.at.z = atof(args[6]);
    c.at.type = 0;
    c.up.x = atof(args[7]);
    c.up.y = atof(args[8]);
    c.up.z = atof(args[9]);
    c.up.type = 1;
    c.fov = atoi(args[10]);
    im->cam = c;
    return 0;

errorMissingArgument:
    set_error(ERROR_MISSING_ARGUMENT, im, "Il manque une ou plusieurs valeurs à la définition de la camera.");
    return error_return(im);
errorTooManyArgs:
    set_error(ERROR_TOO_MANY_ARGUMENTS, im, "Il y a trop de valeurs à la définition de la camera.");
    return error_return(im);
}

int parse_data_ambient(Image *im, char **args, int nbArgs)
{
    if (nbArgs < 4)
        goto errorMissingArgument;
    if (nbArgs > 4)
        goto errorTooManyArgs;

    if (!valid_float(atof(args[1])))
        goto errorInvalidValue;
    im->data->ambient.x = atof(args[1]);
    if (!valid_float(atof(args[2])))
        goto errorInvalidValue;
    im->data->ambient.y = atof(args[2]);
    if (!valid_float(atof(args[3])))
        goto errorInvalidValue;
    im->data->ambient.z = atof(args[3]);
    im->data->ambient.type = 2;
    return 0;

errorMissingArgument:
    set_error(ERROR_MISSING_ARGUMENT, im, "Il manque une ou plusieurs valeurs à la définition de la lumière ambiante (ambient). La définition est ignorée, le parse continue.");
    return 0;
errorTooManyArgs:
    set_error(ERROR_TOO_MANY_ARGUMENTS, im, "Il y a trop de valeurs à la définition de la lumière ambiante (ambient). La définition est ignorée, le parse continue.");
    return 0;
errorInvalidValue:
    set_error(ERROR_INVALID_VALUE, im, "Valeur négative ou supérieure à 1 trouvée pour une couleur à la définition de la lumière ambiante (ambient).");
    return error_return(im);
}

int parse_data_diffuse(Image *im, char **args, int nbArgs)
{
    if (nbArgs < 4)
        goto errorMissingArgument;
    if (nbArgs > 4)
        goto errorTooManyArgs;

    if (atof(args[1]) + im->data->ambient.x > 1 || !valid_float(atof(args[1])))
        goto errorInvalidValue;
    im->data->diffuse.x = atof(args[1]);
    if (atof(args[2]) + im->data->ambient.y > 1 || !valid_float(atof(args[2])))
        goto errorInvalidValue;
    im->data->diffuse.y = atof(args[2]);
    if (atof(args[3]) + im->data->ambient.z > 1 || !valid_float(atof(args[3])))
        goto errorInvalidValue;
    im->data->diffuse.z = atof(args[3]);
    im->data->diffuse.type = 2;
    return 0;

errorMissingArgument:
    set_error(ERROR_MISSING_ARGUMENT, im, "Il manque une ou plusieurs valeur à la définition de la couleur des objets (diffuse). La définition est ignorée, le parse continue.");
    return 0;
errorTooManyArgs:
    set_error(ERROR_TOO_MANY_ARGUMENTS, im, "Il y a trop de valeurs à la définition de la couleur des objets (diffuse). La définition est ignorée, le parse continue.");
    return 0;
errorInvalidValue:
    set_error(ERROR_INVALID_VALUE, im, "Valeur négative trouvée pour une couleur à la définition de la couleur des objets (diffuse).");
    return error_return(im);
}

int parse_data_specular(Image *im, char **args, int nbArgs)
{
    if (nbArgs < 4)
        goto errorMissingArgument;
    if (nbArgs > 4)
        goto errorTooManyArgs;

    if (!valid_float(atof(args[1])))
        goto errorInvalidValue;
    im->data->specular.x = atof(args[1]);
    if (!valid_float(atof(args[2])))
        goto errorInvalidValue;
    im->data->specular.y = atof(args[2]);
    if (!valid_float(atof(args[3])))
        goto errorInvalidValue;
    im->data->specular.z = atof(args[3]);
    im->data->specular.type = 2;
    return 0;

errorMissingArgument:
    set_error(ERROR_MISSING_ARGUMENT, im, "Il manque une ou plusieurs valeurs à la définition du réflechissement (specular). La définition est ignorée, le parse continue.");
    return 0;
errorTooManyArgs:
    set_error(ERROR_TOO_MANY_ARGUMENTS, im, "Il y a trop de valeurs à la définition du réflechissement (specular). La définition est ignorée, le parse continue.");
    return 0;
errorInvalidValue:
    set_error(ERROR_INVALID_VALUE, im, "Valeur négative trouvée à la définition du réflechissement (specular).");
    return error_return(im);
}

int parse_data_shininess(Image *im, char **args, int nbArgs)
{
    if (nbArgs < 2)
        goto errorMissingArgument;
    if (nbArgs > 2)
        goto errorTooManyArgs;

    if (!valid_int(atof(args[1])))
        goto errorInvalidType;
    if (atoi(args[1]) < 0)
        goto errorInvalidValue;
    im->data->shininess = atoi(args[1]);
    return 0;

errorMissingArgument:
    set_error(ERROR_MISSING_ARGUMENT, im, "Il manque une valeur à la définition de la brillance (shininess). La définition est ignorée, le parse continue.");
    return 0;
errorTooManyArgs:
    set_error(ERROR_TOO_MANY_ARGUMENTS, im, "Il y a trop de valeurs à la définition de la brillance (shininess). La définition est ignorée, le parse continue.");
    return 0;
errorInvalidValue:
    set_error(ERROR_INVALID_VALUE, im, "Valeur négative trouvée à la définition de la brillance (shininess).");
    return error_return(im);
errorInvalidType:
    set_error(ERROR_INVALID_TYPE, im, "Type float trouvé à la définition de la brillance (shininess), au lieu de int.");
    return error_return(im);
}

int parse_data_directional(Image *im, char **args, int nbArgs)
{
    if (nbArgs < 7)
        goto errorMissingArgument;
    if (nbArgs > 7)
        goto errorTooManyArgs;

    Triplet vector, color;
    vector.x = atof(args[1]);
    vector.y = atof(args[2]);
    vector.z = atof(args[3]);
    vector.type = 1;
    if (!(vector.x || vector.y || vector.z))
        goto errorNullVector;
    if (!valid_float(atof(args[4])))
        goto errorInvalidValue;
    color.x = atof(args[4]);
    if (!valid_float(atof(args[5])))
        goto errorInvalidValue;
    color.y = atof(args[5]);
    if (!valid_float(atof(args[6])))
        goto errorInvalidValue;
    color.z = atof(args[6]);
    color.type = 2;
    if (create_light_directional(im, vector, color))
        return error_return(im);
    return 0;

errorMissingArgument:
    set_error(ERROR_MISSING_ARGUMENT, im, "Il manque une ou plusieurs valeurs à la création d'une lumière globale directionnelle (directional). Sa création est annulée, le parse continue.");
    return 0;
errorTooManyArgs:
    set_error(ERROR_TOO_MANY_ARGUMENTS, im, "Il y a trop de valeurs à la création d'une lumière globale directionnelle (directional). Sa création est annulée, le parse continue.");
    return 0;
errorNullVector:
    set_error(ERROR_NULL_VECTOR, im, "Impossible de créer une lumière directionnelle avec une direction nulle.");
    return error_return(im);
errorInvalidValue:
    set_error(ERROR_INVALID_VALUE, im, "Valeur négative trouvée pour une couleur à la création d'une lumière globale directionnelle (directional).");
    return error_return(im);
}

int parse_data_point(Image *im, char **args, int nbArgs)
{
    if (nbArgs < 7)
        goto errorMissingArgument;
    if (nbArgs > 7)
        goto errorTooManyArgs;

    Triplet point, color;
    point.x = atof(args[1]);
    point.y = atof(args[2]);
    point.z = atof(args[3]);
    point.type = 0;
    if (!valid_float(atof(args[4])))
        goto errorInvalidValue;
    color.x = atof(args[4]);
    if (!valid_float(atof(args[5])))
        goto errorInvalidValue;
    color.y = atof(args[5]);
    if (!valid_float(atof(args[6])))
        goto errorInvalidValue;
    color.z = atof(args[6]);
    color.type = 2;
    if (create_light_point(im, point, color))
        return error_return(im);
    return 0;

errorMissingArgument:
    set_error(ERROR_MISSING_ARGUMENT, im, "Il manque une ou plusieurs valeurs à la création d'une source de lumière (point). Sa création est annulée, le parse continue.");
    return 0;
errorTooManyArgs:
    set_error(ERROR_TOO_MANY_ARGUMENTS, im, "Il y a trop de valeurs à la création d'une source de lumière (point). Sa création est annulée, le parse continue.");
    return 0;
errorInvalidValue:
    set_error(ERROR_INVALID_VALUE, im, "Valeur négative trouvée pour une couleur à la création d'une source de lumière.");
    return error_return(im);
}

int parse_data_maxverts(Image *im, char **args, int nbArgs)
{
    if (nbArgs < 2)
        goto errorMissingArgument;
    if (nbArgs > 2)
        goto errorTooManyArgs;

    if (im->data->maxverts)
        goto errorAlreadyDefined;

    if (!valid_int(atof(args[1])))
        goto errorInvalidType;
    else if (atoi(args[1]) < 0)
        goto errorInvalidValue;
    im->data->maxverts = atoi(args[1]);
    im->data->nbVertex = 0;
    im->data->vertex = (Triplet *)malloc(sizeof(Triplet) * im->data->maxverts);
    if (!im->data->vertex)
        goto errorAllocation;
    return 0;

errorMissingArgument:
    set_error(ERROR_MISSING_ARGUMENT, im, "Il manque une valeur lors de la définition de maxverts.");
    return error_return(im);
errorTooManyArgs:
    set_error(ERROR_TOO_MANY_ARGUMENTS, im, "Il y a trop de valeurs lors de la définition de maxverts.");
    return error_return(im);
errorInvalidValue:
    set_error(ERROR_INVALID_VALUE, im, "Valeur négative trouvée à la définition de maxverts.");
    return error_return(im);
errorInvalidType:
    set_error(ERROR_INVALID_TYPE, im, "Type float trouvé à la définition de maxverts, au lieu de int.");
    return error_return(im);
errorAllocation:
    set_error(ERROR_ALLOCATION_FAILED, im, "Problème d'allocation dans parse_data_maxverts.");
    return error_return(im);
errorAlreadyDefined:
    set_error(ERROR_ALREADY_DEFINED, im, "Redéfinition de maxverts impossible, donc ignorée.");
    return 0;
}

int parse_data_vertex(Image *im, char **args, int nbArgs)
{
    if (nbArgs < 4)
        goto errorMissingArgument;
    if (nbArgs > 4)
        goto errorTooManyArgs;
    if (!im->data->maxverts)
        goto errorMissingVertexLimit;
    if (im->data->nbVertex == im->data->maxverts)
        goto errorOutOfBounds;

    Triplet v;
    v.x = atof(args[1]);
    v.y = atof(args[2]);
    v.z = atof(args[3]);
    v.type = 0;
    im->data->vertex[im->data->nbVertex++] = v;
    return 0;

errorMissingArgument:
    set_error(ERROR_MISSING_ARGUMENT, im, "Il manque une ou plusieurs valeurs à la création d'un vertex. La création de ce vertex est annulée. Cela peut poser des problèmes pour la création des triangles.");
    return error_return(im);
errorTooManyArgs:
    set_error(ERROR_TOO_MANY_ARGUMENTS, im, "Il y a trop de valeurs à la création d'un vertex. La création de ce vertex est annulée. Cela peut poser des problèmes pour la création des triangles.");
    return error_return(im);
errorMissingVertexLimit:
    set_error(ERROR_MISSING_VERTEX_LIMIT, im, "Création d'un vertex avant la définition de maxverts.");
    return error_return(im);
errorOutOfBounds:
    set_error(ERROR_OUT_OF_BOUNDS, im, "Création d'un vertex alors que la limite de vertex (maxverts) a été atteinte.");
    return error_return(im);
}

int parse_data_sphere(Image *im, char **args, int nbArgs)
{
    if (nbArgs < 5)
        goto errorMissingArgument;
    if (nbArgs > 5)
        goto errorTooManyArgs;

    Triplet center;
    center.x = atof(args[1]);
    center.y = atof(args[2]);
    center.z = atof(args[3]);
    center.type = 0;
    if (atof(args[4]) < 0)
        goto errorInvalidValue;
    if (create_object_sphere(im, center, atof(args[4])))
        return 1;
    return 0;

errorMissingArgument:
    set_error(ERROR_MISSING_ARGUMENT, im, "Il manque une ou plusieurs valeurs à la création d'une sphère. La création de cette sphère est annulée, le parse continue.");
    return 0;
errorTooManyArgs:
    set_error(ERROR_TOO_MANY_ARGUMENTS, im, "Il y a trop de valeurs à la création d'une sphère. La création de cette sphère est annulée, le parse continue.");
    return 0;
errorInvalidValue:
    set_error(ERROR_INVALID_VALUE, im, "Valeur négative trouvée pour une couleur à la création d'une sphère.");
    return error_return(im);
}

int parse_data_triangle(Image *im, char **args, int nbArgs)
{
    if (nbArgs < 4)
        goto errorMissingArgument;
    if (nbArgs > 4)
        goto errorTooManyArgs;

    Triplet x, y, z;
    x.type = 0;
    y.type = 0;
    z.type = 0;

    if (!valid_int(atof(args[1])))
        goto errorInvalidType;
    if (atoi(args[1]) < 0)
        goto errorInvalidValue;
    if (atoi(args[1]) >= im->data->maxverts)
        goto errorOutOfBounds;
    x = im->data->vertex[atoi(args[1])];
    if (!valid_int(atof(args[2])))
        goto errorInvalidType;
    if (atoi(args[2]) < 0)
        goto errorInvalidValue;
    if (atoi(args[2]) >= im->data->maxverts)
        goto errorOutOfBounds;
    y = im->data->vertex[atoi(args[2])];
    if (!valid_int(atof(args[3])))
        goto errorInvalidType;
    if (atoi(args[3]) < 0)
        goto errorInvalidValue;
    if (atoi(args[3]) >= im->data->maxverts)
        goto errorOutOfBounds;
    z = im->data->vertex[atoi(args[3])];
    if (create_object_triangle(im, x, y, z))
        return 1;
    return 0;

errorMissingArgument:
    set_error(ERROR_MISSING_ARGUMENT, im, "Il manque une ou plusieurs valeurs à la création d'un triangle. La création de ce triangle est annulée, le parse continue.");
    return 0;
errorTooManyArgs:
    set_error(ERROR_TOO_MANY_ARGUMENTS, im, "Il y a trop de valeurs à la création d'un triangle. La création de ce triangle est annulée, le parse continue.");
    return 0;
errorInvalidType:
    set_error(ERROR_INVALID_TYPE, im, "Type float trouvé à la création d'un triangle, au lieu de int.");
    return error_return(im);
errorInvalidValue:
    set_error(ERROR_INVALID_VALUE, im, "Valeur négative trouvée à la création d'un triangle. Impossible d'associer un vertex à cette valeur.");
    return error_return(im);
errorOutOfBounds:
    set_error(ERROR_OUT_OF_BOUNDS, im, "Création d'un triangle avec des numéros de vertex supérieurs à la limite (maxverts).");
    return error_return(im);
}

int parse_data_plane(Image *im, char **args, int nbArgs)
{
    if (nbArgs < 7)
        goto errorMissingArgument;
    if (nbArgs > 7)
        goto errorTooManyArgs;

    Triplet point, vector;
    point.x = atof(args[1]);
    point.y = atof(args[2]);
    point.z = atof(args[3]);
    point.type = 0;
    vector.x = atof(args[4]);
    vector.y = atof(args[5]);
    vector.z = atof(args[6]);
    vector.type = 1;
    if (!(vector.x || vector.y || vector.z))
        goto errorNullVector;
    if (create_object_plane(im, point, vector))
        return 1;
    return 0;

errorMissingArgument:
    set_error(ERROR_MISSING_ARGUMENT, im, "Il manque une ou plusieurs valeurs à la création d'un plan. La création de ce plan est annulée, le parse continue.");
    return 0;
errorTooManyArgs:
    set_error(ERROR_TOO_MANY_ARGUMENTS, im, "Il y a trop de valeurs à la création d'un plan. La création de ce plan est annulée, le parse continue.");
    return 0;
errorNullVector:
    set_error(ERROR_NULL_VECTOR, im, "Impossible de créer un plan avec une normale nulle.");
    return error_return(im);
}

int parse_data_shadow(Image *im, char **args, int nbArgs)
{
    if (nbArgs < 2)
        goto errorMissingArgument;
    if (nbArgs > 2)
        goto errorTooManyArgs;

    if (!strcmp("true", args[1]))
        im->data->shadow = true;
    else if (!strcmp("false", args[1]))
        im->data->shadow = false;
    else
        goto errorInvalidValue;

    return 0;

errorMissingArgument:
    set_error(ERROR_MISSING_ARGUMENT, im, "Il manque une valeurs à la définition des ombres. La définition est ignorée, le parse continue.");
    return 0;
errorTooManyArgs:
    set_error(ERROR_TOO_MANY_ARGUMENTS, im, "Il y a trop de valeurs à la définition des ombres. La définition est ignorée, le parse continue.");
    return 0;
errorInvalidValue:
    set_error(ERROR_INVALID_VALUE, im, "Booléen (true/false) attendu après le mot clé \"shadow\". Définition ignorée, le parse continue.");
    return 0;
}

int router_body(Image *im, char **args, int nbArgs)
{
    if (!args || !args[0])
        goto errorInvalidArgument;
    if (!strcmp(args[0], "ambient"))
        return parse_data_ambient(im, args, nbArgs);
    else if (!strcmp(args[0], "diffuse"))
        return parse_data_diffuse(im, args, nbArgs);
    else if (!strcmp(args[0], "specular"))
        return parse_data_specular(im, args, nbArgs);
    else if (!strcmp(args[0], "shininess"))
        return parse_data_shininess(im, args, nbArgs);
    else if (!strcmp(args[0], "directional"))
        return parse_data_directional(im, args, nbArgs);
    else if (!strcmp(args[0], "point"))
        return parse_data_point(im, args, nbArgs);
    else if (!strcmp(args[0], "maxverts"))
        return parse_data_maxverts(im, args, nbArgs);
    else if (!strcmp(args[0], "vertex"))
        return parse_data_vertex(im, args, nbArgs);
    else if (!strcmp(args[0], "sphere"))
        return parse_data_sphere(im, args, nbArgs);
    else if (!strcmp(args[0], "tri"))
        return parse_data_triangle(im, args, nbArgs);
    else if (!strcmp(args[0], "plane"))
        return parse_data_plane(im, args, nbArgs);
    else if (!strcmp(args[0], "shadow"))
        return parse_data_shadow(im, args, nbArgs);
    else
        goto errorInvalidKeyword;
    return 0;

errorInvalidArgument:
    set_error(ERROR_INVALID_ARGUMENT, im, "Pointeur invalide dans router_body: args ou args[0].");
    return error_return(im);
errorInvalidKeyword:;
    char *mess = (char *)malloc(sizeof(char) * (strlen(args[0]) + 100));
    strcpy(mess, "Rencontre d'un mot clé invalide pendant le parsing. Mot rencontré : ");
    strcat(mess, args[0]);
    set_error(ERROR_INVALID_KEYWORD, im, mess);
    free(mess);
    return error_return(im);
}

Image *parse_body(Image *im, FILE *f)
{
    char *line = (char *)malloc(sizeof(char) * 300);
    if (!line)
        goto errorAllocation;
    if (!fgets(line, 299, f))
        goto EndOfFile;

    if (line[0] == '#')
        goto NextLine;

    char *separators = " \t\n\r";
    char *token = strtok(line, separators);
    char **args = NULL;
    int nbArgs = 0;

    while (token)
    {
        if (!args)
            args = (char **)malloc(sizeof(char *));
        else
            args = (char **)realloc(args, sizeof(char *) * (nbArgs + 1));
        if (!args)
            goto errorAllocation;
        args[nbArgs] = (char *)malloc(sizeof(char) * (strlen(token) + 1));
        if (!args[nbArgs])
            goto errorAllocation;
        strcpy(args[nbArgs++], token);
        token = strtok(NULL, separators);
    }

    if (!args)
        goto NextLine;

    if (router_body(im, args, nbArgs))
        goto errorParsingCancelled;

    free_args(args, nbArgs);

NextLine:
    if (line)
        free(line);
    return parse_body(im, f);
EndOfFile:
    free(line);
    return im;
errorAllocation:
    set_error(ERROR_ALLOCATION_FAILED, im, "Problème d'allocation dans parse_body.");
    free_image(im);
    return NULL;
errorParsingCancelled:
    free_args(args, nbArgs);
    free(line);
    set_error(ERROR_PARSING_CANCELLED, im, "Une erreur est survenue pendant le parsing qui a forcé l'arrêt du programme.");
    return NULL;
}

int router_head(Image *im, char **args, int nbArgs)
{
    if (!args || !args[0])
        goto errorInvalidArgument;

    if (strcmp(args[0], "size") && !im->width && !im->height)
        goto errorMissingSize;
    else if (!strcmp(args[0], "size"))
        return parse_data_size(im, args, nbArgs);
    else if (!strcmp(args[0], "output"))
        return parse_data_output(im, args, nbArgs);
    else if (!strcmp(args[0], "camera"))
        return parse_data_camera(im, args, nbArgs);
    else if (!strcmp(args[0], "shadow"))
        return parse_data_shadow(im, args, nbArgs);
    else
    {
        router_body(im, args, nbArgs);
        return -1;
    }

errorInvalidArgument:
    set_error(ERROR_INVALID_ARGUMENT, im, "Pointeur invalide dans router_body: args ou args[0].");
    return error_return(im);
errorMissingSize:
    set_error(ERROR_MISSING_SIZE, im, "Il faut définir la taille de l'image avant de pouvoir fait quoi que ce soit.");
    return error_return(im);
}

Image *parse_head(Image *im, FILE *f)
{
    char *line = (char *)malloc(sizeof(char) * 300);
    if (!line)
        goto errorAllocation;
    if (!fgets(line, 299, f))
        goto EndOfFile;

    if (line[0] == '#')
        goto NextLine;

    char *separators = " \t\n\r";
    char *token = strtok(line, separators);
    char **args = NULL;
    int nbArgs = 0;

    while (token)
    {
        if (!args)
            args = (char **)malloc(sizeof(char *));
        else
            args = (char **)realloc(args, sizeof(char *) * (nbArgs + 1));
        if (!args)
            goto errorAllocation;
        args[nbArgs] = (char *)malloc(sizeof(char) * (strlen(token) + 1));
        if (!args[nbArgs])
            goto errorAllocation;
        strcpy(args[nbArgs++], token);
        token = strtok(NULL, separators);
    }

    if (!args)
        goto NextLine;

    int res = router_head(im, args, nbArgs);
    if (res == -1)
        goto NextParse;
    else if (res)
        goto errorParsingCancelled;

    free_args(args, nbArgs);

NextLine:
    if (line)
        free(line);
    return parse_head(im, f);
NextParse:
    if (line)
        free(line);
    free_args(args, nbArgs);
    return parse_body(im, f);
EndOfFile:
    free(line);
    return im;
errorAllocation:
    set_error(ERROR_ALLOCATION_FAILED, im, "Problème d'allocation dans parse_head.");
    free_image(im);
    return NULL;
errorParsingCancelled:
    free_args(args, nbArgs);
    free(line);
    set_error(ERROR_PARSING_CANCELLED, im, "Une erreur est survenue pendant le parsing qui a forcé l'arrêt du programme.");
    return NULL;
}

Image *parse_start(char *fileName)
{
    FILE *f = fopen(fileName, "r");
    if (!f)
        goto errorFileOpening;

    Image *im = (Image *)malloc(sizeof(Image));
    init_image(im);
    parse_head(im, f);

    fclose(f);
    return im;

errorFileOpening:
    set_error(ERROR_FILE_OPENING, NULL, "Problème pedant l'ouverture du fichier.");
    return NULL;
}

void print_parse_data(ParseData *data)
{
    printf("Parse Date : \n");
    printf("ambient : ");
    print_triplet(data->ambient);
    printf("diffuse : ");
    print_triplet(data->diffuse);
    printf("specular : ");
    print_triplet(data->specular);
    printf("shininess : %d\n", data->shininess);
    printf("maxverts : %d\n", data->maxverts);
    printf("nbVertex : %d\n", data->nbVertex);
    if (data->nbVertex)
    {
        printf("vertex : \n");
        for (int i = 0; i < data->nbVertex; i += 1)
        {
            printf("Vertex %d : ", i);
            print_triplet(data->vertex[i]);
        }
    }
}

void free_parse_data(ParseData *data)
{
    if (!data)
        return;
    if (data->vertex)
        free(data->vertex);
    free(data);
}
