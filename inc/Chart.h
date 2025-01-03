//
// Created by Logib on 2024/12/11.
//

/**
 * @author \b Logib \b
 *
 ***/

#ifndef KEYRHYTHM_CHART_H
#define KEYRHYTHM_CHART_H

/**
 * @brief to include some basic libs
 */
#ifndef _GLIBCXX_CSTDIO
#include <cstdio>
#endif //_GLIBCXX_CSTDIO

#ifndef _GLIBCXX_CSTRING
#include <cstring>
#endif //_GLIBCXX_CSTRING

#ifndef _GLIBCXX_CMATH
#include <cmath>
#endif //_GLIBCXX_CMATH

#ifndef _GLIBCXX_CWCHAR
#include <cwchar>
#endif //_GLIBCXX_CWCHAR

#ifndef _GLIBCXX_IOSTREAM
#include <iostream>
#endif //_GLIBCXX_IOSTREAM

#ifndef BASE64_H
//extern "C"{
#include "base64/base64.h"
//}
#endif

#ifndef FloatMinute
#define FloatMinute 60000.0f
///< @def Float Minute
#endif

#ifndef InfOffset
#define InfOffset 0x7ffffff
///< @def Infinity Offset
#endif

#ifndef BeatToTime
/**
 * @brief trans beat to timestamp
 *
 * @details according to [a, b, c] in .mz file
 * @param eb everyBeat
 * @param a which bar
 * @param b which beat/measure
 * @param c the division
 *
 * @return timestamp (int)
 */
#define BeatToTime(eb, a, b, c) (int )std::round(eb * (long double )a + eb / (long double )c * (long double )b)
#endif

extern int MaxOffset[8];
extern int KeyCode[9];

class Chart;


/**
 * @class Chart
 * @brief The chart data
 *
 * @details It included a head of \b metadata and several \b bar s
 ***/
class Chart{
public :
    long double beatsPerMinute = 0.0l, everyBeat = 0.0l, accPerNote = 0.0l;
    int noteCount = 0, Offset = 0;
    short Column = 0;
    class Measure;
    class ChartAct;
    Measure *ChartHead = nullptr;
    ChartAct *Acting = nullptr;

    wchar_t songTitle[258]{};
    wchar_t Artist[258]{};

    explicit Chart(short Keys);
    Chart(const Chart& C);
    Chart();
    ~Chart();

    Chart& operator =(Chart *Right){
        //memcpy(this, Right, sizeof (*Right) );
        this ->beatsPerMinute = Right ->beatsPerMinute;
        this ->everyBeat = Right ->everyBeat;
        this ->noteCount = Right ->noteCount;
        this ->Column = Right ->Column;
        this ->Offset = Right ->Offset;
        this ->ChartHead = Right ->ChartHead;
        this ->Acting = Right ->Acting;
        wcsset(this ->songTitle, L'\0');
        wcsset(this ->Artist, L'\0');
        wcscpy(this ->songTitle, Right ->songTitle);
        wcscpy(this ->Artist, Right ->Artist);
        return *this;
    }
};

/**
 * @class Measure
 * @brief The mini-est chart unit
 *
 * @details signed keys of every \b timestamp
 * about hold's end (if there is) and \b next Measure
 * \n it is a singly linked list
 *
 ***/
class Chart :: Measure{
public :
    bool ifHold = false;
    int timeStamp = 0;
    int *timeTable = nullptr;
    char *Bar = nullptr;
    Measure *NxtMea = nullptr;

    explicit Measure(short Keys);
    Measure();
    Measure(const Measure& M);
    ~Measure();

    void* operator new(size_t size, void* phead){
        return phead;
    }

    Measure& operator =(Measure *Right){
        this ->ifHold = Right ->ifHold;
        this ->timeStamp = Right ->timeStamp;
        this ->timeTable = Right ->timeTable;
        this ->Bar = Right ->Bar;
        return *this;
    }

};

/**
 * @class ChartAct
 * @brief the variable data in playing chart
 * @details judgeResult is used to save judging results
 * and can be used to save misses
 * \n or can use minus to show misses
 */
class Chart :: ChartAct{
public :
    long double Accuracy = 100.0l;
    int Combo = 0, Score = 0, maxCombo = 0;
    int judgeResult[9]{};

    ChartAct();
    ChartAct(const ChartAct &CA);

    ChartAct& operator =(ChartAct *Right){
        this ->Accuracy = Right ->Accuracy;
        this ->Combo = Right ->Combo;
        this ->Score = Right ->Score;
        this ->maxCombo = Right ->maxCombo;
        memcpy(this ->judgeResult, Right ->judgeResult, sizeof (int )*9);
        return *this;
    }
};

/**
 * @class Result
 * @brief to show the result of a note
 *
 * @details the result of judgment is variable, depend on the judge table
 ***/
class Result{
public :
    short Column = -1, Judgment = -1;
    int Offset = -1;
    bool ifHoldEnd;
    Result();
    Result(short Key, int Dis, bool HoldEnd);
    Result(const Result& R);
};

#ifndef KEYRHYTHM_CHARTWORK_H
#include "ChartWork.h"
#endif //KEYRHYTHM_CHARTWORK_H

#ifndef KEYRHYTHM_CHARTERROR_H
#include "ChartError.h"
#endif //KEYRHYTHM_CHARTERROR_H

#ifndef KEYRHYTHM_USER_H
#include "User.h"
#endif //KEYRHYTHM_USER_H

#endif //KEYRHYTHM_CHART_H
