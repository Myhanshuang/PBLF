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
    User uu;
    scanf("%s", uu.userName);
    scanf("%s", uu.Password);
    Chart :: ChartAct CA;
    wchar_t chs[258]{};
    wcsset(chs, L'\0');

    FILE *f =fopen("in.txt", "r+");
    //printf("tt\n");
    getWords_w(f, chs);
    //printf("aa\n");
    printf("%ls\n", chs);
    UserSaveData USD(uu, CA, chs);
    USD.save("userdata/history");
    //printf("ss\n");
    return 0;
}