#include <cstdio>
#include <cstring>
#include "Chart.h"

using namespace std;

int main() {
    FILE *fp = fopen("1689619069.mc", "r");
    printf("\n\n");
    FILE *t = fopen("out.txt", "w");
    Chart A;
    getChart(fp, A);
    for (auto ptr = A.ChartHead;  ptr != nullptr ; ptr = ptr ->NxtMea) {
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
    }
    return 0;
}