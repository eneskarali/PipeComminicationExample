#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int recivedMatrix[2][2]; // alinan 2x2 dizi
    int recivedValue = 0; // alinan satir/sutun elemanı
    int po = 0;   //ust
    int detRes = 0;  //determinant sonucu
    int res = 0;    // yazilacak sonuc

    read(3 , &recivedValue , sizeof(int));  //satir/sutun degerini oku
    read(3 , &recivedMatrix , 4*sizeof(int)); //oluşturulan 2x2 matrisi oku
    read(3 , &po , sizeof(int));  //satir+sutun oku

    detRes = (recivedMatrix[0][0]*recivedMatrix[1][1]) - (recivedMatrix[0][1]*recivedMatrix[1][0]);  //2x2 matis determinantını hesapla
    
    int minusPow = 1;  //eksi arı ifade icin
    int it = 0;
    for(it = 0; it < po ; it++){
        minusPow = minusPow*(-1);
    }
    res = minusPow*recivedValue*detRes;  //sonucu hesapla
    write(4,&res,sizeof(int)); // sonucu pipe a yaz

    return 0;
}
