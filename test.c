/*#include <stdio.h>
#include "ArrayList.h"

int main(int argc, char **argv)
{
    ArrayList *myArrayList = newArrayList();

    int a = 102;
    myArrayList->add(myArrayList, &a);

    int b = 110;
    myArrayList->add(myArrayList, &b);

    int c = 11;
    myArrayList->add(myArrayList, &c);

    int d = 103;
    myArrayList->add(myArrayList, &d);

    int *e = (int *)myArrayList->indexOf(myArrayList, 3);

    printf("res = %d", *e);

    myArrayList->destroy(myArrayList);

    return 0;
}
'/