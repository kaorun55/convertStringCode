/**
 * @file		convertStringCode.h
 * @brief		文字コードの変換
 *
 * @author		kaorun
 */
#ifndef CONVERT_STRING_CODE_H_INCLUDE
#define CONVERT_STRING_CODE_H_INCLUDE

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <iconv.h>
#include <string.h>

int convertStringCode( char *to, size_t to_len, const char *to_code,
				char *from, const char *from_code );
int sjis2utf8( char *to, size_t to_len, char *from );
int utf82sjis( char *to, size_t to_len, char *from );

#ifdef __cplusplus
extern "C" {
#endif

#endif // #ifndef CONVERT_STRING_CODE_H_INCLUDE
// EOF

