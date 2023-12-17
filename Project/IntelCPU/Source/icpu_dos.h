/////////////////////////////////////////////////////////////////////////
// $Id: x86cpu.h,v 1.0 2002/10/08 13:10:37 bdenney Exp $
/////////////////////////////////////////////////////////////////////////

#ifndef MSDOS_X86_CPU_H
#define MSDOS_X86_CPU_H
#include "x86cpu.h"

class CMSDOS_X86_CPU :public BX_CPU_C
{
	unsigned char *LinearMemory;

	unsigned int ReadModifyWriteLinearAddress;

public:

public:
	void write_virtual_byte(unsigned seg, Bit32u offset, Bit8u *data);
	void write_virtual_word(unsigned seg, Bit32u offset, Bit16u *data);
	void write_virtual_dword(unsigned seg, Bit32u offset, Bit32u *data);
	void read_virtual_byte(unsigned seg, Bit32u offset, Bit8u *data);
	void read_virtual_word(unsigned seg, Bit32u offset, Bit16u *data);
	void read_virtual_dword(unsigned seg, Bit32u offset, Bit32u *data);

	void read_RMW_virtual_byte(unsigned seg, Bit32u offset, Bit8u *data);
	void read_RMW_virtual_word(unsigned seg, Bit32u offset, Bit16u *data);
	void read_RMW_virtual_dword(unsigned seg, Bit32u offset, Bit32u *data);
	void write_RMW_virtual_byte(Bit8u val8);
	void write_RMW_virtual_word(Bit16u val16);
	void write_RMW_virtual_dword(Bit32u val32);

	void access_linear(Bit32u address, unsigned length, unsigned pl,
				 unsigned rw, void *data);
	Bit32u itranslate_linear(Bit32u laddress, unsigned pl);
	Bit32u dtranslate_linear(Bit32u laddress, unsigned pl, unsigned rw);

	void write_push_word(unsigned seg, Bit32u offset, Bit16u *data);
	void write_push_dword(unsigned seg, Bit32u offset, Bit32u *data);
	// ����:physical page number physical_address_base:��С physical page �ı�����ַ
	int get_physical_host_memory(void **physical_address_base);
	Bit8u *getHostMemPtr(Bit32u linear_addr,unsigned op,Bit32u **access_stamp);

	// CPU ��ʼ�� 
	void Init(unsigned char *addrspace);
	// CPU ��ʼ�� 
	void Exit(void);
	// CPU ��λ��
	void Reset(void);
	//ÿ����һ�α�ִ��������ָ�����ִ�е�ָ������ ������-1��ִ��ʧ��.
	int Steps(int steps=1);

	void UpdateContext(Boolean b);
};

#endif //#ifndef MSDOS_X86_CPU_H

