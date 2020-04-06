//
// Created by wavator on 2020/3/31.
//

#include <time.h>
#include <stdio.h>

int main(int argc, char ** argv)
{

    clock_t start = clock();
    while (1)
    {
        if (clock() - start > CLOCKS_PER_SEC / 9) // It is greater than 1/10 s.
        {
            break;
        }
    }
    return !printf("run: %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
}

