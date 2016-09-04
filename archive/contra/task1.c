#include <stdio.h>

typedef long int li;

li convert(li x, li y, li n, li p)
{
    y %= (1 << n);
    li c1 = x;
    c1 = (c1 >> (32- (p - n + 1)));
    c1 = (c1 << (32- (p - n + 1)));
    y = (y << (32 - p));
    c1 += y;
    li c2 = x;
    c2 = x % (1 <<(32 - p));
    c1 += c2;
    return c1;
}

int main()
{
    long int x=0xffffaaaaffffaaaa;
    long int y=0xccccbbbbccccbbbb;
    
    printf("x = %lx\n",x);
    printf("y = %lx\n",y);
    printf("r = %lx\n",convert(x,y,16,12));

    return 0;
}


