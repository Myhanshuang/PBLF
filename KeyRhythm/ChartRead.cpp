//
// Created by Logib on 2024/12/11.
//

#ifndef _GLIBCXX_CSTDIO
#include <cstdio>
#endif //_GLIBCXX_CSTDIO

#ifndef _GLIBCXX_CSTRING
#include <cstring>
#endif //_GLIBCXX_CSTRING

#ifndef _GLIBCXX_CSTDLIB
#include <cstdlib>
#endif //_GLIBCXX_CSTDLIB

#ifndef _GLIBCXX_CTIME
#include <ctime>
#endif

#ifndef _GLIBCXX_IOSTREAM
#include <iostream>
#endif //_GLIBCXX_IOSTREAM

#ifndef _DIRENT_H_
#include <dirent.h>
#endif

#ifndef _INC_TYPES
#include <sys/types.h>
#endif

#ifndef _INC_STAT
#include <sys/stat.h>
#endif

#ifndef NO_ERROR
#include <error.h>
#endif

#ifndef KEYRHYTHM_CHARTERROR_H
#include "ChartError.h"
#endif //KEYRHYTHM_CHARTERROR_H

/**
 * @brief get value in file (time_t)
 * @param File
 * @param val
 */
void getValueTT(FILE *File, time_t & val){
    char ch = (char )getc(File);
    if (ch == EOF) return ;
    while (ch > '9' || ch < '0'){
        ch = (char )getc(File);
        if (ch == EOF) return ;
    }
    val = 0;
    while (ch < '9'+1 && ch > '0'-1){
        val = (val << 3) + (val << 1) + ch - '0';
        ch = (char )getc(File);
    }
}

/**
 * @brief get value in file (int)
 * @param File
 * @param val
 */
void getValueInt(FILE *File, int& val){
    char ch = (char )getc(File);
    if (ch == EOF) return ;
    while (ch > '9' || ch < '0'){
        ch = (char )getc(File);
        if (ch == EOF) return ;
    }
    val = 0;
    while (ch < '9'+1 && ch > '0'-1){
        val = (val << 3) + (val << 1) + ch - '0';
        ch = (char )getc(File);
    }
}

/**
 * @brief get value in file (short)
 * @param File
 * @param val
 */
void getValueShort(FILE *File, short & val){
    char ch = (char )getc(File);
    if (ch == EOF) return ;
    while (ch > '9' || ch < '0'){
        ch = (char )getc(File);
        if (ch == EOF) return ;
    }
    val = 0;
    while (ch < '9'+1 && ch > '0'-1){
        val = static_cast <short>((val << 3) + (val << 1) + ch - '0');
        ch = (char )getc(File);
    }
}

/**
 * @brief get value in file (short)
 * @param File
 * @param val
 */
void getValueLDouble(FILE *File, long double& val){
    val = 0.0l;
    char ch = (char )getc(File);
    while (ch > '9' || ch < '0') ch = (char )getc(File);
    while (ch < '9'+1 && ch > '0'-1){
        val = val*10.0l + ch - '0';
        ch = (char )getc(File);
    }
    if (ch != '.') return ;
    long double p = 0.1;
    ch = (char )getc(File);
    while (ch < '9'+1 && ch > '0'-1){
        val += (ch - '0')*p;
        p/=10.0l;
        ch = (char )getc(File);
    }
}

/**
 * @brief use KMP to get the position of the keyword
 * @param File
 * @param KeyWord
 * @return \c bool ( 1 = found )
 ***/
bool getKeyWord(FILE *File, const char *KeyWord){
    char c = '\0';
    auto Length = static_cast <short>(strlen(KeyWord));
    auto nxt = new short [Length+1];
    nxt[0] = -1;
    for (short i = 0, j = -1; i < Length-1; ){
        if (j == -1 || KeyWord[i] == KeyWord[j]){
            ++i;
            ++j;
            nxt[i] = j;
        }
        else j = nxt[j];
    }
    short j = -1;
    while (!feof(File)){
        if (j == -1 || c == KeyWord[j]){
            c = (char )fgetc(File);
            ++j;
        }
        else j = nxt[j];
        if (j == Length){
            delete[] nxt;
            return true;
        }
    }
    delete[] nxt;
    return false;
}

/**
 * @brief use KMP to get the position of the keyword, with wide char
 * @param File
 * @param KeyWord
 * @return \c bool ( 1 = found )
 ***/
bool getKeyWord_w(FILE *File, const wchar_t *KeyWord){
    wchar_t c = L'\0';
    auto Length = static_cast <short>(wcslen(KeyWord));
    auto nxt = new short [Length+1];
    nxt[0] = -1;
    for (short i = 0, j = -1; i < Length-1; ){
        if (j == -1 || KeyWord[i] == KeyWord[j]){
            ++i;
            ++j;
            nxt[i] = j;
        }
        else j = nxt[j];
    }
    short j = -1;
    while (!feof(File)){
        if (j == -1 || c == KeyWord[j]){
            c = (wchar_t )fgetwc(File);
            ++j;
        }
        else j = nxt[j];
        if (j == Length){
            delete[] nxt;
            return true;
        }
        if (c == WEOF){
            if (errno == EILSEQ) throw ChartError(11);
            if (ferror(File)) throw ChartError(12);
            return false;
        }
    }
    delete[] nxt;
    return false;
}

/**
 * @brief use multiple KMP to get the position of the keywords
 * @param File
 * @param KeyWord1
 * @param KeyWord2
 * @return \c short ( 1 = found 1, 2 = found 2, 0 = all not found)
 ***/
short getKeyWords(FILE *File, const char *KeyWord1, const char *KeyWord2){
    char c = '\0';
    auto Length1 = static_cast <short>(strlen(KeyWord1)), Length2 = static_cast <short>(strlen(KeyWord2));
    auto nxt1 = new short [Length1+1];
    auto nxt2 = new short [Length2+1];
    nxt1[0] = -1, nxt2[0] = -1;
    for (short i = 0, j = -1; i < Length1-1; ){
        if (j == -1 || KeyWord1[i] == KeyWord1[j]){
            ++i;
            ++j;
            nxt1[i] = j;
        }
        else j = nxt1[j];
    }
    for (short i = 0, j = -1; i < Length2-1; ){
        if (j == -1 || KeyWord2[i] == KeyWord2[j]){
            ++i;
            ++j;
            nxt2[i] = j;
        }
        else j = nxt2[j];
    }
    short j1 = -1, j2 = -1;
    bool f1, f2;
    while (!feof(File)){
        f1 = (j1 == -1 || c == KeyWord1[j1]);
        f2 = (j2 == -1 || c == KeyWord2[j2]);
        if (f1 && f2){
            c = (char )fgetc(File);
            ++j1;
            ++j2;
        }
        else {
            while (!f1){
                j1 = nxt1[j1];
                f1 = (j1 == -1 || c == KeyWord1[j1]);
            }
            while (!f2){
                j2 = nxt2[j2];
                f2 = (j2 == -1 || c == KeyWord2[j2]);
            }
        }
        if (j1 == Length1){
            delete[] nxt1;
            delete[] nxt2;
            return 1;
        }
        if (j2 == Length2){
            delete[] nxt1;
            delete[] nxt2;
            return 2;
        }
    }
    delete[] nxt1;
    delete[] nxt2;
    return 0;
}


/** BUG UNDER but fixed */
/** Which made me surprised is wchar_t doesn't have a well support for "new" */
/** I mean, it is a piece of shit, totally */

/**
 * @brief to get wide chars from file, which use \" to sign start and end;
 * @param File
 * @param src
 */
void getWords_w(FILE *File, wchar_t *src){
    wcsset(src, L'\0');
    wchar_t tmp[5];
    wcsset(tmp, L'\0');
    char c = (char )fgetc(File);
    while (c!='\"') c = (char )fgetc(File);
    wchar_t wc = fgetwc(File);
    while (wc != L'\"'){
        if (wc == WEOF){
            if (errno == EILSEQ) throw ChartError(11);
            throw ChartError(12);
        }
        wcsset(tmp, L'\0');
        tmp[0] = wc;
        putwchar(wc);
        wcscat(src, tmp);
        wc = fgetwc(File);
    }
}

void searchInit(const char *chartPath){
    DIR *ptr = opendir(chartPath);
    struct dirent *now = readdir(ptr);
    while (strcmp(now->d_name, "..")!=0) now = readdir(ptr);
    now = readdir(ptr);


}