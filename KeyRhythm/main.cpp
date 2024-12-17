#include <cstdio>
#include "Chart.h"

using namespace std;

int main() {
    //FILE *fp = fopen("1689619069.mc", "r");
    FILE *t = fopen("out.txt", "w");
    FILE *u = fopen("user.txt", "w+");
    //Chart A;
    //agetChart(fp, A);
    /*for (auto ptr = A.ChartHead;  ptr != nullptr ; ptr = ptr ->NxtMea) {
        for (int i = 0; i < A.Column; ++i){
            switch (ptr ->Bar[i]) {
                case 0:
                {
                    fputc((int )'.', t);
                    fputc((int )'.', t);
                    fputc((int )'.', t);
                    fputc((int )'.', t);
                    break ;
                }
                case 1:
                {
                    fputc((int )'_', t);
                    fputc((int )'_', t);
                    break ;
                }
                case 2:
                {
                    fputc((int) '[', t);
                    fputc((int) ']', t);
                    break;
                }
            }
        }
        fprintf(t, "     %10d\n", ptr ->timeStamp);
    }*/
    User uu, uc;
    scanf("%s", uu.userName);
    printf("tt\n");
    scanf("%s", uu.Password);
    printf("aa\n");
    uu.save(u);
    printf("%s\n%s\n", uu.userName, uu.Password);
    if (!uu.newUser(u)) printf("00\n");
    scanf("%s", uc.userName);
    scanf("%s", uc.Password);
    printf("%s\n%s\n", uc.userName, uc.Password);
    if (!uc.newUser(u)) printf("0c\n");
    if (!uc.correct(u)) printf("ff\n");
    return 0;
}