#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <math.h>


int main()
{
    int random = 0;

    srand(time(NULL));
    random = (rand() % 6);  //rastgele deger uret
    srand(time(NULL));
    
    write(4,&random,sizeof(int)); //uretilen degeri pipe a yaz
    
    return 0;
}
