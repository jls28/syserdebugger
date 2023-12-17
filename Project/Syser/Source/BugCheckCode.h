#ifndef _BUGCHECKCODE_H_
#define _BUGCHECKCODE_H_
typedef struct _BUGCHECKCODE
{
	int BucCheckCode;
	const char* Name;
	const char* Description;
}BUGCHECKCODE,*PBUGCHECKCODE;
BUGCHECKCODE gBugCheckCode[];

#endif //_BUGCHECKCODE_H_