//
// Created by Logib on 2024/12/17.
//

#ifndef KEYRHYTHM_USER_H
#define KEYRHYTHM_USER_H


/**
 * @class User
 * @brief to save user data
 *
 * @details name[13], pswd[21]
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

#endif //KEYRHYTHM_USER_H
