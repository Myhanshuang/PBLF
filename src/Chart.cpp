//
// Created by Logib on 2024/12/11.
//

#ifndef KEYRHYTHM_CHART_H
#include "../inc/Chart.h"
#endif



int MaxOffset[8] = {16, 64-27, 70, 100, 151-27, InfOffset, InfOffset};
int KeyCode[9] = {68, 70, 74, 75, 0, 0, 0, 0, 0};

/**
 * @class Chart
 * @brief initialize Chart with param
 * @param Keys
 ***/
Chart :: Chart(const short Keys){
    this ->Column = Keys;
    this ->ChartHead = nullptr;
    this ->Acting = new Chart :: ChartAct;
    wcsset(this ->songTitle, L'\0');
    wcsset(this ->Artist, L'\0');
}

/**
 * @class Chart
 * @brief constructor of Chart
 ***/
Chart :: Chart(){
     this ->Column = 4;
     this ->ChartHead = nullptr;
    this ->Acting = new Chart :: ChartAct;
    wcsset(this ->songTitle, L'\0');
    wcsset(this ->Artist, L'\0');
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
    this ->beatsPerMinute = C.beatsPerMinute;
    this ->everyBeat = C.everyBeat;
    this ->noteCount = C.noteCount;
    if (C.Acting != nullptr){
        this ->Acting = new Chart :: ChartAct;
        *this ->Acting = *(C.Acting);
    }
    else this ->Acting = C.Acting;
    wcsset(this ->songTitle, L'\0');
    wcsset(this ->Artist, L'\0');
    wcscpy(this ->songTitle, C.songTitle);
    wcscpy(this ->Artist, C.Artist);
    /*
    if (C.songTitle != nullptr){
        this ->songTitle = new wchar_t [wcslen(C.songTitle)+2];
        memcpy(this ->songTitle, C.songTitle, sizeof (wchar_t )*(wcslen(C.songTitle)+2));
        this ->songTitle[wcslen(C.songTitle)] = this ->songTitle[wcslen(C.songTitle)+1] = L'\0';
    }
    if (C.Artist != nullptr){
        this ->Artist = new wchar_t [wcslen(C.Artist)+2];
        memcpy(this ->Artist, C.Artist, sizeof (wchar_t )*(wcslen(C.Artist)+2));
        this ->Artist[wcslen(C.Artist)] = this ->Artist[wcslen(C.Artist)+1] = L'\0';
    }*/
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
        ChartHead = nullptr;
    }
    wcsset(this ->songTitle, L'\0');
    wcsset(this ->Artist, L'\0');
    /*delete[] this ->songTitle;
    delete[] this ->Artist;*/
    delete Acting;
    this ->beatsPerMinute = this ->everyBeat = this ->accPerNote = 0.0l;
    this ->noteCount = this ->Offset = 0;
    this ->Column = 0;
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

/**
 * @class ChartAct
 * @brief constructor of ChartAct
 */
Chart :: ChartAct :: ChartAct(){
    memset(this ->judgeResult, 0, sizeof (int )*9);
}

/**
 * @class ChartAct
 * @brief initialize new ChartAct with old one
 * @param CA
 */
Chart :: ChartAct :: ChartAct(const ChartAct &CA){
    this ->Score = CA.Score;
    this ->Accuracy = CA.Accuracy;
    this ->Combo = CA.Combo;
    this ->maxCombo = CA.maxCombo;
    memcpy(this ->judgeResult, CA.judgeResult, sizeof (int )*9);
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
