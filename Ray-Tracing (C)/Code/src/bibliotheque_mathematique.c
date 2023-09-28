#include "bibliotheque_mathematique.h"

Triplet addition(Triplet t1, Triplet t2)
{
    Triplet t;
    t.error = 0;
    if ((t1.type == 0 && t2.type == 0) || ((t1.type != t2.type) && (t1.type == 2 || t2.type == 2)))
    {
        t.error = 1;
    }
    t.x = t1.x + t2.x;
    t.y = t1.y + t2.y;
    t.z = t1.z + t2.z;
    t.type = t1.type;
    return t;
}

Triplet substraction(Triplet t1, Triplet t2)
{
    Triplet t;
    t.error = 0;
    if (t1.type != t2.type || t1.type == 2 || t2.type == 2)
    {
        t.error = 1;
    }
    t.x = t1.x - t2.x;
    t.y = t1.y - t2.y;
    t.z = t1.z - t2.z;
    t.type = 1;
    return t;
}

Triplet scalar_multiplication(Triplet t1, double d)
{
    Triplet t;
    t.x = d * t1.x;
    t.y = d * t1.y;
    t.z = d * t1.z;
    t.type = t1.type;
    t.error = 0;
    return t;
}

double scalar_product(Triplet t1, Triplet t2)
{
    if (t1.type == 1 && t2.type == 1)
        return t1.x * t2.x + t1.y * t2.y + t1.z * t2.z;
    return -1.0 * (t1.x * t2.x + t1.y * t2.y + t1.z * t2.z);
}

int scalar_product_test(Triplet t1, Triplet t2)
{
    return scalar_product(t1, t2) == t1.x * t2.x + t1.y * t2.y + t1.z * t2.z;
}

Triplet vector_product(Triplet t1, Triplet t2)
{
    Triplet t;
    t.error = 0;
    if (t1.type != t2.type || t1.type != 1 || t2.type != 1)
    {
        t.error = 1;
    }
    t.x = (t1.y * t2.z) - (t1.z * t2.y);
    t.y = (t1.z * t2.x) - (t1.x * t2.z);
    t.z = (t1.x * t2.y) - (t1.y * t2.x);
    t.type = t1.type;
    return t;
}

Triplet schur_product(Triplet t1, Triplet t2)
{
    Triplet t;
    t.error = 0;
    if (t1.type != t2.type || t1.type != 2 || t2.type != 2)
    {
        t.error = 1;
    }
    t.x = t1.x * t2.x;
    t.y = t1.y * t2.y;
    t.z = t1.z * t2.z;
    t.type = t1.type;
    return t;
}

double length(Triplet t)
{
    if (t.type == 1)
        return sqrt((t.x * t.x) + (t.y * t.y) + (t.z * t.z));
    return -1.0;
}

Triplet norm(Triplet t)
{
    Triplet res;
    res.error = 0;
    if (t.type != 1)
        res.error = 1;
    res = scalar_multiplication(t, (1.0 / length(t)));
    return res;
}

void print_triplet(Triplet t)
{
    if (!t.error)
    {
        switch (t.type)
        {
        case 0:
            printf("P ");
            break;
        case 1:
            printf("V ");
            break;
        case 2:
            printf("C ");
            break;
        default:
            break;
        }
        if (fmod(t.x, 1))
            printf("%.9g ", t.x);
        else
            printf("%.1f ", t.x);
        if (fmod(t.y, 1))
            printf("%.9g ", t.y);
        else
            printf("%.1f ", t.y);
        if (fmod(t.z, 1))
            printf("%.9g\n", t.z);
        else
            printf("%.1f\n", t.z);
    }
    else
    {
        printf("Interdit");
    }
}

void print_res_triplet_1(Triplet t1, Triplet t2, float d, char *op, int j)
{
    if (!strcmp("add", op))
        print_triplet(addition(t1, t2));
    else if (!strcmp("sub", op))
        print_triplet(substraction(t1, t2));
    else if (!strcmp("mul", op))
        j == 1 ? print_triplet(scalar_multiplication(t1, d)) : print_triplet((Triplet){0, 0, 0, 0, 1});
    else if (!strcmp("dot", op))
        scalar_product_test(t1, t2) ? printf("%.1f", scalar_product(t1, t2)) : printf("Interdit");
}

void print_res_triplet_2(Triplet t1, Triplet t2, float d, char *op)
{
    if (!strcmp("cross", op))
        print_triplet(vector_product(t1, t2));
    else if (!strcmp("times", op))
        print_triplet(schur_product(t1, t2));
    else if (!strcmp("len", op))
        length(t1) == -1.0 ? printf("Interdit") : printf("%.1f", length(t1));
    else if (!strcmp("hat", op))
        print_triplet(norm(t1));
}

void change_t1_t2_check_triplet(Triplet *t1, Triplet *t2, int i, int j, int type, float x, float y, float z)
{
    if (j != 1)
    {
        if (i == 0)
        {
            t1->x = x;
            t1->y = y;
            t1->z = z;
            t1->type = type;
            t1->error = 0;
        }
        else
        {
            t2->x = x;
            t2->y = y;
            t2->z = z;
            t2->type = type;
            t2->error = 0;
        }
    }
}

void determine_values_check_triplet(char *token, float *d, int j, int *type, float *x, float *y, float *z)
{
    if (j == 0)
    {
        *d = atof(token);
        switch (token[0])
        {
        case 'P':
            *type = 0;
            break;
        case 'V':
            *type = 1;
            break;
        case 'C':
            *type = 2;
            break;
        }
    }
    else if (j == 1)
        *x = atof(token);
    else if (j == 2)
        *y = atof(token);
    else
        *z = atof(token);
}

void parse_check_triplet(char *input)
{
    Triplet t1, t2;
    char *t1_ch, *t2_ch, *op;
    int i = 0, j, type;
    float x, y, z, d;
    char *separators = ",";
    char *separators_int = " ";
    char *token = strtok(input, separators);
    while (token)
    {
        if (i == 0)
        {
            t1_ch = (char *)malloc((strlen(token) + 1) * sizeof(char));
            strcpy(t1_ch, token);
        }
        if (i == 2)
        {
            t2_ch = (char *)malloc((strlen(token) + 1) * sizeof(char));
            strcpy(t2_ch, token);
        }
        else
        {
            op = (char *)malloc((strlen(token) + 1) * sizeof(char));
            strcpy(op, token);
        }
        token = strtok(NULL, separators);
        i++;
    }
    for (i = 0; i < 2; i++)
    {
        if (i == 0)
            token = strtok(t1_ch, separators_int);
        else
            token = strtok(t2_ch, separators_int);
        j = 0;
        while (token)
        {
            determine_values_check_triplet(token, &d, j, &type, &x, &y, &z);
            token = strtok(NULL, separators_int);
            j++;
        }
        change_t1_t2_check_triplet(&t1, &t2, i, j, type, x, y, z);
    }
    print_res_triplet_1(t1, t2, d, op, j);
    print_res_triplet_2(t1, t2, d, op);

    free(t1_ch);
    free(t2_ch);
    free(op);
}

int equals_triplet(Triplet t1, Triplet t2)
{
    if (t1.x == t2.x && t1.y == t2.y && t1.z == t2.z)
        return 1;
    return 0;
}
