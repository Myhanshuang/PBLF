    /***********************************************************
    * Base64 library                                           *
    * @author Ahmed Elzoughby                                  *
    * @date July 23, 2017                                      *
    * Purpose: encode and decode base64 format                 *
    ***********************************************************/

#ifndef BASE64_H
#define BASE64_H

#include <stdlib.h>
#include <string.h>
#include <memory.h>


/***********************************************
Encodes ASCII string into base64 format string
@param plain ASCII string to be encoded
@return encoded base64 format string
***********************************************/
char* base64_encode(char* plain);


/***********************************************
decodes base64 format string into ASCII string
@param plain encoded base64 format string
@return ASCII string to be encoded
***********************************************/
char* base64_decode(char* cipher);


#endif //BASE64_H
