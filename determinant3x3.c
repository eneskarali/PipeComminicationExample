#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main(void)
{
    while (1)
    {
        char input[10];       //input alırken her bir satir girdisini saklamak icin kullanılır
        int matrix[3][3];     //üzerinde islemleri yapacagımız matris
        char *pt;             //girilen değerler parçalanırken burada tutulur
        int p[2];             //pipe okuma/yazma dosyaları için tanımlama
        int c;                //exec ile calistirilan program
        int secProgValue = 0; //rastgele dönen deger
        int kofakRes = 0;     //program her calistiginda donen degeri tutar
        int totalRes = 0;     //donen kofak degerlerinin toplamı tutulur

        printf("------------------------------------------");
        printf("\n3x3 Bir Matris Olusturunuz\n\n");
        int inputForIndex = 0;
        for (inputForIndex = 0; inputForIndex < 3; inputForIndex++)
        {
            printf("%d. Satiri Giriniz: ", (inputForIndex + 1));
            fgets(input, 10, stdin);

            int ptIndex = 0; //girilen degerleri parcalamak icin tutulan index
            pt = strtok(input, " ");
            while (pt != NULL)
            {
                if (ptIndex < 3)
                {
                    matrix[inputForIndex][ptIndex] = atoi(pt); // girilen deger bolup 3x3 martix e atar
                }
                else if (ptIndex >= 3) // 3 ten fazla eleman kontrolu
                {
                    printf("3 eleman girmelisiniz!\n");
                    printf("%d. Satiri Giriniz: ", (inputForIndex + 1));
                    fgets(input, 10, stdin);
                    ptIndex = 0;
                    pt = strtok(input, " ");
                }
                pt = strtok(NULL, " ");
                ptIndex++;
            }
        }
        printf("------------------------------------------");

        if (pipe(p) < 0) //veri transferi icin pipe baslatılıyor
        {
            perror("Pipe  error: ");
            exit(1);
        }

        int pid = fork(); //rastgele sayı ureten programı calistirmek icin fork islemi

        if (pid == 0)
        {
            c = execv("sec", NULL); // satsutsec programı calisir ve pipe a rastgele 0-5 arası bir deger yazar
            perror("exec satsunsec: ");
            exit(0);
        }
        else
        {
            wait(&c); //satsutsec programının calismasini tamamlamasini bekler
        }
        read(p[0], &secProgValue, sizeof(int)); // donen deger 0-5 aralığında 2 den buyuk olanların %3 u sutun belirtiyor
        if (secProgValue < 3)                   //donen deger 3 ten kucukse satir islemleri yapilir
        {
            printf("\nSecilen Satir/Sutun: %d. Satir \n", secProgValue + 1);
            int u = 0;
            for (u = 0; u < 3; u++)
            {
                int i, j, i2 = 0, j2 = 0;
                int matrix2[2][2];
                write(p[1], &matrix[secProgValue][u], sizeof(int)); //secilen satir elemanını gonder

                for (i = 0; i < 3; i++)
                {
                    if (i == secProgValue) //silinen satırı okuma
                    {
                        continue;
                    }
                    for (j = 0; j < 3; j++)
                    {
                        if (j == u) // silinen sutunu okuma
                        {
                            continue;
                        }
                        matrix2[i2][j2] = matrix[i][j]; // okunan degerleri yeni 2x2 matrix e at
                        j2++;
                    }
                    j2 = 0;
                    i2++;
                }
                i2 = 0;
                pid = fork(); //kofak hesabı yapan programı baslatmak icin fork islemi
                if (pid == 0)
                {
                    write(p[1], &matrix2, 4 * sizeof(int)); //satir cikartildiktan sonra kalan matrisi gonder
                    int po = 0;
                    po = (secProgValue + 1) + (u + 1); //ust degerini tutar
                    write(p[1], &po, sizeof(int));     // ust degerini pipe a yazar
                    c = execv("ko", NULL);             //kofaktor hesabı yapan programı calistirir
                    perror("exec kofaktor error: ");
                    exit(0);
                }
                else
                {
                    wait(&c); //kofaktor hesabu yapan programın tamamlanmasını bekler
                }

                read(p[0], &kofakRes, sizeof(int)); //hesaplanan degerleri okur
                totalRes = totalRes + kofakRes;     // okunan degeleri toplayarak toplam sonucu bulur
            }
            printf("\nHesaplanan Determinant: %d \n", totalRes);
        }
        else if (secProgValue > 2) // rastgele gelen deger 3 ten buyukse yapılacak sutun islemleri
        {                          //sutun
            printf("\nSecilen Satir/Sutun: %d. Sutun \n", (secProgValue % 3) + 1);
            int u = 0;
            for (u = 0; u < 3; u++)
            {
                int i, j, i2 = 0, j2 = 0;
                int matrix2[2][2];
                write(p[1], &matrix[u][secProgValue % 3], sizeof(int)); //silinecek sutun dan gelen degerleri pipe a yazar

                for (i = 0; i < 3; i++)
                {
                    if (i == u) //silinin satiri okuma
                    {
                        continue;
                    }
                    for (j = 0; j < 3; j++)
                    {
                        if (j == secProgValue % 3) // silinen sutunu okuma
                        {
                            continue;
                        }
                        matrix2[i2][j2] = matrix[i][j]; // okunan degeleri yeni 2x2 matix e at
                        j2++;
                    }
                    j2 = 0;
                    i2++;
                }
                i2 = 0;
                pid = fork(); //kofaktor hesabı yapacak programın calismasi icin fork islemi
                if (pid == 0)
                {
                    write(p[1], &matrix2, 4 * sizeof(int)); //satir cikartildiktan sonra kalan matrisi gonder
                    int po = 0;
                    po = ((secProgValue % 3) + 1) + (u + 1); //ust degerini hesapla
                    write(p[1], &po, sizeof(int));           //ust degerini pipe a yaz
                    c = execv("ko", NULL);                   //kofaktor hesabı yapan programı calistir
                    perror("exec kofaktor error: ");
                    exit(0);
                }
                else
                {
                    wait(&c); //kofaktor hesabı yapan programın calismasını tamamlasını bekle
                }

                read(p[0], &kofakRes, sizeof(int)); //hesaplanan degeleri oku
                totalRes = totalRes + kofakRes;     // okunan degeleri topla
            }
            printf("\nHesaplanan Determinant: %d \n", totalRes); //sonucu ekrana yazdır
        }
        close(p[0]);
        close(p[1]);
    }
    return 0;
}
