#include "comparateur_images.h"
#include "generator.h"

int main(int argc, char *argv[])
{
    Image *image;
    if (!strcmp(argv[1], "compare"))
    {
        compare(argv[2], argv[3]);
        return 0;
    }
    else if (!strcmp(argv[1], "generate"))
    {
        image = generate_all(argv[2]);
    }
    else if (!strcmp(argv[1], "scene"))
    {
        image = parse_start(argv[2]);
        test_scene(image);
        return 0;
    }
    else if (!strcmp(argv[1], "checkT"))
    {
        parse_check_triplet(argv[2]);
        return 0;
    }
    generate_image(image);
    free_image(image);
    return 0;
}
