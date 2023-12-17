/////////////////////////////////////////////////////////////////////////
// $Id: x86cpu.h,v 1.0 2002/10/08 13:10:37 bdenney Exp $
/////////////////////////////////////////////////////////////////////////

#ifndef MSWIN_X86_CPU_H
#define MSWIN_X86_CPU_H
#include "x86cpu.h"

class CVEMemory;

class CMSWIN_X86_CPU :public BX_CPU_C
{
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

	//X86 CPU ��ʼ�� 
	void Init(CVEMemory *addrspace);
	//X86 CPU ��ʼ�� 
	void Exit(void);
	//X86 CPU ��λ��
	void Reset(void);
	//ÿ����һ�α�ִ��������ָ�����ִ�е�ָ������ ������-1��ִ��ʧ��.
	int Execute(int steps=1);
	//һ�β�����װ�����е� CPU �Ĵ���
	int LoadAll286(LOADALL286 *pLoadAll);
	int LoadAll386(LOADALL386 *pLoadAll);
	Boolean WriteSegmentCache286(bx_segment_reg_t *pSegment,LOADALL286_DESCRIPTOR_T *pLoadDescriptor);
	Boolean WriteSegmentCache386(bx_segment_reg_t *pSegment,LOADALL386_DESCRIPTOR_T *pLoadDescriptor);
	//�쳣�º���
	Boolean IntelIntFault(int _vector);
	Boolean IntelIntIret(int _vector);
	void UpdateContext(Boolean b);
	CVEMemory *LinearMemory;
	unsigned int ReadModifyWriteLinearAddress;
};

#endif //#ifndef MSWIN_X86_CPU_H

