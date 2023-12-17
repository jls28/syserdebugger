#ifndef _CODE_CONFIG_H_
#define _CODE_CONFIG_H_

//����ϵͳѡ��
//CODE_OS_WIN		WindowӦ�ó���
//CODE_OS_NT_DRV	Window NT��������
//CODE_OS_UNIX		UNIX����

#define _CRT_SECURE_NO_WARNINGS //VC8��ȫ�⾯��

#ifdef	WIN32
#ifndef CODE_OS_NT_DRV
#define CODE_OS_WIN
#endif
#else	//WIN32
#ifndef CODE_OS_CATOS
#define CODE_OS_UNIX
#endif
#endif

#define CODE_INT_BYTES		4

#ifndef CODE_OS_WIN
#ifndef CODE_OS_NT_DRV
///////////////////////////////////////////
//���ֽ���
#define CODE_BIG_ENDIAN
///////////////////////////////////////////

///////////////////////////////////////////
//����Bus��������
#define CODE_BUS_ALIGN

///////////////////////////////////////////
//���ڶ�������Ľṹ�Զ���ָ���ֽڶ���
//#define CODE_ST_PACK
///////////////////////////////////////////
#endif //not define CODE_OS_NT_DRV
#endif //not define CODE_OS_WIN

///////////////////////////////////////////
//�ɱ��������
//#define CODE_VA_DEFINE

///////////////////////////////////////////
//UNIX���Ƿ�ʹ���߳���غ���
//#define CODE_UNIX_THREAD
///////////////////////////////////////////

///////////////////////////////////////////
//�����ڴ�й©
//#define CODE_DEBUG_ALLOC
///////////////////////////////////////////

///////////////////////////////////////////
//�����ڴ����
//#define CODE_ALLOC_OVERLOAD
///////////////////////////////////////////

///////////////////////////////////////////
//ʹ��CFixedHeap��������ڴ����
//#define CODE_ALLOC_USER_HEAP					30*0x100000
//#define CODE_ALLOC_USER_HEAP_FREE_CHECK			//�ͷ��ڴ�ʱ����ڴ�߽��־�Ƿ�������������ڴ����
//#define CODE_ALLOC_USER_HEAP_OVERFLOW_OUTPUT
//#define CODE_ALLOC_USER_HEAP_DEBUG_BREAK
///////////////////////////////////////////

////////////////////////////////////////////
//#define CODE_USE_NAME_SPACE
//ʹ��code���ֿռ�
////////////////////////////////////////////

///////////////////////////////////////////
//��������ϵ���� don't modify
///////////////////////////////////////////
#ifdef	CODE_USE_NAME_SPACE
#define NAME_SPACE				code
#define NAME_SPACE_BEGIN			namespace	NAME_SPACE{
#define NAME_SPACE_END				}
#else
#define NAME_SPACE
#define NAME_SPACE_BEGIN
#define NAME_SPACE_END
#endif

#ifdef	CODE_ALLOC_USER_HEAP
#define CODE_ALLOC_OVERLOAD
#endif

#ifndef CODE_VA_DEFINE
#ifdef  CODE_OS_NT_DRV
#define CODE_VA_DEFINE
#endif
#endif

///////////////////////////////////////////
//��������ϵ����
///////////////////////////////////////////


#endif
