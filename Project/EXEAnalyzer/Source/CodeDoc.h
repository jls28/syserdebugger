#ifndef _CODE_DOC_H_
#define _CODE_DOC_H_

#include "DbgModule.h"

enum CV_COLOR//��ɫ����
{
	COLOR_NULL	= 0,
	COLOR_BK,				//����
	COLOR_ADDRESS,			//��ַ
	COLOR_PREFIX,			//ǰ׺
	COLOR_OPCODE,			//������
	COLOR_REGISTER,			//�Ĵ���
	COLOR_SEG_REG,			//�μĴ���
	COLOR_IMMED,			//������
	COLOR_OPTR,				//������
	COLOR_SYMBOL,			//��������
	COLOR_COMMENT,			//ע��
	COLOR_KEYWORD,			//�ؼ���
	COLOR_STR,				//�ַ���
	COLOR_BPX_STRIP,		//�ϵ�
	COLOR_EIP_STRIP,		//EIP
	COLOR_BPXEIP_STRIP,		//�ϵ��
	COLOR_API_NAME,			//API����
	COLOR_API_PARAM_TYPE,	//API��������
	COLOR_API_PARAM_NAME,	//API��������
	COLOR_JMP_LINE,			//Jmp��
	COLOR_ACTIVED_JMP_LINE,	//��Ҫ��ת��Jmp��
	COLOR_PREFIX_BYTE,		//opcode �е� prefix �ֽڵ���ɫ
	COLOR_OPCODE_BYTE,		//�� opcode �ֽڵ���ɫ
	COLOR_REGRM_BYTE,		//opcode �� RegRM �ֽڵ���ɫ
	COLOR_SIB_BYPE,			//opcode �� SIB �ֽڵ���ɫ
	COLOR_OTHER_OPCODE_BYTE,		//opcode �� �����ֽڵ���ɫ
	COLOR_3DNOW_BYTE,		//���ָ���� 3dNow ָ�������һ���ֽڵ���ɫ //immediate byte
	MAX_COLOR
};

#define CV_MARK_NULL			0x00000000
#define CV_MARK_EIP				0x00000001
#define CV_MARK_BPX				0x00000002
#define CV_MARK_DISABLED_BPX	0x00000004
#define CV_MARK_BOOK_MARK		0x00000008


typedef TExpCalc<CHAR,ULONG> CExpCalc;

class CCodeDoc
{
	bool	m_bIsOpened;
public:
	CCodeDoc();
	virtual	~CCodeDoc();
public:
	ULPOS		m_DocRangeStart;
	ULPOS		m_DocRangeEnd;
	virtual	bool	Open(PCSTR Name);
	virtual	void	Close();
	virtual ULSIZE	ReadImage(ULPOS Address,void*Buffer,ULSIZE Size) = 0;
	virtual	ULPOS	GetNextAddress(ULPOS Address,int Count) = 0;
	virtual	bool	GetSymbol(ULPOS Address,WCHAR*szSymbol,int BufSize);
	virtual	bool	GetComment(ULPOS Address,WCHAR*szComment,int BufSize);
	virtual	bool	SetComment(ULPOS Address,WCHAR*szComment);
	virtual	bool	GetLabel(ULPOS Address,WCHAR*szLabel,int BufSize);
	virtual	UINT	GetLineMark(ULPOS Address);
public:
	bool	inline	IsOpened(){	return m_bIsOpened;	}
};


#endif
