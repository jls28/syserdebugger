#ifndef _COLOR_OPTION_H_
#define _COLOR_OPTION_H_

extern int ColorOptionRef;

struct COLOR_OPTION
{
	DWORD		CRCCode;
	COLORREF	clrBackground;
	COLORREF	clrText;
	COLORREF	clrSelectedFrame;
	COLORREF	clrLostFocusSelFrame;
	COLORREF	clrChgText;
	COLORREF	clrSpeicalText;
//////////////////////////////////////////////////////////////////////////
//Code View
	COLORREF	clrCVBK;
	COLORREF	clrCVSelectedFrame;
	COLORREF	clrCVLostFocusSelFrame;
	COLORREF	clrCVAddr;
	COLORREF	clrCVPrefix;
	COLORREF	clrCVOpCode;
	COLORREF	clrCVReg;
	COLORREF	clrCVSegReg;
	COLORREF	clrCVImmed;
	COLORREF	clrCVOptr;
	COLORREF	clrCVSymbol;
	COLORREF	clrCVComment;
	COLORREF	clrCVKeyword;
	COLORREF	clrCVStr;
	COLORREF	clrCVBPXStrip;
	COLORREF	clrCVEIPStrip;
	COLORREF	clrCVBPXEIPStrip;
	COLORREF	clrCVAPIName;
	COLORREF	clrCVAPIParamType;
	COLORREF	clrCVAPIParamName;
	COLORREF	clrCVJmpLine;
	COLORREF	clrCVActivedJmpLine;
	COLORREF	clrCVPrefixByte;
	COLORREF	clrCVOpcodeByte;
	COLORREF	clrCVRegRMByte;		//opcode �� RegRM �ֽڵ���ɫ
	COLORREF	clrCVSIBByte;			//opcode �� SIB �ֽڵ���ɫ
	COLORREF	clrCVOtherOpcodeByte;		//opcode �� �����ֽڵ���ɫ
	COLORREF	clrCV3DNowByte;				//AMD 3dNow! ָ������һ���ֽ�
//Code View
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//Source Debug
	COLORREF	clrSDBK;
	COLORREF	clrSDSelectedFrame;
	COLORREF	clrSDLostFocusSelFrame;
	COLORREF	clrSDKeyword;
	COLORREF	clrSDId;
	COLORREF	clrSDPrekeyWord;
	COLORREF	clrSDCmtBlock;
	COLORREF	clrSDCmtLine;
	COLORREF	clrSDString;
	COLORREF	clrSDNum;
	COLORREF	clrSDOperation;
	COLORREF	clrSDClassName;
	COLORREF	clrSDStructName;
	COLORREF	clrSDUnionName;
	COLORREF	clrSDTypedefName;
	COLORREF	clrSDFunctionName;
	COLORREF	clrSDUnknown;
//Source Debug
//////////////////////////////////////////////////////////////////////////
};

extern COLOR_OPTION ColorOption;
extern COLOR_OPTION OrgColorOption;




#endif
