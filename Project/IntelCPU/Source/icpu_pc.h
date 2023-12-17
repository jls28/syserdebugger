/////////////////////////////////////////////////////////////////////////
// $Id: x86cpu.h,v 1.0 2002/10/08 13:10:37 bdenney Exp $
/////////////////////////////////////////////////////////////////////////

#ifndef BIOS_X86_CPU_H
#define BIOS_X86_CPU_H
#include "x86cpu.h"


#define BIOS_MEMORY_READ 1
#define BIOS_MEMORY_WRITE 2
#define BIOS_MEMORY_READWRITE 3

class CBIOS_X86_CPU :public BX_CPU_C
{
	Bit8u *(*GetPageAddress)(unsigned int addr,int rw,void *user);
	void *user;
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


	void access_io(Bit16u addr, unsigned length,unsigned pl,
					  unsigned rw,void *data);

	void access_linear(Bit32u address, unsigned length, unsigned pl,
				 unsigned rw, void *data);
	Bit32u itranslate_linear(Bit32u laddress, unsigned pl);
	Bit32u dtranslate_linear(Bit32u laddress, unsigned pl, unsigned rw);

	void write_push_word(unsigned seg, Bit32u offset, Bit16u *data);
	void write_push_dword(unsigned seg, Bit32u offset, Bit32u *data);
	// ����:physical page number physical_address_base:��С physical page �ı�����ַ
	int get_physical_host_memory(void **physical_address_base);
	Bit8u *getHostMemPtr(Bit32u linear_addr,unsigned op,Bit32u **access_stamp);

	// CPU ��ʼ�� (rw == 2 Ϊ write) (rw == 1 Ϊ read) (rw == 3 Ϊ read write)
	void Init(Bit8u *(*GetPageAddress)(unsigned int addr,int rw,void *user),void *user);
	// CPU ��ʼ�� 
	void Exit(void);
	// CPU ��λ��
	void Reset(void);
	//ÿ����һ�α�ִ��������ָ�����ִ�е�ָ������ ������-1��ִ��ʧ��.
	int Steps(int steps=1);
};

#endif //#ifndef MSDOS_X86_CPU_H

