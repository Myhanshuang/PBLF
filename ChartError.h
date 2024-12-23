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

#ifndef NO_ERROR
#include <error.h>
#endif

/**
 * @defgroup ErrorException
 * @brief all of error exception
 ***/
#define Exception0 "No chart file"
///< @brief cannot find the \b chart \b file
///< @addtogroup ErrorException

#define Exception1 "Chart data error: Lose meta data"
///< @brief cannot find \b metadata in chart file
///< @addtogroup ErrorException

#define Exception2 "Chart data error: Lose column info or bar begin info"
///< @brief cannot find \b "mode_ext" in chart file
///< @addtogroup ErrorException

#define Exception3 "Chart data error: Lose BPM or unit beat of bar"
///< @brief cannot find \b beat \b info in chart file
///< @addtogroup ErrorException

#define Exception4 "Chart data error: Lose note in chart"
///< @brief cannot find \b note in chart file
///< @addtogroup ErrorException

#define Exception5 "Chart data error: Chart format error"
///< @brief cannot find \b "column" or \b beat of note in chart file
///< @addtogroup ErrorException

#define Exception6 "Judgment setting error"
///< @brief invalid judgment time setting
///< @addtogroup ErrorException

#define Exception7 "User data error: Empty username or too long username"
///< @brief user name is a non-empty string with the length of 12-char
///< @addtogroup ErrorException

#define Exception8 "User data error: Empty password or too long password"
///< @brief password is a non-empty string with the length of 20-char
///< @addtogroup ErrorException

#define Exception9 "User data error: Searching a new user"
///< @brief a new user need be saved before correct
///< @addtogroup ErrorException

#define Exception10 "Saving data error: Invalid history path"
///< @brief history should be a name of dir, which is used to save history info
///< @addtogroup ErrorException

#define Exception11 "Character encoding error"
///< @brief wchar_t should read several bytes to encode a character
///< @addtogroup ErrorException

#define Exception12 "Unknown error for wchar_t"
///< @brief using wchar_t as the source of error
///< @addtogroup ErrorException

#define Exception13 "Empty chart folder"
///< @brief when searching, you need at least one chart
///< @addtogroup ErrorException

#define Exception14 "Wrong history timestamp"
///< @brief delete a history record need it be existed before
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
            case 9: return Exception9;
            case 10: return Exception10;
            case 11: return Exception11;
            case 12: return Exception12;
            case 13: return Exception13;
            case 14: return Exception14;

            default :
                return nullptr;
        }
    }
};

#endif //KEYRHYTHM_CHARTERROR_H
