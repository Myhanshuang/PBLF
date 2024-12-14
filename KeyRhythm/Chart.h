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
#define BeatToTime(eb, a, b, c) (int )(eb * (float )(a - 1) + eb / (float )c * (float )b + 0.5f)
#endif

/**
 * @class Chart
 * @brief The chart data
 *
 * @details It included a head of \b metadata and several \b bar s
 ***/
class Chart{
public :
    float BeatsPerMinute = 0.0f, EveryBeat = 0.0f;
    int NoteCount = 0, Column = 0, Offset = 0;
    class Measure;
    Measure *ChartHead = nullptr;

    explicit Chart(int Keys);
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
        this ->NxtMea = Right ->NxtMea;
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

class


#ifndef KEYRHYTHM_CHARTWORK_H
#include "ChartWork.h"
#endif //KEYRHYTHM_CHARTWORK_H

#ifndef KEYRHYTHM_CHARTERROR_H
#include "ChartError.h"
#endif //KEYRHYTHM_CHARTERROR_H

#endif //KEYRHYTHM_CHART_H
