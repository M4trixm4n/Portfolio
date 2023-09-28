#include "error_handler.h"

void set_error(Error err, Image *im, char *message)
{
    switch (err)
    {
    case ERROR_MISSING_ARGUMENT:
        fprintf(stderr, "Error: [PARSE] MISSING ARGUMENT \n %s\n", message);
        break;
    case ERROR_TOO_MANY_ARGUMENTS:
        fprintf(stderr, "Error: [PARSE] TOO MANY ARGUMENT \n %s\n", message);
        break;
    case ERROR_INVALID_VALUE:
        fprintf(stderr, "Error: INVALID VALUE \n %s\n", message);
        break;
    case ERROR_INVALID_TYPE:
        fprintf(stderr, "Error: [PARSE] INVALID TYPE \n %s\n", message);
        break;
    case ERROR_ALLOCATION_FAILED:
        fprintf(stderr, "Error: ALLOCATION FAILED \n %s\n", message);
        break;
    case ERROR_MISSING_VERTEX_LIMIT:
        fprintf(stderr, "Error: [PARSE] MISSING VERTEX LIMIT \n %s\n", message);
        break;
    case ERROR_OUT_OF_BOUNDS:
        fprintf(stderr, "Error: OUT OF BOUNDS \n %s\n", message);
        break;
    case ERROR_INVALID_KEYWORD:
        fprintf(stderr, "Error: [PARSE] INVALID KEYWORD \n %s\n", message);
        break;
    case ERROR_INVALID_ARGUMENT:
        fprintf(stderr, "Error: INVALID ARGUMENT \n %s\n", message);
        break;
    case ERROR_PARSING_CANCELLED:
        fprintf(stderr, "Error: [PARSE] PARSING CANCELLED \n %s\n", message);
        break;
    case ERROR_INVALID_HEADER:
        fprintf(stderr, "Error: [PARSE] INVALID HEADER \n %s\n", message);
        break;
    case ERROR_IMPOSSIBLE:
        fprintf(stderr, "Error: IMPOSSIBLE \n %s\n", message);
        break;
    case ERROR_ALREADY_DEFINED:
        fprintf(stderr, "Error: [PARSE] ALREADY DEFINED \n %s\n", message);
        break;
    case ERROR_FILE_OPENING:
        fprintf(stderr, "Error: [PARSE] FILE OPENING \n %s\n", message);
        break;
    case ERROR_NULL_VECTOR:
        fprintf(stderr, "Error: [PARSE] NULL VECTOR \n %s\n", message);
        break;
    case ERROR_MISSING_SIZE:
        fprintf(stderr, "Error: [PARSE] MISSING SIZE \n %s\n", message);
        break;
    case ERROR_UNTIMELY_CALL:
        fprintf(stderr, "Error: [PARSE] UNTIMELY CALL \n %s\n", message);
        break;
    case ERROR_SAVING_FAILED:
        fprintf(stderr, "Error: SAVING ISSUE \n %s\n", message);
        break;
    case ERROR_LOADING_FAILED:
        fprintf(stderr, "Error: LOADING ISSUE \n %s\n", message);
        break;
    case ERROR_DIFFERENT_SIZE:
        fprintf(stderr, "Error: DIFFERENT SIZE \n %s\n", message);
        break;
    default:
        fprintf(stderr, "Unknown error: %d\n%s\n", err, message);
        break;
    }
}
