///////////////////////////////////////////////////////////////////////////////
//Queue
//Coded by Finaldoom
//队列
//作者：陈俊豪
///////////////////////////////////////////////////////////////////////////////
#ifndef _ALT_QUEUE_HPP_
#define _ALT_QUEUE_HPP_

ALT_NAME_SPACE_BEGIN

template<class VALUE,int SIZE>
class TQueue : public TArray<VALUE,SIZE>
{
protected:
	int m_HeadIndex;
public:
	TQueue()
	{
		m_HeadIndex=-1;
	}
	TQueue(const TQueue&Obj)
	{
		m_HeadIndex=-1;
		Resize(Obj.m_Size);
		for(int n=0;n<Obj.m_Count;n++)
			Append(Obj.m_ValueAry[n]);
	}
	void Clear()
	{
		TArray<VALUE,SIZE>::Clear();
		m_HeadIndex=-1;
	}
	VALUE*Append(const VALUE&Value = *(VALUE*)NULL)
	{
		if(this->m_Size==0)
			return NULL;
		if(this->m_Count< this->m_Size)
			this->m_Count++;
		m_HeadIndex++;
		m_HeadIndex = m_HeadIndex % this->m_Size;
		if(&Value)
			this->m_ValueAry[m_HeadIndex] = Value;
		return &this->m_ValueAry[m_HeadIndex];
	}
	VALUE& operator[](int n)
	{
		return this->m_ValueAry[(m_HeadIndex- this->m_Count+n+1+ this->m_Size)% this->m_Size];
	}
public:
	int	Save(ALTFileStream&Stream)
	{
		int Length;
		Length=Stream.Puts(&this->m_Size,sizeof(this->m_Size));
		Length+=Stream.Puts(&this->m_Count,sizeof(this->m_Count));
		for(int n=0;n< this->m_Count;n++)
			Length+= this->ALTArchiveType::Save(Stream,&this->m_ValueAry[(m_HeadIndex- this->m_Count+n+1+ this->m_Size)% this->m_Size],sizeof(VALUE));
		return Length;
	}
	int	Load(ALTFileStream&Stream)
	{
		int TotalLength,Length,Num;
		Clear();
		TotalLength = 0;
		Length=Stream.Gets(&Num,sizeof(Num));
		if(Length!=sizeof(Num))
			return 0;
		TotalLength+=Length;
		this->Resize(Num);
		Length=Stream.Gets(&Num,sizeof(Num));
		if(Length!=sizeof(Num))
			return 0;
		TotalLength+=Length;
		m_HeadIndex = Num?0:-1;
		for(int n=0;n<Num;n++)
		{
			Length=Append()->Load(Stream);
			if(Length==0)
				return 0;
			TotalLength+=Length;
		}
		return TotalLength;
	}
};

ALT_NAME_SPACE_END

#endif

