#ifndef _INTEL_DISASSEMBLER_H_
#define _INTEL_DISASSEMBLER_H_

#include "IntelDef.h"
#include "../../Code/Source/Code.h"
#define EFLAGS_OF_MASK 0x800
#define EFLAGS_SF_MASK 0x80
#define EFLAGS_ZF_MASK 0x40
#define EFLAGS_AF_MASK 0x10
#define EFLAGS_PF_MASK 0x4
#define EFLAGS_CF_MASK 0x1
#define EFLAGS_OF_SHIFT 11
#define EFLAGS_SF_SHIFT 7
#define EFLAGS_ZF_SHIFT 6
#define EFLAGS_AF_SHIFT 4
#define EFLAGS_PF_SHIFT 2
#define EFLAGS_CF_SHIFT 0

#if 0
union DIS_GEN_REG_T
{
	DWORD erx;
	struct
	{
		WORD word_filler;
		union
		{
			WORD rx;
			struct
			{
				BYTE rh;
				BYTE rl;
			} byte;
		} u_word;
	} word;
};
#else

union DIS_GEN_REG_T
{
	QWORD erx;
	struct
	{
		union
		{
			WORD rx;
			struct
			{
				BYTE rl;
				BYTE rh;
			} byte;
	    } u_word;

		WORD word_filler;
	} word;
};

#endif

struct DIS_CPU
{
	DIS_GEN_REG_T gen_reg[0x16];
	DWORD SEG_BASE[0x06];
	DWORD Eflags;
	char* gen_regsymbolname[0x8];
};

typedef struct _DIS_MODR_M
{
	unsigned int r_m:3;
	unsigned int reg:3; // �Ĵ������ / ��չ������
	unsigned int mod:2;
	unsigned int base:3;
	unsigned int index:3;
	unsigned int scale:2;
	unsigned int disp0:8;
	unsigned int disp1:8;
	unsigned int disp2:8;
	unsigned int disp3:8;

} DIS_MODR_M,*PDIS_MODR_M;

typedef struct _DIS_OPCODE_T
{
	int Attr;			//Ѱַ��ʽ�ֶ�
	int OpcodeType;		//����������
	int Opdata1;		//����������
	int Opdata2;
	int Opdata3;
	const char *name;			//ָ�����(ָ������)	"PUSHF","IRET",	"MOVSW",...
	//char *name32;		//ָ�����(ָ������)	"PUSHFD","IRETD","MOVSD",...
	int opbyte;
	int opbytelen;
	int rwflags;
} DIS_OPCODE_T,*PDIS_OPCODE_T;

//�ڴ�Ѱַ����������ṹ
typedef struct _DIS_ADDRESS
{
	unsigned int address_size;	// ָ���ַ�ߴ� BIT16/BIT32 ��: [bx+si]/[ebx+edi]
	//--------------
	unsigned int r_m;
	unsigned int reg;			// �Ĵ������ / ��չ������
	unsigned int mod;
	unsigned int base;			//��ַ�Ĵ����ĺ� �� [EBX + ESI*2 +99] �е� base = 3 
	unsigned int index;			//�����Ĵ����ĺ� �� [EBX + ESI*8 +69] �е� index = 6
	unsigned int scale;			//������������   �� [EBX + ESI*4 +96] �е� scale = 2
	unsigned int segment;		//��ModR/M�ֶξ�����  �μĴ�����index �� [EBX+ESI*4+96] ��Ϊ I_DS
	unsigned int displacement_size;	//��ModR/M�ֶξ����� ƫ���ֽ�����1/2/4
	//--------------
	QWORD displacement;		//��: mov eax,[ebx+esi*4+8f] �� displacement = 8f
	QWORD effect_address;	//��Ч��ַ
	QWORD line_address;
	//--------------

} DIS_ADDRESS,*PDIS_ADDRESS;

//intel ָ��Ĳ��������͡�
#define mode_invalid	0
#define mode_address	1
#define mode_register	2
#define mode_segment	3
#define mode_immed		4
#define mode_near		5
#define mode_far		6
//intel assembler language ָ��Ĳ��������͡�
#define mode_datadup	20
#define mode_symbol		21
#define mode_align		22


#define	RWFLG_UNKNOWN		0
#define RWFLG_READ			1
#define RWFLG_WRITE			2
#define	RWFLG_ACCESS		3

#define MAX_SYMBOL_LEN		0x40

//intel ����������ṹ
typedef struct _OPERAND_ITEM
{
	int mode;					//0:Invalid 1:Address 2:Register 3:Segment 
								//4:Immed 5:Near Pointer 6:Far Pointer 7:STRING
	int rwflag;					//0:Unknown	1:Read 2:Write 3:Access
	int opersize;				//1:byte, 2:word, 4:dword, 8:double dword

	char string[0x20];			// "[eax+ebx]","ecx","ES","0x0123" 	

	union
	{
		DIS_ADDRESS addr;	//for Mode 1:Address

		struct				//example mov fs,cx
		{
			int  reg_index; //reg_index = I_ECX (0x01)

		} reg;				//for Mode 2:Register	

		struct				//example mov fs,ax
		{
			int seg_index;	// seg_index = I_FS (0x04)

		} seg;				//for Mode 3:Segment Register	

		struct
		{
			QWORD immed_value;

		} immed;			//for Mode 4:Immed

		struct				//example 0x100:jmp 120
		{
			__int64 offset;	// offset = 0x1e
			QWORD label;	// label = 0x120

		} nearptr;			//for Mode 5:Near Pointer

		struct				//example jmp 0x28:0x100
		{
			DWORD   offset;	 //offset = 0x100
			DWORD   segment; //segment = 0x28

		} farptr;			//for Mode 6:Far Pointer


		struct
		{
			int count;
			BYTE buffer[0x20];

		} datadup;			//for Mode 20:mode_datadup

		struct
		{
			char string[MAX_SYMBOL_LEN];
			BOOL offset;
			int	 mode;
			DWORD addr;
		} symbol;			//for Mode 20:mode_symble

		struct
		{
			DWORD nBytes;
		} align;			//for Mode 20:mode_align
	};

} OPERAND_ITEM,*POPERAND_ITEM;

typedef struct _DISP_INSTRUCTION_T
{
	// prefix stuff here...
	unsigned int attr;				// attribute from fetchdecode
	unsigned int b1;				// opcode1 byte
	unsigned int mandatory_used;	// ǿ��ǰ׺ 0x66 0xf2 0xf3 
	unsigned int rep_used;
	unsigned int lock;				// tang add
	unsigned int rex_prefix;		// for 64 bit 
	unsigned int modrm;				// mod-nnn-r/m byte
	unsigned int mod;
	unsigned int nnn;				// �Ĵ������ / ��չ������
    unsigned int rm;
	WORD displ16u;				// for 16-bit modrm forms
	DWORD displ32u;				// for 32-bit modrm forms
	unsigned int seg;
	unsigned int sib;				// scale-index-base (2nd modrm byte)
    unsigned int scale;
    unsigned int index;
    unsigned int base;
	QWORD   addr_displacement;	// address displacement
	QWORD   rm_addr;
	QWORD	Iq;
	DWORD   Id;
	WORD	Iw;
	BYTE    Ib;
	BYTE    Ib2;					// for ENTER_IwIb  EXTRQ Vdq,Ib,Ib2
	WORD	Iw2;					// for JMP_Ap
	unsigned int ilen;					// instruction length
	unsigned int modrm_length;			// ��ModR/M�ֶξ����� ָ���
	unsigned int displacement_size;		//��ModR/M�ֶξ����� ƫ���ֽ�����1/2/4
	unsigned int os_32, as_32;			// OperandSize/AddressSize is 32bit
	unsigned int flags_in, flags_out;	// flags needed, flags modified
	unsigned int destination,source,other; //tang add
	unsigned int opcode_len;
	unsigned int perfix_len;
	unsigned int have_regrm;
	unsigned int have_sib;
	unsigned int mode;
	unsigned int RIP_Relative;

} DISP_INSTRUCTION_T;


//intel ָ������ṹ
typedef struct  _INSTRUCTION_INFORMATION
{
	DWORD eip;		//����ָ������λ�� 
	int CodeMode;	//�����ģʽ---ʮ��λ����(BIT16)����ʮ��λ����(BIT32)
	int Length;		//ָ��� example: nop -- Length = 0x01

	int	OpCode;		//example: mov eax,ebx ָ���С�OpCode = C_MOV
					//C_MOV,C_ADD,C_ADC,C_SUB,C_SBB,C_CMP...
	int OpcodeLen;	//
	int OpType;		//Ŀǰ����
	int CodeType;	//Ŀǰ����

	//======================
	

	char Name[0x10];			//"ADD","ADC","SUB","MOV","INC","DEC"
	int	 Lock;					// for LOCK prefix value = 0xf0 �� -1
	char LockName[0x08];		// "LOCK"
	int	 Repeat;				// for REPZ/REPNZ prefix vlaue = 0xf2 /0xf3 / -1
	char RepeatName[0x08];		// "REP","REPZ","REPNZ","REPE","REPNE"

	//I_ES,I_CS,I_SS,I_DS,I_FS,I_GS
	int SegmentPrefix;				//�ο�Խǰ׺ �� mov es:[eax+ebx],ecx �е� segprefix = I_ES (0x00)
	int xxxxptr;				//��������ָʾ�� �� mov dword ptr [ebx],0x01234567 �е� xxxxptr = sizeof(DWORD)  (0x04)
								//(BYTE PTR) (WORD PTR) (DWORD PTR)
								//(FDWORD PTR) (NEAR) (FAR)

	//=======================
	OPERAND_ITEM op[3];			//intel ָ�� ������� ��������
	//=======================

	BYTE *CodeBuff;			// �����ƵĴ��� example: '0xB8 0x01 0x23 0x45 0x67'
							// CodeBuff ����Ϊ NULL
	//=======================

	char *pasm;				// ����ַ��� example: "mov eax,67543210"
							// pasm ����Ϊ NULL
	//char hexcode[0x20];		//ʮ�����ƵĴ��봮 example: "B801234567"

	int U_x1,U_x2,U_x3;
	//Uasm Output Format Contral,
	//example:
	//		MOV   EAX,WORD PTR ES:[EAX]
	//0		x1    x2                   x3
	//���Ʒ�������ڻ������еĴ����ʽ.
	//�û�������U_x1,U_x2; U_x3ֻ������д.
	int PrefixLen;			//ָ��ǰ׺�ĳ���
	int Have_RegRM;
	int Have_SIB;
	int Is3DNow;
	int Rex_Prefix;

} INSTRUCTION_INFORMATION,*PINSTRUCTION_INFORMATION;

#define SIZE_a		8
#define SIZE_b		1
#define SIZE_d		4
#define SIZE_dq		16
#define SIZE_pi		8
#define SIZE_ps		8
#define SIZE_q		8
#define SIZE_s		6
#define SIZE_ss		16
#define SIZE_sd		16
#define SIZE_t		10
#define SIZE_w		2
#define SIZE_pd		16


#define	OPER_UNKNOWN	0
#define OPER_READ		1
#define	OPER_WRITE		2
#define	OPER_ACCESS		3

typedef enum _OP_TYPE_ENUM
{
	OP_Invalid,
	OP_Address,
	OP_Register,
	OP_Segment,
	OP_Immed,
	OP_Near,
	OP_Far,
    OP_String,
	OP_ControlRegister,
	OP_DebugRegister,
	OP_MMXRegister,
	OP_XMMRegister,
	OP_ByteRegister,
	OP_WordRegister,
	OP_DwordRegister,
	OP_SegmentRegister,
	OP_FPURegister,
}OP_TYPE_ENUM,*POP_TYPE_ENUM;

// The list of the types of Opdata1, Opdata2, Opdata3

typedef struct _tagLOCKREPXX
{
	const char* Name;
	BYTE  Code;

} LOCKREPXX,PLOCKREPXX;

typedef struct _OPDATAWIDE 
{
   int Size;
   const char *Xptr;

}  OPDATAWIDE,POPDATAWIDE;

typedef struct _OPER_ITEM
{
	OP_TYPE_ENUM    mode;		//0:Invalid 1:Address 2:Regsiter 3:Segment 4:Immed
						//5:Near Pointer 6:Far Pointer 7:STRING
	BYTE    rwflag;		//0:Unknown	1:Read 2:Write 3:Access
	int    opersize;	//1:byte, 2:word, 4:dword, 8:double dword
	OP_TYPE_ENUM	reg_type;
	int		prefix_size;// dword ptr, word ptr,byte ptr,qword ptr ,tbyte ptr.if assembly instruction includes 
	union
	{
		struct
		{
			int seg_index;	//SegReg Index!!!
			int reg_size;	//2:WORD 4:DWORD
			int base_reg_index;
			int off_reg_index;
			int off_reg_scale;
			DWORD   off_value;
		}   addr;		//for Mode 1:Address

		struct
		{
			int reg_index;
		}   reg;		//for Mode 2:Register	
		struct
		{
			int sreg_index;
		}   sreg;		//for Mode 3:Segment Register	
		struct
		{
			QWORD   immed_value;
		}   immed;		//for Mode 4:Immed
		struct
		{
			DWORD   offset;
		}   nearptr;	//for Mode 5:Near Pointer
		struct
		{
			DWORD   segment;
			DWORD   offset;
		}   farptr;		//for Mode 6:Far Pointer
		
		char string[0x80];		
	
	};

} OPER_ITEM,*POPER_ITEM;




struct ASSEMBLY_ERROR
{	
	unsigned	int name:1;		//ָ�����Ƿ���û�ҵ�
	unsigned	int Type:1;		//ָ������û�ҵ�
	unsigned	int P1:1;		//����һ����
	unsigned	int P2:1;		//����������
	unsigned	int P3:1;		//����������
	unsigned	int P4:1;		//�����Ĵ���
	unsigned  	int PX:1;		//�������ì��
	unsigned	int Size:1;		//�����޿�ȴ�
};
 
typedef struct  _ASSEMBLY_INFOMATION
{
	int NameSize;

	BYTE Lock;	// for LOCK prefix
	BYTE Repeat;	// for REPZ/REPNZ prefix
	int OperandSize;
	int AddressSize;
	BYTE SegmentPrefix; // for ES: CS: SS: DS: FS: GS:

	int OpcodeType;
	char *Name;
	
	OPER_ITEM op[3];
	DWORD	eip; 
	DIS_MODR_M * pmodr_m;
	int InstLength;
	int PrefixLength;
	DIS_MODR_M RegRM_SIB;
	//=======================================================

	int CodeMode;				//���ģʽ---ʮ��λ����(BIT16)����ʮ��λ����(BIT32)
	int ByteCount;				 //�ڲ�����
	BYTE *CodeBuff;				//�ڲ�����

	ASSEMBLY_ERROR error;

} ASSEMBLY_INFOMATION,*PASSEMBLY_INFOMATION;


//========================================


//========================================

//����ຯ������
//Disassembly: ��������ֵΪ ָ���
//InBuff: ����buffer ����Ϊ 1 -- 16 �ֽ�
//EIP: ����ָ��ĵ�ǰλ�ã������ jmp call jz jnz ��ָ��ʱ���õ�
//Mode: �ò�����ֵΪ BIT16 ��BIT32 �ֱ���� 16λ����/32λ����
int Disassembly(INSTRUCTION_INFORMATION *pInstInfo,BYTE *InBuff,QWORD EIP,int Mode,DIS_CPU *dis_cpu=NULL);
int Disassembler(char *OutBuff,BYTE *InBuff,DWORD EIP,int Mode = BIT32,int U_x1=0,int U_x2=0x08);
int Assembler(const char *InBuff,BYTE *OutBuff,DWORD EIP,int Mode = BIT32);
int PrintPasm(INSTRUCTION_INFORMATION *pInstInfo,char *pasm,int u_x1,int u_x2);
int PrintPasmForIDA(INSTRUCTION_INFORMATION *pInstInfo,char *pasm,int u_x1,int u_x2);
//=================================

class CIntelDisassembler
{
public:

	int m_U_x1;
	int m_U_x2;
	int m_U_x3;
	int m_Mode;
public:
	CIntelDisassembler()
	{
		m_U_x1 = 6;
		m_U_x2 = 12;
		m_U_x3 = 20;
	}
	
	~CIntelDisassembler()
	{		
	}

public:

	void SetStyle(int U_x1,int U_x2,int U_x3)
	{
		m_U_x1 = U_x1;
		m_U_x2 = U_x2;
		m_U_x3 = U_x3;
	}

	void SetMode(int Mode)
	{
		m_Mode = Mode;
	}

	static int GetLength(BYTE *InBuff,int Mode);
	static const char * const* GetIntelConstName(int *pNumb=(int*)(0));
	int Disassembler(INSTRUCTION_INFORMATION *pInstInfo,DIS_CPU *dis_cpu=NULL);
	int Assembler(const char *InBuff,BYTE *OutCode,DWORD EIP,int Mode);
};
bool IsJmp(unsigned short pCode,DWORD Eflags);


extern const DIS_OPCODE_T Group0x80[8];
extern const DIS_OPCODE_T Group0x81[8];
extern const DIS_OPCODE_T Group0x82[8];
extern const DIS_OPCODE_T Group0x83[8];
extern const DIS_OPCODE_T Group0xc0[8];
extern DIS_OPCODE_T Group0xc1[8];
extern DIS_OPCODE_T Group0xc6[8];
extern DIS_OPCODE_T Group0xc7[8];
extern DIS_OPCODE_T Group0xd0[8];
extern DIS_OPCODE_T Group0xd1[8];
extern DIS_OPCODE_T Group0xd2[8];
extern DIS_OPCODE_T Group0xd3[8];
extern DIS_OPCODE_T Group0xf6[8];
extern DIS_OPCODE_T Group0xf7[8];
extern DIS_OPCODE_T Group0xfe[8];
extern DIS_OPCODE_T Group0xff[8];
extern DIS_OPCODE_T Group0x8f[8];
extern DIS_OPCODE_T Group0xd8[72];
extern DIS_OPCODE_T Group0xd9[72];
extern DIS_OPCODE_T Group0xda[72];
extern DIS_OPCODE_T Group0xdb[72];
extern DIS_OPCODE_T Group0xdc[72];
extern DIS_OPCODE_T Group0xdd[72];
extern DIS_OPCODE_T Group0xde[72];
extern DIS_OPCODE_T Group0xdf[72];
extern const DIS_OPCODE_T Group0x0f00[8];

extern DIS_OPCODE_T Group0x0f01[72];
extern DIS_OPCODE_T Group0x0f18[8];
extern DIS_OPCODE_T Group0x0f71[8];
extern DIS_OPCODE_T Group0x0f72[8];
extern DIS_OPCODE_T Group0x0f73[8];
extern DIS_OPCODE_T Group0x0fae[16];
extern const DIS_OPCODE_T Group0x0fba[8];
extern DIS_OPCODE_T Group0x0fc7[8];
extern DIS_OPCODE_T Group0x0f3800[4];
extern DIS_OPCODE_T Group0x0f3801[4];
extern DIS_OPCODE_T Group0x0f3802[4];
extern DIS_OPCODE_T Group0x0f3803[4];
extern DIS_OPCODE_T Group0x0f3804[4];
extern DIS_OPCODE_T Group0x0f3805[4];
extern DIS_OPCODE_T Group0x0f3806[4];
extern DIS_OPCODE_T Group0x0f3807[4];
extern DIS_OPCODE_T Group0x0f3808[4];
extern DIS_OPCODE_T Group0x0f3809[4];
extern DIS_OPCODE_T Group0x0f380a[4];
extern DIS_OPCODE_T Group0x0f380b[4];
extern DIS_OPCODE_T Group0x0f3810[4];
extern DIS_OPCODE_T Group0x0f3814[4];
extern DIS_OPCODE_T Group0x0f3815[4];
extern DIS_OPCODE_T Group0x0f3817[4];
extern DIS_OPCODE_T Group0x0f381c[4];
extern DIS_OPCODE_T Group0x0f381d[4];
extern DIS_OPCODE_T Group0x0f381e[4];
extern DIS_OPCODE_T Group0x0f3820[8];
extern DIS_OPCODE_T Group0x0f3821[8];
extern DIS_OPCODE_T Group0x0f3822[8];
extern DIS_OPCODE_T Group0x0f3823[8];
extern DIS_OPCODE_T Group0x0f3824[8];
extern DIS_OPCODE_T Group0x0f3825[8];
extern DIS_OPCODE_T Group0x0f3828[4];
extern DIS_OPCODE_T Group0x0f3829[4];
extern DIS_OPCODE_T Group0x0f382a[4];
extern DIS_OPCODE_T Group0x0f382b[4];
extern DIS_OPCODE_T Group0x0f3830[8];
extern DIS_OPCODE_T Group0x0f3831[8];
extern DIS_OPCODE_T Group0x0f3832[8];
extern DIS_OPCODE_T Group0x0f3833[8];
extern DIS_OPCODE_T Group0x0f3834[8];
extern DIS_OPCODE_T Group0x0f3835[8];
extern DIS_OPCODE_T Group0x0f3837[4];
extern DIS_OPCODE_T Group0x0f3838[4];
extern DIS_OPCODE_T Group0x0f3839[4];
extern DIS_OPCODE_T Group0x0f383a[4];
extern DIS_OPCODE_T Group0x0f383b[4];
extern DIS_OPCODE_T Group0x0f383c[4];
extern DIS_OPCODE_T Group0x0f383d[4];
extern DIS_OPCODE_T Group0x0f383e[4];
extern DIS_OPCODE_T Group0x0f383f[4];
extern DIS_OPCODE_T Group0x0f3840[4];
extern DIS_OPCODE_T Group0x0f3841[4];
extern DIS_OPCODE_T Group0x0f3880[8];
extern DIS_OPCODE_T Group0x0f3881[8];
extern DIS_OPCODE_T Group0x0f38f0[8];
extern DIS_OPCODE_T Group0x0f38f1[8];
extern DIS_OPCODE_T Group0x0f38[256];
extern DIS_OPCODE_T Group0x0f3a08[4];
extern DIS_OPCODE_T Group0x0f3a09[4];
extern DIS_OPCODE_T Group0x0f3a0a[4];
extern DIS_OPCODE_T Group0x0f3a0b[4];
extern DIS_OPCODE_T Group0x0f3a0c[4];
extern DIS_OPCODE_T Group0x0f3a0d[4];
extern DIS_OPCODE_T Group0x0f3a0e[4];
extern DIS_OPCODE_T Group0x0f3a0f[4];
extern DIS_OPCODE_T Group0x0f3a14[8];
extern DIS_OPCODE_T Group0x0f3a15[8];
extern DIS_OPCODE_T Group0x0f3a16[4];
extern DIS_OPCODE_T Group0x0f3a17[4];
extern DIS_OPCODE_T Group0x0f3a20[8];
extern DIS_OPCODE_T Group0x0f3a21[8];
extern DIS_OPCODE_T Group0x0f3a22[4];
extern DIS_OPCODE_T Group0x0f3a40[4];
extern DIS_OPCODE_T Group0x0f3a41[4];
extern DIS_OPCODE_T Group0x0f3a42[4];
extern DIS_OPCODE_T Group0x0f3a60[4];
extern DIS_OPCODE_T Group0x0f3a61[4];
extern DIS_OPCODE_T Group0x0f3a62[4];
extern DIS_OPCODE_T Group0x0f3a63[4];
extern DIS_OPCODE_T Group0x0f3a[256];
extern DIS_OPCODE_T Group0x0f70[4];
extern DIS_OPCODE_T Group0x0f74[4];
extern DIS_OPCODE_T Group0x0f75[4];
extern DIS_OPCODE_T Group0x0f76[4];
extern DIS_OPCODE_T Group0x0f7c[4];
extern DIS_OPCODE_T Group0x0f7d[4];
extern DIS_OPCODE_T Group0x0f7e[4];
extern DIS_OPCODE_T Group0x0f7f[4];
extern DIS_OPCODE_T Group0x0f60[4];
extern DIS_OPCODE_T Group0x0f61[4];
extern DIS_OPCODE_T Group0x0f62[4];
extern DIS_OPCODE_T Group0x0f63[4];
extern DIS_OPCODE_T Group0x0f64[4];
extern DIS_OPCODE_T Group0x0f65[4];
extern DIS_OPCODE_T Group0x0f66[4];
extern DIS_OPCODE_T Group0x0f67[4];
extern DIS_OPCODE_T Group0x0f68[4];
extern DIS_OPCODE_T Group0x0f69[4];
extern DIS_OPCODE_T Group0x0f6a[4];
extern DIS_OPCODE_T Group0x0f6b[4];
extern DIS_OPCODE_T Group0x0f6c[4];
extern DIS_OPCODE_T Group0x0f6d[4];
extern DIS_OPCODE_T Group0x0f6e[4];
extern DIS_OPCODE_T Group0x0f6f[4];
extern DIS_OPCODE_T Group0x0f50[8];
extern DIS_OPCODE_T Group0x0f51[4];
extern DIS_OPCODE_T Group0x0f52[4];
extern DIS_OPCODE_T Group0x0f53[4];
extern DIS_OPCODE_T Group0x0f54[4];
extern DIS_OPCODE_T Group0x0f55[4];
extern DIS_OPCODE_T Group0x0f56[4];
extern DIS_OPCODE_T Group0x0f57[4];
extern DIS_OPCODE_T Group0x0f58[4];
extern DIS_OPCODE_T Group0x0f59[4];
extern DIS_OPCODE_T Group0x0f5a[4];
extern DIS_OPCODE_T Group0x0f5b[4];
extern DIS_OPCODE_T Group0x0f5c[4];
extern DIS_OPCODE_T Group0x0f5d[4];
extern DIS_OPCODE_T Group0x0f5e[4];
extern DIS_OPCODE_T Group0x0f5f[4];
extern DIS_OPCODE_T Group0x0f10[4];
extern DIS_OPCODE_T Group0x0f11[4];
extern DIS_OPCODE_T Group0x0f12[8];
extern DIS_OPCODE_T Group0x0f13[4];
extern DIS_OPCODE_T Group0x0f14[4];
extern DIS_OPCODE_T Group0x0f15[4];
extern DIS_OPCODE_T Group0x0f16[8];
extern DIS_OPCODE_T Group0x0f17[4];
extern DIS_OPCODE_T Group0x0f28[4];
extern DIS_OPCODE_T Group0x0f29[4];
extern DIS_OPCODE_T Group0x0f2a[4];
extern DIS_OPCODE_T Group0x0f2b[4];
extern DIS_OPCODE_T Group0x0f2c[4];
extern DIS_OPCODE_T Group0x0f2d[4];
extern DIS_OPCODE_T Group0x0f2e[4];
extern DIS_OPCODE_T Group0x0f2f[4];
extern DIS_OPCODE_T Group0x0fd0[4];
extern DIS_OPCODE_T Group0x0fd1[4];
extern DIS_OPCODE_T Group0x0fd2[4];
extern DIS_OPCODE_T Group0x0fd3[4];
extern DIS_OPCODE_T Group0x0fd4[4];
extern DIS_OPCODE_T Group0x0fd5[4];
extern DIS_OPCODE_T Group0x0fd6[4];
extern DIS_OPCODE_T Group0x0fd7[4];
extern DIS_OPCODE_T Group0x0fd8[4];
extern DIS_OPCODE_T Group0x0fd9[4];
extern DIS_OPCODE_T Group0x0fda[4];
extern DIS_OPCODE_T Group0x0fdb[4];
extern DIS_OPCODE_T Group0x0fdc[4];
extern DIS_OPCODE_T Group0x0fdd[4];
extern DIS_OPCODE_T Group0x0fde[4];
extern DIS_OPCODE_T Group0x0fdf[4];
extern DIS_OPCODE_T Group0x0fe0[4];
extern DIS_OPCODE_T Group0x0fe1[4];
extern DIS_OPCODE_T Group0x0fe2[4];
extern DIS_OPCODE_T Group0x0fe3[4];
extern DIS_OPCODE_T Group0x0fe4[4];
extern DIS_OPCODE_T Group0x0fe5[4];
extern DIS_OPCODE_T Group0x0fe6[4];
extern DIS_OPCODE_T Group0x0fe7[4];
extern DIS_OPCODE_T Group0x0fe8[4];
extern DIS_OPCODE_T Group0x0fe9[4];
extern DIS_OPCODE_T Group0x0fea[4];
extern DIS_OPCODE_T Group0x0feb[4];
extern DIS_OPCODE_T Group0x0fec[4];
extern DIS_OPCODE_T Group0x0fed[4];
extern DIS_OPCODE_T Group0x0fee[4];
extern DIS_OPCODE_T Group0x0fef[4];
extern DIS_OPCODE_T Group0x0ff0[4];
extern DIS_OPCODE_T Group0x0ff1[4];
extern DIS_OPCODE_T Group0x0ff2[4];
extern DIS_OPCODE_T Group0x0ff3[4];
extern DIS_OPCODE_T Group0x0ff4[4];
extern DIS_OPCODE_T Group0x0ff5[4];
extern DIS_OPCODE_T Group0x0ff6[4];
extern DIS_OPCODE_T Group0x0ff7[4];
extern DIS_OPCODE_T Group0x0ff8[4];
extern DIS_OPCODE_T Group0x0ff9[4];
extern DIS_OPCODE_T Group0x0ffa[4];
extern DIS_OPCODE_T Group0x0ffb[4];
extern DIS_OPCODE_T Group0x0ffc[4];
extern DIS_OPCODE_T Group0x0ffd[4];
extern DIS_OPCODE_T Group0x0ffe[4];
extern DIS_OPCODE_T Group0x0fc4[8];
extern DIS_OPCODE_T Group0x0fc5[4];
extern DIS_OPCODE_T Group0x0fc6[4];
extern DIS_OPCODE_T Group0x0f[256];
//extern DIS_OPCODE_T Group_main[398];
extern DIS_OPCODE_T RexPrefixFor64Bit[1];
typedef TMap<CIStrA,DIS_OPCODE_T*> NAME_OPCODE_MAP;

#endif
