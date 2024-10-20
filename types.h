#ifndef TYPES_H
#define TYPES_H

typedef unsigned int uint;

typedef enum
{
    failure,
    success
}Status;
typedef enum
{
    view,
    edit,
    help,
    unsupported
}Operationtype;

#endif