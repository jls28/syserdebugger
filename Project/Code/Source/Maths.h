#ifndef  _MATH_H_
#define  _MATH_H_

NAME_SPACE_BEGIN

DWORD	Mul64(DWORD NulN1Low,DWORD NulN2Low,DWORD*ResHigh);//64λ�޷��ų�
DWORD	Mul64(int Num1,int Num2,int *ResHigh);//64λ�з��ų�
bool	Div64(DWORD*NumLow,DWORD*NumHigh,DWORD DivisorLow,DWORD*ResLow,DWORD*ResHigh);//64λ�޷��ų�
bool	Div64(DWORD*NumLow,int*NumHigh,int DivisorLow,DWORD*ResLow,int*ResHigh);//64λ�з��ų�
void	Neg64(DWORD*NumLow,int*NumHigh);//64λȡ����
LONG	Pow(INT a,INT n);

NAME_SPACE_END

#endif

