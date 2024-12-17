//
// Created by Logib on 2024/12/17.
//

#ifndef KEYRHYTHM_CHART_H
#include "Chart.h"
#endif


/**
 * @class User
 * @brief initialize user data
 */
User :: User(){
    memset(this ->userName, 0, sizeof(char )*13);
    memset(this ->userName, 0, sizeof(char )*21);
}

/**
 * @class User
 * @brief to save user data
 * @param Saving (\c FILE )
 */
void User :: save(FILE *Saving){
    if (this ->userName[0] == '\0') throw ChartError(7);
    if (this ->userName[12] != '\0') throw ChartError(8);
    putc((int )'\n', Saving);
    putWords(Saving, this ->userName);
    putc((int )'\n', Saving);
    char *encode = base64_encode(this ->Password);
    putWords(Saving, encode);
    putc((int )'\n', Saving);
    free(encode);
}

/**
 * @class User
 * @brief to check if this is new user
 * @param Saving (\c FILE )
 * @return \c bool (1 = new, 0 = old)
 */
bool User :: newUser(FILE *Saving) {
    fseek(Saving, 0, SEEK_SET);
    return !( getKeyWord(Saving, this ->userName) );
}

/**
 * @class User
 * @brief to check whether the user's password is right
 * @param Saving (\c FILE )
 * @return \c bool (1 = right, 0 = wrong)
 */
bool User :: correct(FILE *Saving) {
    fseek(Saving, 0, SEEK_SET);
    if ( !getKeyWord(Saving, this ->userName) ) throw ChartError(9);
    char *tmp = new char [257];
    memset(tmp, 0, sizeof (char )*257);
    int t = 0;
    char ch = (char )getc(Saving);
    while (ch != '=' && !(ch > '0'-1 && ch < '9'+1)
           && !(ch > 'A'-1 && ch < 'Z'+1) && !(ch > 'a'-1 && ch < 'z'+1))
        ch = (char )getc(Saving);

    while (ch == '=' || (ch > '0'-1 && ch < '9'+1)
           || (ch > 'A'-1 && ch < 'Z'+1) || (ch > 'a'-1 && ch < 'z'+1)){
        tmp[t++] = ch;
        ch = (char )getc(Saving);
    }
    tmp[t]='\0';
    char *decode = base64_decode(tmp);
    delete[] tmp;
    if ( strcmp(decode, this ->Password) != 0){
        free(decode);
        return false;
    }
    free(decode);
    return true;
}
