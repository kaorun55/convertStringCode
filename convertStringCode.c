/**
 * @file		convertStringCode.c
 * @brief		文字コードの変換
 *
 * @author		kaorun
 */
#include "convertStringCode.h"

#define SJIS "SJIS"
#define UTF8 "UTF-8"

/**
 * @brief		文字コードを変換する
 *
 * @param		to		出力バッファ
 * @param		to_len		出力バッファサイズ
 * @param		to_code	出力文字コード
 * @param		from		入力バッファ
 * @param		from_code	入力文字コード
 *
 * @return		int	正常終了：0、エラー：errnoの値
 */
int convertStringCode( char *to, size_t to_len, const char *to_code,
				char *from, const char *from_code )
{
	char	*in = from;
	char	*out = to;
	size_t in_size = (size_t)strlen(from);
	size_t out_size = (size_t)to_len;
	size_t result = 0;

	iconv_t ic = iconv_open( to_code, from_code );
	if ( ic == (iconv_t)-1 ) {
		// エラーの場合はerrnoを返す
		result = errno;
		goto clenaup;
	}

	// ・out_size の出力は、呼び出し時の out_size から
	// 　処理したバイト数を引いたものが返るっぽい
	// ・戻り値は0が成功、-1が失敗
	result = iconv( ic, &in, &in_size, &out, &out_size );
	if ( result == (size_t)-1 ) {
		// エラーの場合はerrnoを返す
		result = errno;
		goto clenaup;
	}

clenaup:
	
	if ( ic != (iconv_t)-1 ) {
		iconv_close( ic );
	}

	return result;
}

/**
 * @brief		SJISをUTF8に変換する
 *
 * @param		to		出力バッファ
 * @param		to_len		出力バッファサイズ
 * @param		from		入力バッファ
 *
 * @return		int	正常終了：0、エラー：errnoの値
 */
int sjis2utf8( char *to, size_t to_len, char *from )
{
	return convertStringCode( to, to_len, UTF8, from, SJIS );
}

/**
 * @brief		UTF8をSJISに変換する
 *
 * @param		to		出力バッファ
 * @param		to_len		出力バッファサイズ
 * @param		from		入力バッファ
 *
 * @return		int	正常終了：0、エラー：errnoの値
 */
int utf82sjis( char *to, size_t to_len, char *from )
{
	return convertStringCode( to, to_len, SJIS, from, UTF8 );
}
// EOF

