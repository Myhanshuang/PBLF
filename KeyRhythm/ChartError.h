//
// Created by Logib on 2024/12/11.
//

/**
 * @author \b Logib
 *
 ***/

#ifndef KEYRHYTHM_CHARTERROR_H
#define KEYRHYTHM_CHARTERROR_H

#ifndef __EXCEPTION__
#include <exception>
#endif

/**
 * @defgroup ErrorException
 * @brief all of error exception
 ***/
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

#define Exception6 "Judgment setting error"

#define Exception7 "User data error: Empty username"

#define Exception8 "User data error: Too long username"

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
        switch (*ErrorCode) {
            case 0: return Exception0;
            case 1: return Exception1;
            case 2: return Exception2;
            case 3: return Exception3;
            case 4: return Exception4;
            case 5: return Exception5;
            case 6: return Exception6;
            case 7: return Exception7;
            case 8: return Exception8;

            default :
                return nullptr;
        }
    }
};

#endif //KEYRHYTHM_CHARTERROR_H
