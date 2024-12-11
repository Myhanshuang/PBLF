//
// Created by Logib on 2024/12/3.
//

/**
 * @author \b Logib \b
 *
 ***/

#ifndef KEYRHYTHM_CHARTANALYZER_H
#define KEYRHYTHM_CHARTANALYZER_H

/**
 * @brief to include some basic libs
 */
#ifndef _GLIBCXX_CSTDIO
#include <cstdio>
#endif //_GLIBCXX_CSTDIO

#ifndef _GLIBCXX_CSTRING
#include <cstring>
#endif //_GLIBCXX_CSTRING

#ifndef __EXCEPTION__
#include <exception>
#endif

#ifndef FloatMinute
#define FloatMinute 60000.0f
///< @def FloatMinute
#endif

/**
 * @brief trans \b beat to \b timestamp
 *
 * @details according to [a, b, c] in .mz file
 * @param eb everybeat
 * @param a which bar
 * @param b which beat/measure
 * @param c the division
 *
 * @return \c int timestamp
 ***/
#define BeatToTime(eb, a, b, c) (int )(eb * (float )(a - 1) + eb / (float )c * (float )b + 0.5f)


#ifndef CHARTERROREXCEPTION
/**
 * @defgroup ErrorException
 * @brief all of error exception
 ***/
#define CHARTERROREXCEPTION

#define Exception0 "No chart file"
///< @brief cannot find the \b chart \b file
///< @addtogroup ErrorException

#define Exception1 "Lose meta data"
///< @brief cannot find \b metadata in chart file
///< @addtogroup ErrorException

#define Exception2 "Lose column info or bar begin info"
///< @brief cannot find \b "mode_ext" in chart file
///< @addtogroup ErrorException

#define Exception3 "Lose BPM or unit beat of bar"
///< @brief cannot find \b beat \b info in chart file
///< @addtogroup ErrorException

#define Exception4 "Lose note in chart"
///< @brief cannot find \b note in chart file
///< @addtogroup ErrorException

#define Exception5 "Chart format error"
///< @brief cannot find \b "column" or \b beat of note in chart file
///< @addtogroup ErrorException



class ChartError : public std :: exception{
public :
    short *ErrorCode;
    explicit ChartError(short errorCode){
        ErrorCode = new short;
        *ErrorCode = errorCode;
    };
    ChartError(const ChartError& e){
        ErrorCode = new short;
        *ErrorCode = *(e.ErrorCode);
    }
    ~ChartError() noexcept override{
        delete ErrorCode;
    }
    [[nodiscard]] const char * what() const noexcept override{
        if (*ErrorCode == 0) return Exception0;
        if (*ErrorCode == 1) return Exception1;
        if (*ErrorCode == 2) return Exception2;
        if (*ErrorCode == 3) return Exception3;
        if (*ErrorCode == 4) return Exception4;
        if (*ErrorCode == 5) return Exception5;
        return nullptr;
    }
};

#endif

/**
 * @class Chart
 * @brief The chart data
 *
 * @details It included a head of \b metadata and several \b bar s
 *
 * @param BPM (\& Keys)
 ***/
class Chart{
public :
    float BeatsPerMinute = 0.0f, EveryBeat = 0.0f;
    int NoteCount = 0, Column = 0, Offset = 0;
    class Measure;
    Measure *ChartHead = nullptr;

    explicit Chart(const int Keys){
        this ->Column = Keys;
        this ->ChartHead = nullptr;
    }
    Chart(){
        this ->Column = 4;
        this ->ChartHead = nullptr;
    }
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
 * @param Keys
 ***/
class Chart :: Measure{
public :
    bool ifHold = false;
    int timeStamp = 0;
    int *timeTable = nullptr;
    char *Bar = nullptr;
    Measure *NxtMea = nullptr;

    Measure(){
        this ->timeStamp = 0;
        this ->timeTable = nullptr;
        this ->NxtMea = nullptr;
        this ->Bar=new char [4];
    }
    explicit Measure(const int Keys){
        this ->timeStamp = 0;
        this ->timeTable = nullptr;
        this ->NxtMea = nullptr;
        this ->Bar=new char [Keys];
    }
    ~Measure(){
        delete[] this ->Bar;
        delete[] this ->timeTable;
        this ->Bar = nullptr;
        this ->timeTable = nullptr;
    }

    void* operator new(size_t size, void* phead){
        return phead;
    }

};

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
 * @brief use KMP to get the position of the keyword
 * @param File
 * @param Length
 * @param KeyWord
 * @return \c bool ( 1 = found )
 ***/
bool getKeyWord(FILE *File, int Length, const char *KeyWord);

/**
 * @brief use multiple KMP to get the position of the keywords
 * @param File
 * @param Length1
 * @param Length2
 * @param KeyWord1
 * @param KeyWord2
 * @return \c short ( 1 = found 1, 2 = found 2, 0 = all not found)
 ***/
short getKeyWords(FILE *File, int Length1, int Length2, const char *KeyWord1, const char *KeyWord2);

#endif //KEYRHYTHM_CHARTANALYZER_H
