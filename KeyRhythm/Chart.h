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

#ifndef FloatMinute
#define FloatMinute 60000.0f
///< @def FloatMinute
#endif

#ifndef InfOffset
#define InfOffset 0x7ffffff
///< @def FloatMinute
#endif

#ifndef BeatToTime
/**
 * @brief trans beat to timestamp
 *
 * @details according to [a, b, c] in .mz file
 * @param eb everybeat
 * @param a which bar
 * @param b which beat/measure
 * @param c the division
 *
 * @return timestamp (int)
 */
#define BeatToTime(eb, a, b, c) (int )std::round(eb * (long double )a + eb / (long double )c * (long double )b)
#endif

extern int MaxOffset[8];

class Chart;


/**
 * @class Chart
 * @brief The chart data
 *
 * @details It included a head of \b metadata and several \b bar s
 ***/
class Chart{
public :
    long double BeatsPerMinute = 0.0l, EveryBeat = 0.0l;
    int NoteCount = 0, Offset = 0;
    short Column = 0;
    class Measure;
    Measure *ChartHead = nullptr;

    explicit Chart(int Keys);
    Chart(const Chart& C);
    Chart();
    ~Chart();

    Chart& operator =(Chart *Right){
        //memcpy(this, Right, sizeof (*Right) );
        this ->BeatsPerMinute = Right ->BeatsPerMinute;
        this ->EveryBeat = Right ->EveryBeat;
        this ->NoteCount = Right ->NoteCount;
        this ->Column = Right ->Column;
        this ->Offset = Right ->Offset;
        this ->ChartHead = Right ->ChartHead;
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

    explicit Measure(int Keys);
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

//class

#ifndef KEYRHYTHM_CHARTWORK_H
#include "ChartWork.h"
#endif //KEYRHYTHM_CHARTWORK_H

#ifndef KEYRHYTHM_CHARTERROR_H
#include "ChartError.h"
#endif //KEYRHYTHM_CHARTERROR_H

#endif //KEYRHYTHM_CHART_H
