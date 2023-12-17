///////////////////////////////////////////////////////////////////////////////
//Stach
//Coded by Finaldoom
//Õ»
//×÷Õß£º³Â¿¡ºÀ
///////////////////////////////////////////////////////////////////////////////
#ifndef _ALT_STACK_HPP_
#define _ALT_STACK_HPP_

ALT_NAME_SPACE_BEGIN

template<class VALUE,int Size>
class TStack : public TQueue<VALUE,Size>
{
public:
	void Push(const VALUE&Value)
	{
		this->Append(Value);
	}
	bool Pop(VALUE&Value)
	{
		int Index;
		if(this->m_Count<=0)
			return false;
		Index=(this->m_HeadIndex+ this->m_Size)% this->m_Size;
		this->m_HeadIndex--;
		this->m_HeadIndex = this->m_HeadIndex % this->m_Size;
		this->m_Count--;
		Value = this->m_ValueAry[Index];
		return true;
	}
	VALUE& operator[](int n)
	{
		return this->m_ValueAry[(this->m_HeadIndex+ this->m_Size-n)% this->m_Size];
	}
};

ALT_NAME_SPACE_END

#endif

