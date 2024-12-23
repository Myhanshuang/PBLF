//
// Created by Logib on 2024/12/11.
//

/**
 * @author \b Logib
 *
 ***/

#ifndef KEYRHYTHM_CHARTREAD_H
#define KEYRHYTHM_CHARTREAD_H

#ifndef _GLIBCXX_CSTDIO
#include <cstdio>
#endif //_GLIBCXX_CSTDIO

/**
 * @brief get value in file (time_t)
 * @param File
 * @param val
 */
void getValueTT(FILE *File, time_t & val);

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
 * @param KeyWord
 * @return \c bool ( 1 = found )
 ***/
bool getKeyWord(FILE *File, const char *KeyWord);

/**
 * @brief use KMP to get the position of the keyword, with wide char
 * @param File
 * @param KeyWord
 * @return \c bool ( 1 = found )
 ***/
bool getKeyWord_w(FILE *File, const wchar_t *KeyWord);

/**
 * @brief to get wide chars from file, which use nothing to sign start and end;
 * @param File
 * @param src
 */
void getWordsW(FILE *File, wchar_t *src);

/**
 * @brief use multiple KMP to get the position of the keywords
 * @param File
 * @param KeyWord1
 * @param KeyWord2
 * @return \c short ( 1 = found 1, 2 = found 2, 0 = all not found)
 ***/
short getKeyWords(FILE *File, const char *KeyWord1, const char *KeyWord2);

/**
 * @brief to get wide chars from file, which use \" to sign start and end;
 * @param File
 * @param src
 * @return \c bool (0 is finished, 1 is failed)
 */
bool getWords_w(FILE *File, wchar_t *src);

/**
 * @brief set search index
 * @brief if charts have changed, we also need to reset it, by this func
 * @param chartPath
 */
void searchInit(const char *chartPath);

/**
 * @brief search key words in index
 * @param keyWord
 * @param result
 * @param chartPath
 * @return \c bool (1 = found, 0 = not found)
 */
bool searchSong(const char *keyWord, const char *chartPath);

#endif //KEYRHYTHM_CHARTREAD_H
