#include <cspec.h>
#include <cspec_output_verbose.h>
#include <string.h>

#include "convertStringCode.h"

#define MAX_BUF 1024

// sjis の"あいうえおかきくけこさしすせそたちつてと"
const char sjis[] = {
	0x82,0xA0,0x82,0xA2,0x82,0xA4,0x82,0xA6,0x82,0xA8,
	0x82,0xA9,0x82,0xAB,0x82,0xAD,0x82,0xAF,0x82,0xB1,
	0x82,0xB3,0x82,0xB5,0x82,0xB7,0x82,0xB9,0x82,0xBB,
	0x82,0xBD,0x82,0xBF,0x82,0xC2,0x82,0xC4,0x82,0xC6,
	0x00 };

// utf8 の"あいうえおかきくけこさしすせそたちつてと"
const char utf8[] = {
	0xE3,0x81,0x82,0xE3,0x81,0x84,0xE3,0x81,0x86,0xE3,
	0x81,0x88,0xE3,0x81,0x8A,0xE3,0x81,0x8B,0xE3,0x81,
	0x8D,0xE3,0x81,0x8F,0xE3,0x81,0x91,0xE3,0x81,0x93,
	0xE3,0x81,0x95,0xE3,0x81,0x97,0xE3,0x81,0x99,0xE3,
	0x81,0x9B,0xE3,0x81,0x9D,0xE3,0x81,0x9F,0xE3,0x81,
	0xA1,0xE3,0x81,0xA4,0xE3,0x81,0xA6,0xE3,0x81,0xA8,
	0x00 };

DESCRIBE( SJIStoUTF8, "convertStringCode()" )
{
	IT( "SJIS から UTF8 に変換" ) {
		char	inbuf[MAX_BUF+1] = { 0 };
		char	outbuf[MAX_BUF+1] = { 0 };
		strcpy( inbuf, sjis );

		int result = convertStringCode( outbuf, sizeof(outbuf) - 1, "UTF-8", inbuf, "SJIS" );
		SHOULD_EQUAL( result, 0 );

		result = memcmp( outbuf, utf8, sizeof(utf8) );
		SHOULD_EQUAL( result, 0 );
	} END_IT;
} END_DESCRIBE;

DESCRIBE( SJIStoUTF8_EILSEQ, "convertStringCode()" )
{
	IT( "SJIS から UTF8 に変換するときに、途中のデータが壊れていた場合は、EILSEQを返す" ) {
		char	inbuf[MAX_BUF+1] = { 0 };
		char	outbuf[MAX_BUF+1] = { 0 };
		strcpy( inbuf, sjis );

		// 壊す
		inbuf[10] = 0xFF;

		int result = convertStringCode( outbuf, sizeof(outbuf) - 1, "UTF-8", inbuf, "SJIS" );

		// 不正な文字コードエラー
		SHOULD_EQUAL( result, EILSEQ );
	} END_IT;
} END_DESCRIBE;

DESCRIBE( SJIStoUTF8_E2BIG, "convertStringCode()" )
{
	IT( "SJIS から UTF8 に変換するときに、出力バッファ不足の場合は、E2BIGを返す" ) {
		char	inbuf[MAX_BUF+1] = { 0 };
		char	outbuf[2] = { 0 };
		strcpy( inbuf, sjis );

		int result = convertStringCode( outbuf, sizeof(outbuf) - 1, "UTF-8", inbuf, "SJIS" );

		// 長さが足りないエラー
		SHOULD_EQUAL( result, E2BIG );
	} END_IT;
} END_DESCRIBE;



DESCRIBE( UTF8toSJIS, "convertStringCode()" )
{
	IT( "UTF8 から SJIS に変換" ) {
		char	inbuf[MAX_BUF+1] = { 0 };
		char	outbuf[MAX_BUF+1] = { 0 };
		strcpy( inbuf, utf8 );

		int result = convertStringCode( outbuf, sizeof(outbuf) - 1, "SJIS", inbuf, "UTF-8" );
		SHOULD_EQUAL( result, 0 );

		result = memcmp( outbuf, sjis, sizeof(sjis) );
		SHOULD_EQUAL( result, 0 );
	} END_IT;
} END_DESCRIBE;

DESCRIBE( UTF8toSJIS_EILSEQ, "convertStringCode()" )
{
	IT( "UTF8 から SJIS に変換するときに、途中のデータが壊れていた場合は、EILSEQを返す" ) {
		char	inbuf[MAX_BUF+1] = { 0 };
		char	outbuf[MAX_BUF+1] = { 0 };
		strcpy( inbuf, utf8 );

		// 壊す
		inbuf[10] = 0xFF;

		int result = convertStringCode( outbuf, sizeof(outbuf) - 1, "SJIS", inbuf, "UTF-8" );

		// 不正な文字コードエラー
		SHOULD_EQUAL( result, EILSEQ );
	} END_IT;
} END_DESCRIBE;

DESCRIBE( UTF8toSJIS_E2BIG, "convertStringCode()" )
{
	IT( "UTF8 から SJIS に変換するときに、出力バッファ不足の場合は、E2BIGを返す" ) {
		char	inbuf[MAX_BUF+1] = { 0 };
		char	outbuf[2] = { 0 };
		strcpy( inbuf, utf8 );

		int result = convertStringCode( outbuf, sizeof(outbuf) - 1, "SJIS", inbuf, "UTF-8" );

		// 長さが足りないエラー
		SHOULD_EQUAL( result, E2BIG );
	} END_IT;
} END_DESCRIBE;


DESCRIBE( NotSuitableStringCode, "convertStringCode()" )
{
	IT( "入力文字と文字コードが合わない場合は、EILSEQを返す" ) {
		char	inbuf[MAX_BUF+1] = { 0 };
		char	outbuf[MAX_BUF+1] = { 0 };
		strcpy( inbuf, utf8 );

		// utf8 の文字列を入力して、SJISを入力文字コードに指定
		int result = convertStringCode( outbuf, sizeof(outbuf) - 1, "SJIS", inbuf, "SJIS" );

		// 不正な文字コードエラー
		SHOULD_EQUAL( result, EILSEQ );
	} END_IT;
} END_DESCRIBE;

DESCRIBE( InvalidInputStringCode, "convertStringCode()" )
{
	IT( "入力文字と文字コードが合わない場合は、EINVALを返す" ) {
		char	inbuf[MAX_BUF+1] = { 0 };
		char	outbuf[MAX_BUF+1] = { 0 };
		strcpy( inbuf, utf8 );

		// utf8 を間違えてみる
		int result = convertStringCode( outbuf, sizeof(outbuf) - 1, "SJIS", inbuf, "hoge" );

		SHOULD_EQUAL( result, EINVAL );
	} END_IT;
} END_DESCRIBE;

DESCRIBE( InvalidOutputStringCode, "convertStringCode()" )
{
	IT( "出力文字コードが未サポートの場合は、EINVALを返す" ) {
		char	inbuf[MAX_BUF+1] = { 0 };
		char	outbuf[MAX_BUF+1] = { 0 };
		strcpy( inbuf, utf8 );

		// sjis を間違えてみる
		int result = convertStringCode( outbuf, sizeof(outbuf) - 1, "fuga", inbuf, "UTF-8" );

		SHOULD_EQUAL( result, EINVAL );
	} END_IT;
} END_DESCRIBE;


DESCRIBE( SJIStoUTF8_function, "sjis2utf8()" )
{
	IT( "SJIS から UTF8 に変換" ) {
		char	inbuf[MAX_BUF+1] = { 0 };
		char	outbuf[MAX_BUF+1] = { 0 };
		strcpy( inbuf, sjis );

		int result = sjis2utf8( outbuf, sizeof(outbuf) - 1, inbuf);
		SHOULD_EQUAL( result, 0 );

		result = memcmp( outbuf, utf8, sizeof(utf8) );
		SHOULD_EQUAL( result, 0 );
	} END_IT;
} END_DESCRIBE;

DESCRIBE( UTF8toSJIS_function, "sjis2utf8()" )
{
	IT( "UTF8 から SJIS に変換" ) {
		char	inbuf[MAX_BUF+1] = { 0 };
		char	outbuf[MAX_BUF+1] = { 0 };
		strcpy( inbuf, utf8 );

		int result = utf82sjis( outbuf, sizeof(outbuf) - 1, inbuf);
		SHOULD_EQUAL( result, 0 );

		result = memcmp( outbuf, sjis, sizeof(sjis) );
		SHOULD_EQUAL( result, 0 );
	} END_IT;
} END_DESCRIBE;

int main( int argc, char **argv )
{
	CSpec_Run( DESCRIPTION(SJIStoUTF8), CSpec_NewOutputVerbose() );
	CSpec_Run( DESCRIPTION(SJIStoUTF8_EILSEQ), CSpec_NewOutputVerbose() );
	CSpec_Run( DESCRIPTION(SJIStoUTF8_E2BIG), CSpec_NewOutputVerbose() );

	CSpec_Run( DESCRIPTION(UTF8toSJIS), CSpec_NewOutputVerbose() );
	CSpec_Run( DESCRIPTION(UTF8toSJIS_EILSEQ), CSpec_NewOutputVerbose() );
	CSpec_Run( DESCRIPTION(UTF8toSJIS_E2BIG), CSpec_NewOutputVerbose() );

	CSpec_Run( DESCRIPTION(NotSuitableStringCode), CSpec_NewOutputVerbose() );
	CSpec_Run( DESCRIPTION(InvalidInputStringCode), CSpec_NewOutputVerbose() );
	CSpec_Run( DESCRIPTION(InvalidOutputStringCode), CSpec_NewOutputVerbose() );

	CSpec_Run( DESCRIPTION(SJIStoUTF8_function), CSpec_NewOutputVerbose() );
	CSpec_Run( DESCRIPTION(UTF8toSJIS_function), CSpec_NewOutputVerbose() );

	return 0;
}

