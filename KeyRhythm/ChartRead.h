//
// Created by Logib on 2024/12/11.
//

/**
 * @author \b Logib
 *
 ***/

#ifndef KEYRHYTHM_CHARTREAD_H
#define KEYRHYTHM_CHARTREAD_H

/**
 * @brief get value in file (int)
 * @param File
 * @param val
 */
void getValueInt(FILE *File, int& val);

/**
 * @brief get value in file (short)
 * @param File
 * @param val
 */
void getValueShort(FILE *File, short& val);

/**
 * @brief get value in file (short)
 * @param File
 * @param val
 */
void getValueLDouble(FILE *File, long double& val);

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

#endif //KEYRHYTHM_CHARTREAD_H
