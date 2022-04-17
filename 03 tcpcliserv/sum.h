#include <netinet/in.h>

#include "lib.h"
#define SERV_PORT       2022

struct args {
        long    arg1;
        long    arg2;
};
struct result {
        long    sum;
};