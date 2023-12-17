#ifndef _RIPPLE_H_
#define _RIPPLE_H_

class CRipple
{
public:
	CRipple();
	~CRipple();
	bool	InitRipple(int nBackWidth, int nBackHeight, int nDepth, int Pitch = 0);
	void	CleanRippleData();
	void	DropStone(int x,int y,int stonesize,int stoneweight);
	bool	RippleSpread();
	void	RenderRipple(void* Bitmap1, void* Bitmap2);
private:
	int		m_BackWidth;	//����ͼ���
	int		m_BackHeight;	//����ͼ�߶�
	int		m_PixelBytes;	//��ɫ��� 
	int		m_Pitch;		//ˮƽ���
	short*	m_buf1;			//�������ݻ�����
	short*	m_buf2;			//�������ݻ�����
};
#endif

