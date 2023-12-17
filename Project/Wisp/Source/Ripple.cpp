#include "StdAfx.h"
#include "Ripple.h"

CRipple::CRipple()
{
	m_buf1 = NULL;
	m_buf2 = NULL;
}

CRipple::~CRipple()
{
	CleanRippleData();
}

bool CRipple::InitRipple(int nBackWidth, int nBackHeight, int nDepth, int Pitch)
{
	m_BackWidth  = nBackWidth;
	m_BackHeight = nBackHeight;
	m_PixelBytes = (nDepth+7)/8;
	if(Pitch)
		m_Pitch = Pitch;
	else
		m_Pitch = m_PixelBytes*m_BackWidth;
    m_buf1 = new short[m_BackWidth*m_BackHeight];
	m_buf2 = new short[m_BackWidth*m_BackHeight];
	ZeroMemory(m_buf1,m_BackWidth*m_BackHeight*sizeof(short));
	ZeroMemory(m_buf2,m_BackWidth*m_BackHeight*sizeof(short));
	return true;
}

//*****************************************************
//���Ӳ�Դ
//*****************************************************
void CRipple::DropStone(int x,int y,int stonesize,int stoneweight)
{
	//�ж������Ƿ�����Ļ��Χ��
	if(x+stonesize>=m_BackWidth || y+stonesize>=m_BackHeight||x-stonesize<0||y-stonesize<0)
		return;
	for(int posx=x-stonesize; posx<x+stonesize; posx++)
		for(int posy=y-stonesize; posy<y+stonesize; posy++)
			if((posx-x)*(posx-x) + (posy-y)*(posy-y) < stonesize*stonesize)
				m_buf1[m_BackWidth*posy+posx] = -stoneweight;
}

//*******************************************************
//���㲨�����ݻ�����
//*******************************************************
bool CRipple::RippleSpread()
{
	bool Result;
	int pos;
	Result=false;
	for(int j=1; j<m_BackHeight-1; j++)
	{
		for(int i=1; i<m_BackWidth-1; i++)
		{
			pos = i+m_BackWidth*j;
			if(m_buf2[pos] = ((m_buf1[pos-1]+m_buf1[pos+1]+m_buf1[pos-m_BackWidth]+m_buf1[pos+m_BackWidth])>>1)-m_buf2[pos])
				Result = true;
			if(m_buf2[pos] -= m_buf2[pos]>>5)
				Result = true;
		}
	}
	short *ptmp =m_buf1;
	m_buf1 = m_buf2;
	m_buf2 = ptmp;
	return Result;
}

//*******************************************************
//���ݲ������ݻ�����������ҳ�������Ⱦ
//*******************************************************
void CRipple::RenderRipple(void* Bitmap1,void* Bitmap2)
{
	//�������ҳ����Ⱦ
	int pos1,pos2,xoff,yoff;
	int k = m_BackWidth;
	for(int y=1; y<m_BackHeight-1;y++)
	{
		for(int x=0; x<m_BackWidth;x++)
		{
			//����ƫ����
			xoff = m_buf1[k-1]-m_buf1[k+1];
			yoff = m_buf1[k-m_BackWidth]-m_buf1[k+m_BackWidth];
			//�ж������Ƿ��ڴ��ڷ�Χ��
			if(y+yoff < 0 || y+yoff >= m_BackHeight || x+xoff < 0 || x+xoff >= m_BackWidth)
			{
				k++;
				continue;
			}
			//�����ƫ�����غ�ԭʼ���ص��ڴ��ַƫ����
			pos1=m_Pitch*(y+yoff)+ m_PixelBytes*(x+xoff);
			pos2=m_Pitch*y+ m_PixelBytes*x;
			//��������
			for(int d=0; d<m_PixelBytes; d++)
				((BYTE*)Bitmap2)[pos2++]=((BYTE*)Bitmap1)[pos1++];
			k++;
		}
	}
}
// �ͷ��ڴ�
void CRipple::CleanRippleData()
{
	SafeDelete(m_buf1);
	SafeDelete(m_buf2);
}

