//
// Created by Logib on 2024/12/11.
//

#ifndef KEYRHYTHM_CHART_H
#include "Chart.h"
#endif

#include <cstring>

int MaxOffset[8] = {0};
int KeyCode[9] = {0};

/**
 * @class Chart
 * @brief initialize Chart with param
 * @param Keys
 ***/
Chart :: Chart(const short Keys){
    this ->Column = Keys;
    this ->ChartHead = nullptr;
    this ->Acting = new Chart :: ChartAct;
}

/**
 * @class Chart
 * @brief constructor of Chart
 ***/
Chart :: Chart(){
     this ->Column = 4;
     this ->ChartHead = nullptr;
}

/**
 * @class Chart
 * @brief create new chart with old one
 * @param C
 */
Chart ::Chart(const Chart &C) {
    this ->ChartHead = C.ChartHead;
    this ->Column = C.Column;
    this ->Offset = C.Offset;
    this ->BeatsPerMinute = C.BeatsPerMinute;
    this ->EveryBeat = C.EveryBeat;
    this ->NoteCount = C.NoteCount;
    this ->Acting = C.Acting;
}

/**
 * @class Chart
 * @brief destructor of Chart
 ***/
Chart :: ~Chart(){
    Chart :: Measure *temp = ChartHead, *preMea = nullptr;
    if (temp != nullptr){
        while(temp -> NxtMea != nullptr)
        {
            preMea = temp;
            temp = preMea ->NxtMea;
            preMea ->~Measure();
            delete preMea;
            return ;
        }
        delete ChartHead;
    }
}

/**
 * @class Measure
 * @brief initialize Measure with param
 * @param Keys
 ***/
Chart :: Measure :: Measure(const short Keys){
    this ->timeStamp = 0;
    this ->timeTable = nullptr;
    this ->NxtMea = nullptr;
    this ->Bar=new char [Keys];
    memset(this ->Bar, 0, sizeof (char )*Keys);
}

/**
 * @class Measure
 * @brief constructor of Measure
 ***/
Chart :: Measure :: Measure(){
    this ->timeStamp = 0;
    this ->timeTable = nullptr;
    this ->NxtMea = nullptr;
    this ->Bar=new char [4];
    memset(this ->Bar, 0, sizeof (char )*4);
}

/**
 * @class Measure
 * @brief destructor of Measure
 ***/
Chart :: Measure :: ~Measure(){
    delete[] this ->Bar;
    delete[] this ->timeTable;
    this ->Bar = nullptr;
    this ->timeTable = nullptr;
}

/**
 * @class Measure
 * @brief initialize new Measure with old one
 * @param M
 ***/
Chart :: Measure :: Measure(const Chart :: Measure &M) {
    this ->ifHold = M.ifHold;
    this ->timeStamp = M.timeStamp;
    short j = sizeof (M.Bar);
    this ->Bar = new char [j];
    memcpy(this ->Bar, M.Bar, j);

    if (M.ifHold){
        this ->timeTable = new int [j];
        memcpy(this ->timeTable, M.timeTable, j * sizeof (int ));
    }
    else this ->timeTable = nullptr;
    this ->NxtMea = M.NxtMea;
}

Chart :: ChartAct ::ChartAct(){
    memset(this ->judgeResult, 0, sizeof (int )*9);
}

/**
 * @class Result
 * @brief constructor of Result
 ***/
Result :: Result(){
    this ->Offset = -1;
    this ->Column = -1;
    this ->Judgment = -1;
    this ->ifHoldEnd = false;
}

/**
 * @class Result
 * @brief initialize new Result with old one
 * @param R
 */
Result :: Result(const Result &R) {
    this ->Offset = R.Offset;
    this ->Column = R.Column;
    this ->Judgment = R.Judgment;
    this ->ifHoldEnd = R.ifHoldEnd;
}

/**
 * @class Result
 * @brief set result
 * @param Key
 * @param Dis
 * @param HoldEnd
 */
Result :: Result(short Key, int Dis, bool HoldEnd) {
    this ->Column = Key;
    this ->Offset = Dis;
    this ->ifHoldEnd = HoldEnd;
    for (short i = 0; MaxOffset[i] != InfOffset; ++i){
        if (Dis > MaxOffset[i]) continue;
        this ->Judgment = i;
    }
}

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
