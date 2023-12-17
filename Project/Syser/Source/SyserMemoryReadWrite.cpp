#include "StdAfx.h"
#include "SyserMemoryReadWrite.h"

#define JLS

#ifdef __cplusplus
extern "C" {
#endif

bool SyserCFlag(VOID);

#ifdef JLS
bool SyserCFlag(VOID)
{
	return true;
}

int __stdcall SyserPrivateMemCopy(BYTE* Address1, BYTE* Address2, DWORD Len)
{
	memcpy(Address1, Address2, Len);
	return true;
}

BYTE* __stdcall SyserPrivateFindByte(BYTE* Address1, BYTE Value, DWORD Len)
{

	while (Len-- && (*Address1 != Value))
	{
		Address1 += 1;
	}
	return Len ? Address1 : NULL;
}
#endif

#ifdef __cplusplus
}; // extern "C"
#endif

bool SafeWriteByte(void* Address,BYTE ByteValue)
{
	bool bSuccess=true;
#ifndef JLS
	SyserPrivateSafeWriteByte(Address,ByteValue);
// 	__asm
// 	{
// 		jnc local_001
// 		mov bSuccess,0
// local_001:
// 	}
#else 
	DebugBreak();
#endif

	bSuccess = SyserCFlag();

	return bSuccess;
}

bool SafeWriteWord(void* Address,WORD WordValue)
{
	bool bSuccess=true;
#ifndef JLS
	SyserPrivateSafeWriteWord(Address,WordValue);
// 	__asm
// 	{
// 		jnc local_001
// 		mov bSuccess,0
// local_001:
// 	}
#else
	DebugBreak();
#endif
	
	bSuccess = SyserCFlag();

	return bSuccess;
}

bool SafeWriteDword(void* Address,DWORD DwordValue)
{
	bool bSuccess=true;
#ifndef JLS
	SyserPrivateSafeWriteDword(Address,DwordValue);
// 	__asm
// 	{
// 		jnc local_001
// 		mov bSuccess,0
// local_001:
// 	}
#else
	DebugBreak();
#endif

	bSuccess = SyserCFlag();

	return bSuccess;
}

bool WriteByte(void* Address,BYTE ByteValue)
{
	bool bSuccess=true;
#ifndef JLS
	SyserPrivateWriteByte(Address,ByteValue);
// 	__asm
// 	{
// 		jnc local_001
// 		mov bSuccess,0
// local_001:
// 	}
#else
	* static_cast<BYTE*>(Address) = ByteValue;
#endif

	bSuccess = SyserCFlag();

	return bSuccess;
}

bool WriteWord(void* Address,WORD WordValue)
{
	bool bSuccess=true;
#ifndef JLS
	SyserPrivateWriteWord(Address,WordValue);
// 	__asm
// 	{
// 		jnc local_001
// 		mov bSuccess,0
// local_001:
// 	}
#else
	*static_cast<WORD*>(Address) = WordValue;
#endif
	bSuccess = SyserCFlag();

	return bSuccess;
}

bool WriteDword(void* Address,DWORD DwordValue)
{
	bool bSuccess=true;
#ifndef JLS
	SyserPrivateWriteDword(Address,DwordValue);
// 	__asm
// 	{
// 		jnc local_001
// 		mov bSuccess,0
// local_001:
// 	}
#else
	*static_cast<DWORD*>(Address) = DwordValue;
#endif

	bSuccess = SyserCFlag();

	return bSuccess;
}

bool ReadByte(void* Address,BYTE* ByteValue)
{
	bool Success;
	*ByteValue=ReadByte(Address,&Success);
	return Success;
}

bool ReadWord(void* Address,WORD* WordValue)
{
	bool Success;
	*WordValue=ReadWord(Address,&Success);
	return Success;
}

bool ReadDword(void* Address,DWORD* DwordValue)
{
	bool Success;
	*DwordValue=ReadDword(Address,&Success);
	return Success;
}

BYTE ReadByteDefaultValue(void* Address,BYTE DefaultValue)
{
	BYTE ByteValue;
	bool Success;
	ByteValue = ReadByte(Address,&Success);
	if(Success==false)
		ByteValue=DefaultValue;
	return ByteValue;
}

WORD ReadWordDefaultValue(void* Address,WORD DefaultValue)
{
	WORD WordValue;
	bool Success;
	WordValue = ReadWord(Address,&Success);
	if(Success==false)
		WordValue=DefaultValue;
	return WordValue;
}

DWORD ReadDwordDefaultValue(void* Address,DWORD DefaultValue)
{
	DWORD DwordValue;
	bool Success;
	DwordValue = ReadDword(Address,&Success);
	if(Success==false)
		DwordValue=DefaultValue;
	return DwordValue;
}

BYTE ReadByte(void* Address,bool* Success)
{
	BYTE RetValue;
	bool bSuccess=true;
#ifndef JLS
	RetValue = SyserPrivateReadByte(Address);
// 	__asm
// 	{
// 		jnc local_001
// 		mov bSuccess,0
// local_001:
// 	}
#else
	RetValue =*static_cast<BYTE*>(Address);
#endif

	bSuccess = SyserCFlag();

	if(Success)
		*Success=bSuccess;
	return RetValue;
}

WORD ReadWord(void* Address,bool* Success)
{
	WORD RetValue;
	bool bSuccess=true;
#ifndef JLS
	RetValue = SyserPrivateReadWord(Address);
// 	__asm
// 	{
// 		jnc local_001
// 		mov bSuccess,0
// local_001:
// 	}
#else
	RetValue = *static_cast<const WORD*>(Address);
#endif

	bSuccess = SyserCFlag();

	if(Success)
		*Success=bSuccess;
	return RetValue;
}

DWORD ReadDword(void* Address,bool* Success)
{
	DWORD RetValue;
	bool bSuccess=true;
#ifndef JLS
	RetValue = SyserPrivateReadDword(Address);
// 	__asm
// 	{
// 		jnc local_001
// 		mov bSuccess,0
// local_001:
// 	}
#else
	RetValue = *static_cast<const DWORD*>(Address);
#endif

	bSuccess = SyserCFlag();

	if(Success)
		*Success=bSuccess;
	return RetValue;
}
// 
// __declspec(naked) BYTE __stdcall PrivateReadByte(void* Address)
// {
// 	__asm
// 	{
// 		push esi
// 		push eax
// 		mov esi,[esp+0xc]
// 		movzx eax,byte ptr[esi]
// 		lea esp,[esp+4]
// 		pop esi
// 		clc
// 		ret 4
// 	}
// }
// 
// __declspec(naked)WORD __stdcall PrivateReadWord(void* Address)
// {
// 	__asm
// 	{
// 		push esi
// 		push eax
// 		mov esi,[esp+0xc]
// 		movzx eax,word ptr[esi]
// 		lea esp,[esp+4]
// 		pop esi
// 		clc
// 		ret 4
// 	}
// }
// 
// __declspec(naked)DWORD __stdcall PrivateReadDword(void* Address)
// {
// 	__asm
// 	{
// 		push esi
// 		push eax
// 		mov esi,[esp+0xc]
// 		mov eax,[esi]
// 		lea esp,[esp+4]
// 		pop esi
// 		clc
// 		ret 4
// 	}
//}
// 
// __declspec(naked)void __stdcall PrivateReadError()
// {
// 	__asm{
// 		pop eax
// 		pop esi
// 		stc
// 		ret 4
// 	}
// }
// 
// __declspec(naked)void __stdcall PrivateWriteByte(void* Address,BYTE ByteValue)
// {
// 	__asm
// 	{
// 		push esi
// 		push eax
// 		mov esi,[esp+0xc]
// 		mov al,[esp+0x10]
// 		mov [esi],al
// 		pop eax
// 		pop esi
// 		clc
// 		ret 8
// 	}
// }
// 
// __declspec(naked)void __stdcall PrivateWriteWord(void* Address,WORD WordValue)
// {
// 	__asm
// 	{
// 		push esi
// 		push eax
// 		mov esi,[esp+0xc]
// 		mov ax,[esp+0x10]
// 		mov [esi],ax
// 		pop eax
// 		pop esi
// 		clc
// 		ret 8
// 	}
// }
// 
// __declspec(naked)void __stdcall PrivateWriteDword(void* Address,DWORD DwordValue)
// {
// 	__asm
// 	{
// 		push esi
// 		push eax 
// 		mov esi,[esp+0xc]
// 		mov eax,[esp+0x10]
// 		mov [esi],eax
// 		pop eax
// 		pop esi
// 		clc
// 		ret 8
// 	}
// }
// 
// __declspec(naked)void __stdcall PrivateSafeWriteByte(void* Address,BYTE ByteValue)
// {
// 	__asm
// 	{
// 		push esi
// 		push eax
// 		mov esi,[esp+0xc]
// 		mov al,[esi]//����������ַ��ʧ�ܷ���
// 		mov eax,cr0
// 		push eax 
// 		and eax,0xfffeffff 
// 		mov cr0,eax 
// 		mov al,[esp+0x14]
// 		mov [esi],al 
// 		pop eax 
// 		mov cr0,eax
// 		pop eax
// 		pop esi
// 		clc
// 		ret 8
// 	}
// }
// 
// __declspec(naked)void __stdcall PrivateSafeWriteWord(void* Address,WORD WordValue)
// {
// 	__asm
// 	{
// 		push esi
// 		push eax
// 		mov esi,[esp+0xc]
// 		mov ax,[esi]//����������ַ��ʧ�ܷ���
// 		mov eax,cr0
// 		push eax 
// 		and eax,0xfffeffff 
// 		mov cr0,eax 
// 		mov ax,[esp+0x14]
// 		mov [esi],ax 
// 		pop eax 
// 		mov cr0,eax
// 		pop eax
// 		pop esi
// 		clc
// 		ret 8
// 	}
// }
// 
// __declspec(naked)void __stdcall PrivateSafeWriteDword(void* Address,DWORD DwordValue)
// {
// 	__asm
// 	{
// 		push esi
// 		push eax
// 		mov esi,[esp+0xc]
// 		mov eax,[esi]//����������ַ��ʧ�ܷ���
// 		mov eax,cr0
// 		push eax
// 		and eax,0xfffeffff
// 		mov cr0,eax
// 		mov eax,[esp+0x14]
// 		mov [esi],eax
// 		pop eax
// 		mov cr0,eax
// 		pop eax
// 		pop esi
// 		clc
// 		ret 8
// 	}
// }
// 
// __declspec(naked)void __stdcall PrivateWriteError()
// {
// 	__asm{
// 		pop eax
// 		pop esi
// 		stc
// 		ret 8
// 	}
// }
// 
// __declspec(naked)int __stdcall PrivateMemCopy(BYTE* DestAddress,BYTE* SourceAddress,DWORD Len)
// {
// 	__asm
// 	{
// 		push ebp 
// 		mov ebp,esp
// 		sub esp,4
// 		pushfd
// 		push edi
// 		push esi 
// 		push ecx
// 		cld
// 		xor eax,eax
// 		mov ecx,[ebp+0x10]
// 		cmp ecx,0					//�жϿ����ĳ����Ƿ�Ϊ�㣬���������ֱ���˳���
// 		jz local_006
// 		mov esi,[ebp+0xc]
// 		cmp esi,[ebp+8]            //�Ƚ�ԭ��ַ��Ŀ���ַ�Ƿ���ͬ�������ͬ��ֱ���˳���
// 		jz local_006				
// 		shr ecx,2
// 		jecxz local_001
// 		lodsb 
// 		dec esi
// 		rep lodsd
// local_001:
// 		mov ecx,[ebp+0x10]
// 		and ecx,3 
// 		jecxz local_002
// 		rep lodsb					//�ж�Դ��ַ�Ƿ�ȱҳ
// local_002:
// 		mov ecx,[ebp+0x10]
// 		mov esi,[ebp+8]
// 		shr ecx,2 
// 		jecxz local_003
// 		lodsb
// 		dec esi
// 		rep lodsd
// local_003:
// 		mov ecx,[ebp+0x10]
// 		and ecx,3 
// 		jecxz local_004
// 		rep lodsb					//�ж�Ŀ���ַ�Ƿ�ȱҳ
// local_004:
// 		mov edi,[ebp+8]
// 		mov esi,[ebp+0xc]
// 		mov ecx,[ebp+0x10]
// #ifdef CODE_OS_NT_DRV
// 		mov eax,cr0
// 		mov [ebp-4],eax				//����ԭʼ�� cr0 ��Ϣ		
// 		and eax,0xfffeffff 
// 		mov cr0,eax					//��ʱ��ֹд����λ
// #endif
// 		cmp edi,esi 
// 		jb local_009
// 		lea eax,[esi+ecx]
// 		cmp edi,eax 
// 		jae local_009				//�ж�Ŀ��Ŀ�ʼ��ַ�Ƿ���ԭ���ݵķ�Χ֮�ڣ���ֹ���ݱ�����
// 		std
// 		mov esi,eax 
// 		lea edi,[edi+ecx]
// 		mov eax,ecx 
// 		shr ecx,2
// 		jecxz local_010
// 		lea esi,[esi-4]
// 		lea edi,[edi-4]
// 		rep movsd
// 		lea esi,[esi+3]
// 		lea edi,[edi+3]
// 		jmp local_011
// local_010:
// 		dec esi 
// 		dec edi
// local_011:
// 		mov ecx,eax 
// 		and ecx,3 
// 		rep movsb
// 		jmp local_007
// local_009:	
// 		mov eax,ecx
// 		shr ecx,2
// 		jecxz local_005
// 		rep movsd 
// local_005:
// 		mov ecx,eax
// 		and ecx,3 
// 		jecxz local_007
// 		rep movsb
// local_007:
// #ifdef CODE_OS_NT_DRV
// 		mov eax,[ebp-4]		
// 		mov cr0,eax 
// #endif
// local_006:
// 		mov eax,[ebp+0x10]
// 		pop ecx
// 		pop esi
// 		pop edi
// 		popfd
// 		mov esp,ebp
// 		pop ebp
// 		clc
// 		ret 0xc
// 	}
// }
// 
// 
// __declspec(naked)int __stdcall VideoMemCopy(BYTE* DestAddress,BYTE* SourceAddress,DWORD Len)
// {
// 	__asm
// 	{
// 		push ebp 
// 		mov ebp,esp
// 		sub esp,4
// 		pushfd
// 		push edi
// 		push esi 
// 		push ecx
// 		cld
// 		xor eax,eax
// 		mov ecx,[ebp+0x10]
// 		cmp ecx,0					//�жϿ����ĳ����Ƿ�Ϊ�㣬���������ֱ���˳���
// 		jz local_006
// 		mov esi,[ebp+0xc]
// 		mov edi,[ebp+8]            //�Ƚ�ԭ��ַ��Ŀ���ַ�Ƿ���ͬ�������ͬ��ֱ���˳���
// 									//?�����������һ��Ƚ� - ���޾�
// 		jz local_006				
// 		shr ecx,2
// 		jecxz local_001
// 		rep movsd
// local_001:
// 		mov ecx,[ebp+0x10]
// 		and ecx,3
// 		jecxz local_006
// 		rep movsb
// local_006:
// 		mov eax,[ebp+0x10]
// 		pop ecx
// 		pop esi
// 		pop edi
// 		popfd
// 		mov esp,ebp
// 		pop ebp
// 		clc
// 		ret 0xc
// 	}
// }
// 
// __declspec(naked)void __stdcall PrivateMemCopyError()
// {
// 	__asm{
// 		pop ecx
// 		pop esi
// 		pop edi
// 		popfd
// 		mov esp,ebp
// 		pop ebp
// 		stc
// 		ret 0xc
// 	}
// }
// 
// __declspec(naked)BYTE* __stdcall PrivateFindByte(BYTE* Address1,BYTE Value,DWORD Len)
// {
// 	__asm
// 	{
// 		pushfd
// 		push edi
// 		push esi
// 		push ecx
// 		cld
// 		mov eax,0
// 		mov ecx,[esp+0x1c]
// 		mov edi,[esp+0x14]
// 		mov al,[esp+0x18]
// 		repne scasb
// 		mov eax,edi
// 		jz local_001
// 		mov eax,1
// local_001:
// 		lea eax,[eax-1]
// 		pop ecx
// 		pop esi
// 		pop edi
// 		popfd
// 		clc
// 		ret 0xc
// 	}
// }
// 
// __declspec(naked)BYTE* __stdcall PrivateFindWord(BYTE* Address1,WORD Value,DWORD Len)
// {
// 	__asm
// 	{
// 		pushfd
// 		push edi
// 		push esi
// 		push ecx
// 		cld
// 		mov eax,0
// 		mov ecx,[esp+0x1c]
// 		mov edi,[esp+0x14]
// 		mov ax,[esp+0x18]
// 		repne scasw
// 		mov eax,edi
// 		jz local_001
// 		mov eax,2
// local_001:
// 		lea eax,[eax-2]
// 		pop ecx
// 		pop esi
// 		pop edi
// 		popfd
// 		clc
// 		ret 0xc
// 	}
// }
// 
// __declspec(naked)BYTE* __stdcall PrivateFindDword(BYTE* Address1,DWORD Value,DWORD Len)
// {
// 	__asm
// 	{
// 		pushfd
// 		push edi
// 		push esi
// 		push ecx
// 		cld
// 		mov eax,0
// 		mov ecx,[esp+0x1c]
// 		mov edi,[esp+0x14]
// 		mov eax,[esp+0x18]
// 		repne scasd
// 		mov eax,edi 
// 		jz local_001
// 		mov eax,4
// local_001:
// 		lea eax,[eax-4]
// 		pop ecx
// 		pop esi
// 		pop edi
// 		popfd
// 		clc
// 		ret 0xc
// 	} 
// }
// 
// __declspec(naked)void __stdcall PrivateFindMemoryError()
// {
// 	__asm{
// 		pop ecx
// 		pop esi
// 		pop edi
// 		popfd
// 		stc
// 		ret 0xc
// 	}
// }

bool MemSearch(BYTE* SourceAddrss,DWORD SourceLen,BYTE *DestinationAddress,DWORD DestinationLen,BYTE** ErrorAddress)
{
	bool bSuccess=true;
	BYTE * pStr;
#ifndef JLS
	pStr = SyserPrivateMemSearch(SourceAddrss,SourceLen,DestinationAddress,DestinationLen);
// 	__asm
// 	{
// 		jnc local_001
// 		mov bSuccess,0
// local_001:
// 	}
#else
	DebugBreak();
#endif

	bSuccess = SyserCFlag();

	if(ErrorAddress)
		*ErrorAddress=pStr;
	return bSuccess;
}

BYTE* MemSearch(BYTE* SourceAddrss,DWORD SourceLen,BYTE *DestinationAddress,DWORD DestinationLen,bool* Success)
{
	bool bSuccess=true;
	BYTE * pStr;
#ifndef JLS
	pStr = SyserPrivateMemSearch(SourceAddrss,SourceLen,DestinationAddress,DestinationLen);
// 	__asm
// 	{
// 		jnc local_001
// 		mov bSuccess,0
// local_001:
// 	}
#else
	DebugBreak();
#endif

	bSuccess = SyserCFlag();

	if(Success)
		*Success=bSuccess;
	return pStr;
}
// 
// /*
// SourceAddrss			[esp+0x10]
// SourceLen				[esp+0x14]
// DestinationAddress		[esp+0x1c]
// DestinationLen			[esp+0x20]
// */
// __declspec(naked)BYTE* __stdcall PrivateMemSearch(BYTE* SourceAddrss,DWORD SourceLen,BYTE *DestinationAddress,DWORD DestinationLen)
// {
// #define ARG_SourceAddrss 0x18
// #define ARG_SourceLen ARG_SourceAddrss+4
// #define ARG_DestinationAddress ARG_SourceAddrss+8
// #define ARG_DestinationLen ARG_SourceAddrss+12
// 	__asm
// 	{
// 		push edi
// 		push esi
// 		push ecx
// 		push edx
// 		push ebx
// 		cld
// 		mov ecx,[esp+ARG_SourceLen]
// 		jecxz local_002
// 		cmp ecx,[esp+ARG_DestinationLen]
// 		jnb local_001
// local_006:
// 		mov eax,0 
// 		jmp local_002
// local_001:
// 		sub ecx,[esp+ARG_DestinationLen]
// 		inc ecx
// 		mov edi,[esp+ARG_SourceAddrss]
// local_008:
// 		mov esi,[esp+ARG_DestinationAddress]
// 		mov al,[esi]
// 		repne scasb
// 		jnz local_002
// 		mov ebx,ecx 
// 		mov edx,edi 
// 		mov ecx,[esp+ARG_DestinationLen]
// 		lea edi,[edi-1]
// 		repz cmpsb
// 		jz local_009
// 		mov ecx,ebx 
// 		jecxz local_006
// 		mov edi,edx 
// 		jmp local_008
// local_009:
// 		mov eax,edx 
// 		lea eax,[eax-1]
// 		jmp local_005
// local_002:
// 		mov eax,0
// local_005:
// 		pop ebx 
// 		pop edx
// 		pop ecx
// 		pop esi
// 		pop edi
// 		clc
// 		ret 0x10
// 	}
// }
// 
// __declspec(naked)void  __stdcall PrivateMemorySearchError()
// {
// 	__asm{
// 		mov eax,edi
// 		pop ebx 
// 		pop edx
// 		pop ecx
// 		pop esi
// 		pop edi
// 		stc
// 		ret 0x10
// 	}
// }
// 
// __declspec(naked)int __stdcall PrivatePagePresent(BYTE* Address,DWORD Len)
// {
// 	__asm
// 	{
// 		push esi
// 		push ecx 
// 		cld 
// 		mov esi,[esp+0x0c]
// 		mov ecx,[esp+0x10]
// 		jecxz local_002
// 		shr ecx,2 
// 		jecxz local_001
// 		lodsd
// local_001:
// 		mov ecx,[esp+0x10]
// 		and ecx,3 
// 		jecxz local_002 
// 		lodsb 
// local_002:
// 		mov eax,1
// 		pop ecx 
// 		pop esi
// 		clc
// 		ret 8
// 	}
// }
// __declspec(naked)int __stdcall PrivatePageNotPresent()
// {
// 	__asm 
// 	{
// 		pop ecx 
// 		pop esi 
// 		mov eax,0 
// 		stc
// 		ret 8
// 	}
// }

bool SafeMemCmp(BYTE* SourceAddress,BYTE* DestAddress,DWORD Len,DWORD* ReturnLen)
{
	bool bSuccess=true;
	DWORD  RetValue;
#ifndef JLS
	RetValue = SyserPrivateMemCmp(SourceAddress,DestAddress,Len);
// 	__asm{
// 		jnc local_001
// 		mov bSuccess,0
// local_001:
// 	}
#else
	DebugBreak();
#endif

	bSuccess = SyserCFlag();

	if(ReturnLen)
		*ReturnLen=RetValue-*(DWORD*)&SourceAddress;
	return bSuccess;
}
// __declspec(naked)DWORD  __stdcall PrivateMemCmp(BYTE* SourceAddr,BYTE* DestAddr,DWORD Len)
// {
// 	__asm
// 	{
// 		push edi
// 		push esi
// 		push ecx
// 		mov esi,[esp+0x10]
// 		mov edi,[esp+0x14]
// 		mov ecx,[esp+0x18]
// 		jecxz local_002
// 		cld
// 		repz cmpsb
// 		jz local_002
// 		lea esi,[esi-1]
// local_002:		
// 		mov eax,esi
// 		pop ecx
// 		pop esi
// 		pop edi
// 		clc
// 		ret 0xc
// 	}
// }
// __declspec(naked)DWORD __stdcall PrivateMemCmpError()
// {
// 	__asm 
// 	{
// 		mov eax,esi
// 		pop ecx 
// 		pop esi 
// 		pop edi
// 		stc
// 		ret 0xc
// 	}
// }
#if 0 // JLS
bool SafeVideoMemCopy(BYTE* DestAddress,BYTE* SourceAddress,DWORD Len)
{
	bool bSuccess=true;
#ifndef JLS
	SyserVideoMemCopy(DestAddress,SourceAddress,Len);
// 	__asm{
// 		jnc local_001
// 		mov bSuccess,0
// local_001:
// 	}
#else
	DebugBreak();
#endif

	bSuccess = SyserCFlag();

	return bSuccess;
}
#endif
int SafeMemCopy(BYTE* DestAddress,BYTE* SourceAddress,DWORD Len)
{
	bool bSuccess=true;
	DWORD dwDest;
	DWORD dwDestLen,dwSourceLen;
	DWORD dwSource;
	DWORD dwLen;
	DWORD i;
	BYTE byteValue;
	SyserPrivateMemCopy(DestAddress,SourceAddress,Len);
// 	__asm{
// 		jnc local_001
// 		mov bSuccess,0
// local_001:
// 	}

	bSuccess = SyserCFlag();

	if(bSuccess)
		return Len;
	dwDest = *(DWORD*)&DestAddress;	
	dwLen = dwDest&0xfff;
	dwLen+=Len;
	dwDest=dwDest & 0xfffff000;
	dwDestLen=Len;
	for(i=0;i<dwLen;i+=0x1000,dwDest+=0x1000)
	{
		if(ReadByte(*(void**)&dwDest,&byteValue)==false)
		{
			dwDestLen = dwDest- *(DWORD*)&DestAddress;
			break;
		}
	}	
	dwSource = *(DWORD*)&SourceAddress;
	dwLen = dwSource&0xfff;
	dwLen+=Len;
	dwSource=dwSource & 0xfffff000;
	dwSourceLen=Len;
	for(i=0;i<dwLen;i+=0x1000,dwSource+=0x1000)
	{
		if(ReadByte(*(void**)&dwSource,&byteValue)==false)
		{
			dwSourceLen = dwSource- *(DWORD*)&SourceAddress;
			break;
		}
	}
	Len=MIN(dwSourceLen,dwDestLen);
	bSuccess=true;
	SyserPrivateMemCopy(DestAddress,SourceAddress,Len);
// 	__asm{
// 		jnc local_002
// 		mov bSuccess,0
// local_002:
// 	}

	bSuccess = SyserCFlag();

	if(bSuccess)
		return Len;
	return 0;
}