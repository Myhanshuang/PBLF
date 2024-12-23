//
// Created by Logib on 2024/12/17.
//

#ifndef KEYRHYTHM_USER_H
#define KEYRHYTHM_USER_H

#ifndef _DIRENT_H_
#include <dirent.h>
#endif

#ifndef _INC_TYPES
#include <sys/types.h>
#endif

#ifndef _INC_STAT
#include <sys/stat.h>
#endif

#ifndef _GLIBCXX_CTIME
#include <ctime>
#endif

/**
 * @class User
 * @brief to save user data
 *
 * @details name[13], password[21]
 */
class User{
public :
    char userName[13]{};
    char Password[21]{};

    User();

    void save(FILE *Saving);
    bool newUser(FILE *Saving);
    bool correct(FILE *Saving);
};

class UserSaveData{
public :
    char userName[13]{};
    time_t timeSign = time(nullptr);
    wchar_t songTitle[258]{};
    Chart :: ChartAct playData;

    UserSaveData();
    UserSaveData(User U);
    UserSaveData(User U, Chart :: ChartAct &CA, wchar_t *ch);
    ~UserSaveData();

    void save(const char* historyPath);
    bool removeData(const char *historyPath, time_t timeStamp);
    bool getData(const char *historyPath, time_t timeStamp);
    bool getAnyData(const char *historyPath, int count);
    time_t getSongData(const char *historyPath, wchar_t *ch);

private:
    void getFromFile(FILE *his);
};

#endif //KEYRHYTHM_USER_H
