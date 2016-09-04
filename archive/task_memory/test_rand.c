#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main()
{
int i;
srand(time(NULL));
for(i =0;i <10000;++i)
printf("%u \n",rand());
return 0;
}
