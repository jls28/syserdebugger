#ifndef _INPUT_DRIVER_H_
#define _INPUT_DRIVER_H_

#define KEYBUFFERMAXLENGTH					0x200
#define FUNCKEYPRESS						1  //���ܼ�������ʱ  dwFuncKeyState ������ֵ
#define FUNCKEYRELEASE						0 //���ܼ����ͷ�ʱ  dwFuncKeyState ������ֵ
#define MOUSE_DATA_MAX						0x100
#define MOUSE_BEGINDATA_MAX					24
#define MOUSE_INTERRUPT_LEFT_PRESS			0x1
#define MOUSE_INTERRUPT_RIGHT_PRESS			0x2
#define MOUSE_INTERRUPT_MIDDLE_PRESS		0x4
#define MAX_WINDOWSKEYBUFFER_LENGTH			0x10
#define DISABLE_IOAPIC_HARDWARE_INTERRUPT	1
#define DEBUGKEYBORADEXIT					0


#define KEYBOARD_CAPS_LOCK					4
#define KEYBOARD_NUM_LOCK					2
#define KEYBOARD_SCROLL_LOCK				1

DWORD GetHardwareInterruptVectorNum(DWORD dwNum);

#ifdef __cplusplus
extern "C" {
#endif

extern bool gbInterruptInstalled;
extern DWORD UHCI_Number;// UHCI USB HOST CONTROL NUMBER
extern DWORD UHCIFrameBaseRegister[10];
extern bool bWRITE_PORT_UCHAR_PatchOK;
extern bool bREAD_PORT_UCHAR_PatchOK;
extern BYTE byteOrgWRITE_PORT_UCHARInstruction[];
extern BYTE byteOrgREAD_PORT_UCHARInstruction[];
extern DWORD dwKeyLedFlags;
extern DWORD dwWaitMouseCounter;
extern DWORD dwPhysical_0_LinesAddress;
extern DWORD OwnerPageDirItemAddress;
extern DWORD OwnerPageTabItemAddress;
extern bool bEscKeyStat;
extern bool bKeyLedCmd;
extern bool bKeyScrollLockStat;
extern bool bKeyNumLockStat;
extern bool bKeyCapsLockStat;
extern bool bKeyScrollLockStatInitOK;
extern bool bKeyNumLockStatInitOK;
extern bool bKeyCapsLockStatInitOK;
extern bool gbChangeDrmState;
extern bool gbKeyActiveMethod;
extern __int64 TscPerSec;

extern bool gTouchPad;
extern BYTE gTouchPadMode;

extern bool bKeytoSoftice;
extern bool bInt1HereStat;
extern bool bInt3HereStat;

typedef struct stSofticeIdFind
{
	DWORD dwIntNum1;
	DWORD dwIntNum1JmpOffset;					//softice ������ʱ��	push OldWindowsHandleAddress
												//						jmp SofticeHandleAddress
												// ����	jmp ��ָ������� E98631f7ff ����� DWORD ���� fff73186
	DWORD dwIntNum2;
	DWORD dwIntNum2JmpOffset;					//ͬ��
	DWORD dwIntNum3;
	DWORD dwIntNum3JmpOffset;					//ͬ��	
	DWORD dwIntNum1BaseOffset;					//�� dword �� softice ���ж� 1 �Ĵ������ĵ�ַ����� softice ����ַ��ƫ��
	DWORD dwActiveFunctionBaseOffset;			//softice �ڲ��ļ��������� softice ����ַ��ƫ��
	DWORD dwActiveFlagsBaseOffset;				//softice �ڲ��ļ���� byte ��������� softice ����ַ��ƫ��
}SOFTICEFINDDATA;


typedef struct _st_desc
{
	WORD wBaseAddrLow;
	WORD wSegmentSec;
	WORD wAttribture;
	WORD wBaseAddrHigh;
}ST_DESC;


//
//	��Ҫ��common-x86.inc�еĶ���ͬ��
//

typedef struct _KeyInterruptVariable{
	bool IsActive;								//0  ����û�н������ģʽ //1  �����˲���ģʽ
	bool IsInsideActive;						//�������Ƽ����жϵ���ѭ������
	DWORD dwFuncKeyState;						//���ܼ���״̬��0�����ܼ�û�б����£�1�����ܼ�������
	DWORD dwOldKeyboardInterrupt;				//ԭʼ�ļ����жϵ���ڵ�ַ
	UINT nCurrentKeyBufferIndex;				//���̻������ĵ�ǰ����
	ST_DESC *timer;								//���̵���������ָ��
	DWORD dwOldTimerInterrupt;
	ST_DESC *keyboard;							//���̵���������ָ��
	ST_DESC *mouse;								//������������ָ��
	WORD wSystemDataSegment;					//Windows ����ϵͳ�� �㻷�����ݶε�ѡ����
	WORD wDSRegister;							// DS ��������ԭʼֵ
	BYTE chFuncKeyScanCode;						//�ȼ���ɨ����    Ctrl ����ɨ����
	BYTE chSecondKeyScanCode;					//�ȼ���ɨ����    a ����ɨ����								
	BYTE IsSaveInterruptInformation;			//�˿� 21 �� A1 ��ֵ�Ƿ񱣴��ˣ������������Ϊ 1��
	BYTE Interrupt8259Shield_1;					//�˿� 21 ��ֵ
	BYTE Interrupt8259Shield_2;					//�˿� A1 ��ֵ
	BYTE chAltKeyState;							//ALT ����״̬��
	BYTE chAltKeyScanCode;						// ALT ����ɨ����
	BYTE chCurrentScanCode;						//��ǰ�İ����� ɨ����
	DWORD dwIOAPICLineAddress;
	DWORD dwIOAPICPhysAddress;
	DWORD dwLocalAPICPhysAddress;
	DWORD dwLocalAPICLineAddress;
	bool bIsUseAPIC;
	DWORD dwOldMouseInterrupt;	  
	bool bMouseResponseState;
	DWORD dwMouseDataIndex;
	BYTE chCurrentMouseData;
	DWORD dwLocalAPICTaskPriority;
	BYTE chPrveScanCode;
	BYTE chKeyBuffer[KEYBUFFERMAXLENGTH];			//���̻�����

} KeyInterruptVariable;
extern bool gHasPS2Keyboard;
extern bool gHasPS2Mouse;
#ifdef CODE_OS_NT_DRV
extern SYSTEM_QUERY_TIME_ADJUSTMENT SystemTimeAdjustment;
#endif
extern DWORD dwInterruptF8Address;
extern DWORD dwInterruptF9Address;
extern DWORD dwInterruptFaAddress;
extern DWORD dwInterruptFbAddress;
extern DWORD dwInterruptFcAddress;
extern BYTE chWindowsKeyArray[];   //�û�����������ģ��  READ_PORT_UCHAR �����õģ�
								//�û������е�����Ҫ��i8042prt.sys �������˿�60ʱ��ģ����� 60�˿�
extern DWORD dwCurrentReadIndex;
extern DWORD dwCurrentWriteIndex;
extern DWORD dwCurrentWindowsKeyLen;
extern bool bIsMouseData;


extern DWORD dwWaitMouseInterrupt;
extern DWORD dwEflag;
extern KeyInterruptVariable stKeyGlobalVariable;
extern INT nTempVar;
extern BYTE *SoftIceActive;
extern BYTE NormalTable[2][0x80];
extern BYTE ExternTable[];
/*
extern BYTE ShiftKeyTable[];
extern BYTE AltKeyTable[];
extern BYTE CtrlKeyTable[];
extern BYTE CtrlAltKeyTable[];
*/
extern DWORD dwSofticeDriverBase;
extern DWORD dwRPUC;
extern bool bActiveSoftice;
extern DWORD dwRPUCLookup;
extern DWORD dwWPUCLookup;
extern DWORD dwWPUC;
extern DWORD dwTmpVar;
extern ST_DESC *dwTmpIdt;
extern DWORD gHardInt1Vector;
extern DWORD dwOldInterruptFA;
extern DWORD dwOldInterruptF8;
extern DWORD dwOldInterruptF9;
extern void (*pSofticeActivePointer)(void);

//extern stSerialMouse aSerialMouseData[];
UCHAR NEW_READ_PORT_UCHAR(IN DWORD Port);

//VOID SaveDSRegister(VOID);
//VOID StoreDSRegister(VOID);
void KeyboardInterruptService();
VOID SyserWaitKeyboardInputBufferEmpte(VOID);
VOID SyserWaitKeyboardOutPutBufferFull(VOID);
void SyserSimulateKeyboardInput(BYTE scancode,bool isDisableInterrupt = FALSE);
VOID	SyserWaitMouseStatus(VOID);


bool InstallAllInterrupt();
bool UninstallAllInterrupt();

//bool GetIOAPICBase();
void Delay();

//void MouseInterruptService();
VOID SyserWaitMouseInputBufferEmpte(VOID);
VOID SyserWaitMouseOutPutBufferFull(VOID);
bool IsMouseOutPutBufferFull();
void SyserCleanMouseOutPutBuffer();
void WriteMouseCommandPrefix(BYTE chCmd);
void WriteMouseCommandByte(BYTE chCmd);
bool SyserIsMouseOutPutBufferFull();
// void SaveEflag();
// void StoreEflag();
BYTE SyserReadMouseByte(VOID);
DWORD CalcPackageBeginOffset();
void InsertKeyboardBuffer(BYTE chScanCode);




DWORD CheckSofticeExist();

bool SendCommandTo60Port(BYTE byteCmd);//��˿� 60 ��������
void SetKeyboardPortState(bool bState);
bool SendCommandTo64Port(BYTE byteCmd);//��˿� 64 ��������
void SetKeyboardLEDState(DWORD bScrollLock,DWORD bNumLock,DWORD bCapsLock);
VOID SyserGetPortState(VOID);//�ڸ����̷����������Ժ�ú���������ȡ���̵ķ���ֵ
VOID SyserSimulateWindowsKeyboardInput(BYTE byteScanCode);
bool GetCurrentWindowLEDState(DWORD dwKeyIndex);
void NEW_WRITE_PORT_UCHAR(IN PUCHAR  Port, IN UCHAR  Value);
void PatchSystemWRITE_PORT_UCHAR(bool bIsPatch);
void PatchSystemREAD_PORT_UCHAR(bool bIsPatch);
//void SerialMouseInterruptService();
//bool CheckSerialMouse();
VOID SyserSyncMouseInterrupt(VOID);
extern bool bRightCtrlKey;


//void Interrupt_0xF9_Mouse_Service();
VOID SyserInterrupt_0xF8_Keyboard_Service(VOID);

#ifdef __cplusplus
}; // extern "C"
#endif


typedef struct _IMAGE_DATA_DIRECTORY {
	DWORD   VirtualAddress;
	DWORD   Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;
#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16
typedef struct _IMAGE_DOS_HEADER {      // DOS .EXE header
	WORD   e_magic;                     // Magic number
	WORD   e_cblp;                      // Bytes on last page of file
	WORD   e_cp;                        // Pages in file
	WORD   e_crlc;                      // Relocations
	WORD   e_cparhdr;                   // Size of header in paragraphs
	WORD   e_minalloc;                  // Minimum extra paragraphs needed
	WORD   e_maxalloc;                  // Maximum extra paragraphs needed
	WORD   e_ss;                        // Initial (relative) SS value
	WORD   e_sp;                        // Initial SP value
	WORD   e_csum;                      // Checksum
	WORD   e_ip;                        // Initial IP value
	WORD   e_cs;                        // Initial (relative) CS value
	WORD   e_lfarlc;                    // File address of relocation table
	WORD   e_ovno;                      // Overlay number
	WORD   e_res[4];                    // Reserved words
	WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
	WORD   e_oeminfo;                   // OEM information; e_oemid specific
	WORD   e_res2[10];                  // Reserved words
	LONG   e_lfanew;                    // File address of new exe header
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;
typedef struct _IMAGE_FILE_HEADER {
	WORD    Machine;
	WORD    NumberOfSections;
	DWORD   TimeDateStamp;
	DWORD   PointerToSymbolTable;
	DWORD   NumberOfSymbols;
	WORD    SizeOfOptionalHeader;
	WORD    Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;
typedef struct _IMAGE_OPTIONAL_HEADER {
	//
	// Standard fields.
	//

	WORD    Magic;
	BYTE    MajorLinkerVersion;
	BYTE    MinorLinkerVersion;
	DWORD   SizeOfCode;
	DWORD   SizeOfInitializedData;
	DWORD   SizeOfUninitializedData;
	DWORD   AddressOfEntryPoint;
	DWORD   BaseOfCode;
	DWORD   BaseOfData;

	//
	// NT additional fields.
	//

	DWORD   ImageBase;
	DWORD   SectionAlignment;
	DWORD   FileAlignment;
	WORD    MajorOperatingSystemVersion;
	WORD    MinorOperatingSystemVersion;
	WORD    MajorImageVersion;
	WORD    MinorImageVersion;
	WORD    MajorSubsystemVersion;
	WORD    MinorSubsystemVersion;
	DWORD   Win32VersionValue;
	DWORD   SizeOfImage;
	DWORD   SizeOfHeaders;
	DWORD   CheckSum;
	WORD    Subsystem;
	WORD    DllCharacteristics;
	DWORD   SizeOfStackReserve;
	DWORD   SizeOfStackCommit;
	DWORD   SizeOfHeapReserve;
	DWORD   SizeOfHeapCommit;
	DWORD   LoaderFlags;
	DWORD   NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;
typedef struct _IMAGE_OPTIONAL_HEADER64 {
	WORD        Magic;
	BYTE        MajorLinkerVersion;
	BYTE        MinorLinkerVersion;
	DWORD       SizeOfCode;
	DWORD       SizeOfInitializedData;
	DWORD       SizeOfUninitializedData;
	DWORD       AddressOfEntryPoint;
	DWORD       BaseOfCode;
	ULONGLONG   ImageBase;
	DWORD       SectionAlignment;
	DWORD       FileAlignment;
	WORD        MajorOperatingSystemVersion;
	WORD        MinorOperatingSystemVersion;
	WORD        MajorImageVersion;
	WORD        MinorImageVersion;
	WORD        MajorSubsystemVersion;
	WORD        MinorSubsystemVersion;
	DWORD       Win32VersionValue;
	DWORD       SizeOfImage;
	DWORD       SizeOfHeaders;
	DWORD       CheckSum;
	WORD        Subsystem;
	WORD        DllCharacteristics;
	ULONGLONG   SizeOfStackReserve;
	ULONGLONG   SizeOfStackCommit;
	ULONGLONG   SizeOfHeapReserve;
	ULONGLONG   SizeOfHeapCommit;
	DWORD       LoaderFlags;
	DWORD       NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;

#define IMAGE_DIRECTORY_ENTRY_EXPORT          0   // Export Directory
#define IMAGE_DIRECTORY_ENTRY_IMPORT          1   // Import Directory
#define IMAGE_DIRECTORY_ENTRY_RESOURCE        2   // Resource Directory
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION       3   // Exception Directory
#define IMAGE_DIRECTORY_ENTRY_SECURITY        4   // Security Directory
#define IMAGE_DIRECTORY_ENTRY_BASERELOC       5   // Base Relocation Table
#define IMAGE_DIRECTORY_ENTRY_DEBUG           6   // Debug Directory
//      IMAGE_DIRECTORY_ENTRY_COPYRIGHT       7   // (X86 usage)
#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE    7   // Architecture Specific Data
#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR       8   // RVA of GP
#define IMAGE_DIRECTORY_ENTRY_TLS             9   // TLS Directory
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG    10   // Load Configuration Directory
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT   11   // Bound Import Directory in headers
#define IMAGE_DIRECTORY_ENTRY_IAT            12   // Import Address Table
#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT   13   // Delay Load Import Descriptors
#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14   // COM Runtime descriptor
#define IMAGE_SIZEOF_ROM_OPTIONAL_HEADER      56
#define IMAGE_SIZEOF_STD_OPTIONAL_HEADER      28
#define IMAGE_SIZEOF_NT_OPTIONAL32_HEADER    224
#define IMAGE_SIZEOF_NT_OPTIONAL64_HEADER    240

#define IMAGE_NT_OPTIONAL_HDR32_MAGIC      0x10b
#define IMAGE_NT_OPTIONAL_HDR64_MAGIC      0x20b
#define IMAGE_ROM_OPTIONAL_HDR_MAGIC       0x107

#ifdef _WIN64
typedef IMAGE_OPTIONAL_HEADER64             IMAGE_OPTIONAL_HEADER;
typedef PIMAGE_OPTIONAL_HEADER64            PIMAGE_OPTIONAL_HEADER;
#define IMAGE_SIZEOF_NT_OPTIONAL_HEADER     IMAGE_SIZEOF_NT_OPTIONAL64_HEADER
#define IMAGE_NT_OPTIONAL_HDR_MAGIC         IMAGE_NT_OPTIONAL_HDR64_MAGIC
#else
typedef IMAGE_OPTIONAL_HEADER32             IMAGE_OPTIONAL_HEADER;
typedef PIMAGE_OPTIONAL_HEADER32            PIMAGE_OPTIONAL_HEADER;
#define IMAGE_SIZEOF_NT_OPTIONAL_HEADER     IMAGE_SIZEOF_NT_OPTIONAL32_HEADER
#define IMAGE_NT_OPTIONAL_HDR_MAGIC         IMAGE_NT_OPTIONAL_HDR32_MAGIC
#endif

typedef struct _IMAGE_NT_HEADERS64 {
	DWORD Signature;
	IMAGE_FILE_HEADER FileHeader;
	IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64, *PIMAGE_NT_HEADERS64;

typedef struct _IMAGE_NT_HEADERS {
	DWORD Signature;
	IMAGE_FILE_HEADER FileHeader;
	IMAGE_OPTIONAL_HEADER32 OptionalHeader;
} IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32;



#ifdef _WIN64
typedef IMAGE_NT_HEADERS64                  IMAGE_NT_HEADERS;
typedef PIMAGE_NT_HEADERS64                 PIMAGE_NT_HEADERS;
#else
typedef IMAGE_NT_HEADERS32                  IMAGE_NT_HEADERS;
typedef PIMAGE_NT_HEADERS32                 PIMAGE_NT_HEADERS;
#endif
#define IMAGE_SIZEOF_SHORT_NAME              8

typedef struct _IMAGE_SECTION_HEADER {
	BYTE    Name[IMAGE_SIZEOF_SHORT_NAME];
	union {
		DWORD   PhysicalAddress;
		DWORD   VirtualSize;
	} Misc;
	DWORD   VirtualAddress;
	DWORD   SizeOfRawData;
	DWORD   PointerToRawData;
	DWORD   PointerToRelocations;
	DWORD   PointerToLinenumbers;
	WORD    NumberOfRelocations;
	WORD    NumberOfLinenumbers;
	DWORD   Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

#define IMAGE_SIZEOF_SECTION_HEADER          40

typedef struct _IMAGE_IMPORT_DESCRIPTOR {
	union {
		DWORD   Characteristics;            // 0 for terminating null import descriptor
		DWORD   OriginalFirstThunk;         // RVA to original unbound IAT (PIMAGE_THUNK_DATA)
	};
	DWORD   TimeDateStamp;                  // 0 if not bound,
	// -1 if bound, and real date\time stamp
	//     in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
	// O.W. date/time stamp of DLL bound to (Old BIND)

	DWORD   ForwarderChain;                 // -1 if no forwarders
	DWORD   Name;
	DWORD   FirstThunk;                     // RVA to IAT (if bound this IAT has actual addresses)
} IMAGE_IMPORT_DESCRIPTOR;
typedef IMAGE_IMPORT_DESCRIPTOR UNALIGNED *PIMAGE_IMPORT_DESCRIPTOR;

#ifdef __cplusplus
extern "C" {
#endif

DWORD GetWin32KServiceTable();
//bool MouseCommand(BYTE chCmd);
bool SyserClearI8042OutputBuffer();
bool SyserReadPort60(BYTE* ReturnValue,bool IsMouse);
bool SyserWaitI8042InputBufferEmpty(VOID);
bool SendMouseCommand(BYTE CommandByte);
extern DWORD MouseInterruptCount;

VOID SyserSimulateKeyboardInput(BYTE scancode,bool isInterrupt);
//bool CheckPS2Keyboard();
void InitPS2MouseDevice();
DWORD GetScanCode(DWORD WispVKCode);

#ifdef __cplusplus
}; // extern "C"
#endif

void TestTouchPad();


#endif