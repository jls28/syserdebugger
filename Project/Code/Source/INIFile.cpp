#include "StdAfx.h"
#include "INIFile.h"

NAME_SPACE_BEGIN
CINIFile::CINIFile()
{

}
CINIFile::~CINIFile()
{

}
bool CodeGetPrivateProfileString(IN LPCSTR lpAppName,IN LPCSTR lpKeyName,IN LPCSTR lpDefault,OUT LPSTR lpReturnedString,IN DWORD nSize,IN LPCSTR lpFileName)
{
	CTXTFile TxtFile;
	char * pLine;
	char* pValue,*pNewValue;
	bool bSectionFind;
	char* pSectionReturnName;
	DWORD pSectionReturnNameLen;
	DWORD  before=0,after=0;
	DWORD offset=0;
	char* lpDestKeyName;
	DWORD DestKeyNameLen;
	DWORD ValueLen;
	if(TxtFile.Open(lpFileName)==false)
		return false;
	TTXTStrList::IT BeginIt,EndIt,SectionIt;
	BeginIt = TxtFile.m_StrList.Begin();
	EndIt = TxtFile.m_StrList.End();
	bSectionFind=false;
	SectionIt=EndIt;
	for(;BeginIt!=EndIt;BeginIt++)
	{
		pLine=*BeginIt;
		if(FindSection(pLine,(const char*)lpAppName,&pSectionReturnName,&pSectionReturnNameLen)==false)
			continue;
		if(lpAppName)
		{
			BeginIt++;
			SectionIt=BeginIt;
			break;
		}
		if(lpReturnedString)
		{
			if(nSize>pSectionReturnNameLen)
			{
				TStrNCpy(&lpReturnedString[offset],pSectionReturnName,pSectionReturnNameLen);
				offset+=pSectionReturnNameLen;
				lpReturnedString[offset]=0;
				offset++;
				nSize-=(pSectionReturnNameLen+1);
				bSectionFind=true;
				continue;
			}
		}
		return bSectionFind;
	}
	if(lpAppName==NULL)//�������� section name
	{
		TxtFile.Close();
		return bSectionFind;
	}
	if(SectionIt!=EndIt)
	{
		for(;BeginIt!=EndIt;BeginIt++)//�ҵ���һ�� section �Ŀ�ʼ
		{
			pLine=*BeginIt;
			if(FindSection(pLine,NULL)==false)
				continue;
			EndIt=BeginIt;
			break;
		}
	}
	offset=0;
	for(;SectionIt!=EndIt;SectionIt++)
	{
		pLine=*SectionIt;
		if(FindKey(pLine,lpKeyName,&lpDestKeyName,&DestKeyNameLen)==false)
			continue;
		if(lpKeyName==NULL)//�������� key name.
		{
			if(lpReturnedString)
			{
				if(nSize>DestKeyNameLen)
				{
					TStrNCpy(&lpReturnedString[offset],lpDestKeyName,DestKeyNameLen);
					offset+=DestKeyNameLen;
					lpReturnedString[offset]=0;
					offset++;
					nSize-=(DestKeyNameLen+1);
					bSectionFind=true;
					continue; 
				}
			}
			continue;
		}
		pValue = TStrChr(&lpDestKeyName[DestKeyNameLen],'=');
		pNewValue=TStrTrim(pValue+1,&ValueLen);
		bSectionFind=true;
		if(lpReturnedString)
		{
			if(nSize>ValueLen)
			{
				TStrNCpy(lpReturnedString,pNewValue,ValueLen);
				lpReturnedString[ValueLen]=0;
			}
		}
		break;
	}
	if(lpKeyName==NULL)
	{
		TxtFile.Close();
		return bSectionFind;
	}
	if(bSectionFind==false)
	{
		if(lpReturnedString&&nSize)
		{
			if(lpDefault)
			{
				int DefaultLen=TStrLen(lpDefault);
				if(DefaultLen<(int)nSize)
					TStrCpy(lpReturnedString,lpDefault);
				else
					TStrNCpy(lpReturnedString,lpDefault,nSize);
				lpReturnedString[nSize-1]=0;
			}
		}
	}
	TxtFile.Close();
	return bSectionFind;
}

bool FindKey(const char*pLine,const char* KeyName,char** pDestKeyName,DWORD* dwDestKeyNameLen)
{
	const char* pFindKey,*pValue;
	int len;
	pFindKey = TStrTrimBefore(pLine,NULL);
	if(KeyName)
	{
		len = TStrLen(KeyName);
		if(TStrNICmp(pFindKey,KeyName,len))
			return false;
		pValue=TStrTrimBefore(&pFindKey[len],NULL);
		if(pValue[0]!='=')
			return false;
		if(pDestKeyName)
			*pDestKeyName=(char*)pFindKey;
		if(dwDestKeyNameLen)
			*dwDestKeyNameLen=len;
		return true;
	}
	if(pFindKey[0]=='='||pFindKey[0]==0)
		return false;
	pValue=TStrChr(pFindKey,'=');
	if(pValue==NULL)
		return false;
	if(pDestKeyName)
		*pDestKeyName=(char*)pFindKey;
	if(dwDestKeyNameLen)
	{
		len=*(int*)&pValue-*(int*)&pFindKey;
		len--;
		for(;len;len--)
			if(pFindKey[len]!=' '&&pFindKey[len]!= '\t')
				break;
		len++;
		*dwDestKeyNameLen=len;
	}
	return true;
}

bool FindSection(const char* pLine,const char* SectionName,char** pDestSectionName,DWORD* dwDestSectionNameLen)
{
	bool bSectionFind=false;
	int i,j,m,n;
	for(i = 0;pLine[i];i++)
		if(pLine[i]==' '||pLine[i]=='\t')
			continue;
		else
			if(pLine[i]!='[')
				break;
			else
			{
				i++;
				if(!pLine[i])
					break;
				for(j =i; pLine[j]; j++)//���˵��ո�
				{
					if(pLine[j]==' '||pLine[j]=='\t')
						continue;
					break;
				}
				if(!pLine[j]||pLine[j]==']')//�ж��Ƿ�Ϊ [    ]�������
					break;
 				n = j;						//n ��ʱΪ section name �Ŀ�ʼλ��
				for(;pLine[j]&&pLine[j]!=']';j++);
				if(!pLine[j])
					break;
				for(m=j-1;m>n;m--)//���˵� section name �ĺ���Ŀո�
					if(pLine[m]==' '||pLine[m]=='\t')
						continue;
					else
						break;
				
				if(SectionName)//������������ֵ� section ��Ƚ�
				{	
					if(TStrNICmp(&pLine[n],SectionName,m-n+1))
						break;
				}
				j++;
				for(;pLine[j];j++)
				{
					if(pLine[j]==' '||pLine[j]=='\t')
						continue;
					break;
				}
				if(pLine[j])
					break;
				if(pDestSectionName)
				{
					*pDestSectionName=(char*)&pLine[n];
				}
				if(dwDestSectionNameLen)
					*dwDestSectionNameLen=m-n+1;
				return true;
			}
	return bSectionFind;
}
bool CodeWritePrivateProfileString(
							   IN LPCSTR lpAppName,
							   IN LPCSTR lpKeyName,
							   IN LPCSTR lpString,
							   IN LPCSTR lpFileName
							   )
{
	CTXTFile TxtFile;
	char * pLine;
	char* pValue;
	bool bSectionFind;
	char* pSectionReturnName;
	DWORD pSectionReturnNameLen;
	DWORD  before=0,after=0;
	DWORD offset=0;
	char* lpDestKeyName;
	DWORD DestKeyNameLen;
	int len;

	if(lpAppName==NULL)
		return false;
	if(TxtFile.Open(lpFileName)==false)
		return false;
	TTXTStrList::IT BeginIt,EndIt,SectionIt;
	BeginIt = TxtFile.m_StrList.Begin();
	EndIt = TxtFile.m_StrList.End();
	bSectionFind=false;
	SectionIt=EndIt;
	for(;BeginIt!=EndIt;BeginIt++)
	{
		pLine=*BeginIt;
		if(FindSection(pLine,(const char*)lpAppName,&pSectionReturnName,&pSectionReturnNameLen)==false)
			continue;
		BeginIt++;
		SectionIt=BeginIt;
		bSectionFind=true;
		break;
	}
	if(bSectionFind==false)//����� section ����������
	{
		len=TStrLen(lpAppName)+1+1+3;//1 == [  1 == ] 3 == 0d0a00
		pValue=new char[len];
		TStrCpy(pValue,"[");
		TStrCat(pValue,lpAppName);
		TStrCat(pValue,"]");
		TxtFile.m_StrList.InsertBefore(EndIt,pValue);
		EndIt = TxtFile.m_StrList.End();
		SectionIt=EndIt;
	}
	bSectionFind=false;
	if(SectionIt!=EndIt)
	{
		for(;BeginIt!=EndIt;BeginIt++)//�ҵ���һ�� section �Ŀ�ʼ
		{
			pLine=*BeginIt;
			if(FindSection(pLine,NULL)==false)
				continue;
			EndIt=BeginIt;
			break;
		}
	}
	offset=0;
	if(lpKeyName==NULL)
	{
		for(;SectionIt!=EndIt;SectionIt++)
		{
			delete *SectionIt;
			SectionIt=TxtFile.m_StrList.Remove(SectionIt);
			bSectionFind=true;
		}
		if(bSectionFind)
		{
			TxtFile.Save(lpFileName);
		}
	}
	else
	{
		BeginIt=SectionIt;
		for(;SectionIt!=EndIt;SectionIt++)
		{
			pLine=*SectionIt;
			if(FindKey(pLine,lpKeyName,&lpDestKeyName,&DestKeyNameLen)==false)
				continue;
			bSectionFind=true;
			delete *SectionIt;//������ܻ�������⣬���� windows �� CTXTFile�ڶ�̬����ʱ��*SectionItָ������ ��̬���еĶѵ��ڴ棬
								//delete ����exe�Ķ���
			SectionIt=TxtFile.m_StrList.Remove(SectionIt);
			if(lpString==NULL)
			{
				break;
			}
			len = TStrLen(lpKeyName)+1+TStrLen(lpString)+3;
			pValue=new char[len];
			TStrCpy(pValue,lpKeyName);
			TStrCat(pValue,"=");
			TStrCat(pValue,lpString);
			//TStrCat(pValue,"\r\n");
			TxtFile.m_StrList.InsertAfter(SectionIt,pValue);
			break;
		}
		if(bSectionFind)
		{
			TxtFile.Save(lpFileName);
		}
		else
		{
			if(lpString)//����һ���µļ�
			{
				len = TStrLen(lpKeyName)+1+TStrLen(lpString)+3;
				pValue=new char[len];
				TStrCpy(pValue,lpKeyName);
				TStrCat(pValue,"=");
				TStrCat(pValue,lpString);
				//TStrCat(pValue,"\r\n");
				TxtFile.m_StrList.InsertBefore(BeginIt,pValue);
				TxtFile.Save(lpFileName);
				bSectionFind=true;
			}
		}
	}
	TxtFile.Close();
	return bSectionFind;
}

NAME_SPACE_END
