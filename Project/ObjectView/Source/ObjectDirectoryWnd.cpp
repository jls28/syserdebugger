#include "StdAfx.h"
#include "ObjectDirectoryWnd.h"
#ifdef CODE_OS_WIN
#include "Ring3Object.h"
#endif
WISP_MSG_MAP_BEGIN(CObjectDirectoryWnd)
WISP_MSG_MAP(WISP_WM_CREATE,OnCreate)
WISP_MSG_MAP_ON_COMMAND
WISP_MSG_MAP_END(CWispSplitWnd)

WISP_MSG_CMD_MAP_BEGIN(CObjectDirectoryWnd)
WISP_MSG_CMD_MAP(CHILD_WND_ID_OBJECTDIRECTORYTRE,OnCmdObjectDirectory)
WISP_MSG_CMD_MAP(CHILD_WND_ID_OBJECTLIST,OnCmdObjectList)
WISP_MSG_CMD_MAP_END

bool CObjectDirectoryWnd::OnCmdObjectDirectory(IN WISP_MSG *pMsg)
{	
	NUM_PTR NumPtr;
	WCHAR* String;
	WISP_CHAR	wcBuf[MAX_PATH]={0};
	DWORD		ObjectHandle;
	HANDLE hSubItem,hItem;
	if(pMsg->Command.CmdMsg==WISP_CMD_ITEM_EXPANDED||pMsg->Command.CmdMsg==WISP_CMD_ITEM_SELECTED)
	{
		hItem = (HANDLE)pMsg->Command.Param2;
		hSubItem = m_ObjectDirectoryTree.GetItem(0,hItem);
		if(hSubItem==NULL)
		{
			m_ObjectList.ClearChildItem();
			ObjectList(hItem);
			return true;
		}
		NumPtr = m_ObjectDirectoryTree.GetItemData(hItem,0);
		ObjectHandle = *(DWORD*)&NumPtr;
		String = m_ObjectDirectoryTree.GetItemText(hSubItem,0);
		
		if(TStrCmp(String, WISP_STR(".."))==0)
		{
			m_ObjectDirectoryTree.ClearChildItem(hItem);
			m_ObjectList.ClearChildItem();
#ifdef CODE_OS_WIN
			InitContext(hItem);
#else
			InitContext(ObjectHandle,hItem);
#endif
			
		}	
		else
		{
			m_ObjectList.ClearChildItem();
			ObjectList(hItem);
		}
	}
	if(pMsg->Command.CmdMsg==WISP_CMD_ITEM_UNEXPANDED)
	{
		hItem = (HANDLE)pMsg->Command.Param2;
		hSubItem = m_ObjectDirectoryTree.GetItem(0,hItem);
		if(hSubItem==NULL)
			return true;
		m_ObjectDirectoryTree.ClearChildItem(hItem);
		m_ObjectDirectoryTree.InsertItem(WSTR(".."),hItem);
	}
	return true;
}

bool CObjectDirectoryWnd::OnCmdObjectList(IN WISP_MSG *pMsg)
{		
	return true;
}

bool CObjectDirectoryWnd::OnCreate(IN WISP_MSG *pMsg)
{	
	m_ObjectDirectoryTree.Create(NULL,CWispRect(0,0,0,0),this,CHILD_WND_ID_OBJECTDIRECTORYTRE,WISP_WLS_TREE);
	m_ObjectList.Create(NULL,CWispRect(0,0,0,0),this,CHILD_WND_ID_OBJECTLIST,WISP_WLS_TREE|WISP_WLS_COLUMN_TITLE);
	InsertWnd(&m_ObjectDirectoryTree,0,m_ClientRect.cx / 3);
	InsertWnd(&m_ObjectList,0,m_ClientRect.cx / 3 *2);
	AdjustWndPos();
	m_ObjectList.InsertColumn(WSTR("Name"),150);
	m_ObjectList.InsertColumn(WSTR("Type"),80);
	m_ObjectList.InsertColumn(WSTR("SymLink"),100);
	m_ObjectDirectoryTree.InsertColumn(NULL,200);
	m_ObjectList.SetOwner(this);
	m_ObjectDirectoryTree.SetOwner(this);
#ifdef CODE_OS_NT_DRV
	InitContext(0,NULL);
#else
	InitContext();
#endif
	return true;
}

bool CObjectDirectoryWnd::UpdateContext()
{

	return true;
}
bool CObjectDirectoryWnd::InitContext()
{
#if CODE_OS_NT_DRV
	DWORD ObjectHandle;
	OBJFINDDATA FindData;
	WISP_CHAR TypeName[65],ObjectName[65];
	HANDLE hItem,hListItem,hChildItem;
	ObjectHandle = dwObjectRootDirectory;	
	FindData.ObjectHandle=ObjectHandle;
	FindData.NextObjectPointer=NULL;
	FindData.DirectoryItemIndex=0;
	hItem=m_ObjectDirectoryTree.InsertItem(WSTR("\\"));
	while(FindNextObject(FindData))
	{						
		GetObjectTypeName(FindData.ReturnObjectHandle,TypeName);
		GetObjectName(FindData.ReturnObjectHandle,ObjectName);
		if(IsDirectoryObjectType(FindData.ReturnObjectHandle))		
		{
			hChildItem = m_ObjectDirectoryTree.InsertItem(ObjectName,hItem);
			m_ObjectDirectoryTree.SetItemData(hChildItem,0,FindData.ReturnObjectHandle);
			m_ObjectDirectoryTree.InsertItem(WSTR(".."),hChildItem);
		}
		hListItem = m_ObjectList.InsertItem(ObjectName);
		m_ObjectList.SetItemData(hListItem,0,FindData.ReturnObjectHandle);
		m_ObjectList.SetItemText(hListItem,1,TypeName);		
	}	
#else
	{
		LoadAPI();
		bool bOK;
		FIND_OBJECT_DATA FindObjectData;
		HANDLE hItem,hSubItem;
		hItem=m_ObjectDirectoryTree.InsertItem(WSTR("\\"));
		bOK = FindFirstObject(L"\\",&FindObjectData);
		if(bOK)
		{
			while(bOK)
			{
				if(TStrICmp(FindObjectData.FindDataBuffer->DirectoryBaseInfo.ObjectTypeName.Buffer,"Directory")==0)
				{
					hSubItem = m_ObjectDirectoryTree.InsertItem(FindObjectData.FindDataBuffer->DirectoryBaseInfo.ObjectName.Buffer,hItem);
					m_ObjectDirectoryTree.InsertItem(WSTR(".."),hSubItem);
				}
				bOK = FindNextObject(&FindObjectData);
			}
			CloseFindObject(&FindObjectData);
		}
		/*
		CLocalFileIO FileIO;
		FILE_FIND_DATA FindData;
		WISP_CHAR ObjectName[MAX_PATH];
		bool bExit;
		
		hItem=m_ObjectDirectoryTree.InsertItem(WSTR("C:\\"));
		bExit = FileIO.FindFirstFile("c:\\",&FindData);	
		while(bExit)
		{
			TStrToStr(FindData.FileName,ObjectName);
			if(FindData.FileAttribute & FIO_FA_DIR)
			{
				hChildItem = m_ObjectDirectoryTree.InsertItem(ObjectName,hItem);
				m_ObjectDirectoryTree.InsertItem(WSTR(".."),hChildItem);
			}
			hListItem = m_ObjectList.InsertItem(ObjectName);
			m_ObjectList.SetItemText(hListItem,1,WSTR(""));
			if (!FileIO.FindNextFile(&FindData)) 
				bExit = false;
		}
		*/
	}
#endif
	return true;
}

bool CObjectDirectoryWnd::InitContext(DWORD ObjectHandle,HANDLE hItem)
{	
#ifdef CODE_OS_NT_DRV
	OBJFINDDATA FindData;
	WISP_CHAR TypeName[65],ObjectName[65];
	HANDLE hListItem,hChildItem;
	if(ObjectHandle==0)
		ObjectHandle = dwObjectRootDirectory;	
	FindData.ObjectHandle=ObjectHandle;
	FindData.NextObjectPointer=NULL;
	FindData.DirectoryItemIndex=0;
	if(hItem==NULL)
		hItem=m_ObjectDirectoryTree.InsertItem(WSTR("\\"));
	while(FindNextObject(FindData))
	{						
		GetObjectTypeName(FindData.ReturnObjectHandle,TypeName);
		GetObjectName(FindData.ReturnObjectHandle,ObjectName);
		if(IsDirectoryObjectType(FindData.ReturnObjectHandle))		
		{
			hChildItem = m_ObjectDirectoryTree.InsertItem(ObjectName,hItem);
			m_ObjectDirectoryTree.SetItemData(hChildItem,0,FindData.ReturnObjectHandle);
			m_ObjectDirectoryTree.InsertItem(WSTR(".."),hChildItem);
		}
		hListItem = m_ObjectList.InsertItem(ObjectName);
		m_ObjectList.SetItemData(hListItem,0,FindData.ReturnObjectHandle);
		m_ObjectList.SetItemText(hListItem,1,TypeName);		
	}	
#endif
	return true;
}
bool CObjectDirectoryWnd::InitContext(HANDLE hItem)
{
#ifdef CODE_OS_WIN
	bool bOK;
	WCHAR FullPathName[512];
	HANDLE hSubItem;
	FIND_OBJECT_DATA FindObjectData;
	WCHAR TargetName[512];
	UNICODE_STRING LinkTargetName;
	DWORD FullPathNameLen;
	ULONG ReturnLength;
	m_ObjectDirectoryTree.GetItemFullPath(hItem,0,FullPathName);
	FullPathNameLen=TStrLen(FullPathName);
	bOK = FindFirstObject(&FullPathName[1],&FindObjectData);
	if(bOK)
	{
		while(bOK)
		{
			if(TStrICmp(FindObjectData.FindDataBuffer->DirectoryBaseInfo.ObjectTypeName.Buffer,"Directory")==0)
			{
				hSubItem = m_ObjectDirectoryTree.InsertItem(FindObjectData.FindDataBuffer->DirectoryBaseInfo.ObjectName.Buffer,hItem);
				m_ObjectDirectoryTree.InsertItem(WSTR(".."),hSubItem);
			}
			hSubItem = m_ObjectList.InsertItem(FindObjectData.FindDataBuffer->DirectoryBaseInfo.ObjectName.Buffer);
			m_ObjectList.SetItemText(hSubItem,1,FindObjectData.FindDataBuffer->DirectoryBaseInfo.ObjectTypeName.Buffer);
			if(TStrICmp(FindObjectData.FindDataBuffer->DirectoryBaseInfo.ObjectTypeName.Buffer,"SymbolicLink")==0)
			{
				FullPathName[FullPathNameLen]=0;
				TStrCat(FullPathName,"\\");
				TStrCat(FullPathName,FindObjectData.FindDataBuffer->DirectoryBaseInfo.ObjectName.Buffer);
				LinkTargetName.Buffer=TargetName;
				LinkTargetName.MaximumLength=sizeof(TargetName);
				LinkTargetName.Length =sizeof(TargetName)-2;
				if(GetSymbolicLink(&FullPathName[1],&LinkTargetName,&ReturnLength))
				{
					m_ObjectList.SetItemText(hSubItem,2,LinkTargetName.Buffer);
				}
			}
			bOK = FindNextObject(&FindObjectData);
		}
		CloseFindObject(&FindObjectData);
	}
	m_ObjectList.Update();
#endif
	return true;
}
bool CObjectDirectoryWnd::ObjectList(HANDLE hItem)
{
#ifdef CODE_OS_WIN
	bool bOK;
	WCHAR FullPathName[512];
	HANDLE hSubItem;
	FIND_OBJECT_DATA FindObjectData;
	WCHAR TargetName[512];
	UNICODE_STRING LinkTargetName;
	DWORD FullPathNameLen;
	ULONG ReturnLength;
	m_ObjectDirectoryTree.GetItemFullPath(hItem,0,FullPathName);
	FullPathNameLen=TStrLen(FullPathName);
	bOK = FindFirstObject(&FullPathName[1],&FindObjectData);
	if(bOK)
	{
		while(bOK)
		{
			hSubItem = m_ObjectList.InsertItem(FindObjectData.FindDataBuffer->DirectoryBaseInfo.ObjectName.Buffer);
			m_ObjectList.SetItemText(hSubItem,1,FindObjectData.FindDataBuffer->DirectoryBaseInfo.ObjectTypeName.Buffer);
			if(TStrICmp(FindObjectData.FindDataBuffer->DirectoryBaseInfo.ObjectTypeName.Buffer,"SymbolicLink")==0)
			{
				FullPathName[FullPathNameLen]=0;
				TStrCat(FullPathName,"\\");
				TStrCat(FullPathName,FindObjectData.FindDataBuffer->DirectoryBaseInfo.ObjectName.Buffer);
				LinkTargetName.Buffer=TargetName;
				LinkTargetName.MaximumLength=sizeof(TargetName);
				LinkTargetName.Length =sizeof(TargetName)-2;
				if(GetSymbolicLink(&FullPathName[1],&LinkTargetName,&ReturnLength))
				{
					m_ObjectList.SetItemText(hSubItem,2,LinkTargetName.Buffer);
				}
			}
			bOK = FindNextObject(&FindObjectData);
		}
		CloseFindObject(&FindObjectData);
	}
	m_ObjectList.Update();
#endif //CODE_OS_WIN
	return true;
}