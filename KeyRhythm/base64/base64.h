#ifndef _BASE64_H_
#define _BASE64_H_

#ifdef __cplusplus
extern "C"{
#endif

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis base64 encode
 *
 * @Param src data before encode
 * @Param length data length
 *
 * @Returns data after encoded, need free.
 */
/* ----------------------------------------------------------------------------*/
extern char *base64_encode(char *src, int length);

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis base64 decode
 *
 * @Param dest data encoded
 *
 * @Returns data decoded
 */
/* ----------------------------------------------------------------------------*/
extern char *base64_decode(char *src, int *destlen);

#ifdef __cplusplus
}
#endif

#endif // _BASE64_H_
