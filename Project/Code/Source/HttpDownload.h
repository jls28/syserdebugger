// HttpDownload.h: interface for the CHttpDownload class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _HTTPDOWNLOAD_H_
#define _HTTPDOWNLOAD_H_

#include <time.h>

//��Ϣ���
const long MSG_DOWNLOAD_STATUS = 101; //����״̬
const long MSG_FILE_TOTAL_SIZE = 102; //Ҫ���ص��ļ��ܴ�С
const long MSG_DOWNLOADED_SIZE = 103; //�����ص��ļ���С

//����״̬��Ϣ���
const long STATUS_RESOLVE_HOST  = 201; //��������
const long STATUS_CONNECT_HOST  = 202; //��������
const long STATUS_SEND_REQUEST  = 203; //��������
const long STATUS_RECV_RESPONSE = 204; //�ȴ�Ӧ��
const long STATUS_HOST_REDIRECT = 205; //�ض�������
const long STATUS_DOWNLOAD_FILE = 206; //��ʼ�����ļ�
const long STATUS_DOWN_FINISHED = 207; //�������

//������Ϣ���
const int ERROR_DOWNLOAD_NONE = 300; //û�д���
const int ERROR_RESOLVE_HOST  = 301; //��������ʧ��
const int ERROR_CONNECT_HOST  = 302; //��������ʧ��
const int ERROR_SEND_REQUEST  = 303; //��������ʧ��
const int ERROR_RECV_RESPONSE = 304; //�ȴ�Ӧ�����
const int ERROR_RECEIVE_FILE  = 305; //�����ļ�ʱ����
const int ERROR_STOP_DOWNLOAD = 306; //�û���ֹ����

typedef bool (* DownloadCallbackProc)(long wParam, long lParam);
///////////////////////////////////////////////////////////////////////////////////////////////

class CHttpDownload  
{
public:
	CHttpDownload();
	virtual ~CHttpDownload();

	static bool StartupWinsock();
	static void CleanupWinsock();

	void SetCalllbackProc(DownloadCallbackProc proc);
	void SetProxy(const char *lpszProxyServer, unsigned short nProxyPort);
	void SetProxy(const char *lpszProxyServer, unsigned short nProxyPort, bool bProxyAuthorization, const char *lpszProxyUser, char *lpszProxyPwd);
	int	 DownloadToBuffer(const char *lpszDownloadURL, char *Buffer,int BufLen,int* ReturnedLength);
	int  Download(const char *lpszDownloadURL,const char *lpszSaveFileName);
	void StopDownload();

	bool SendFinishRequest(const char *lpszFinishURL);

	int GetErrorCode();

private:
	bool CreateSocket();
	void CloseSocket();
	
	int  SendRequest();
	bool GetHostIP(char *strServer);

  char *URLEncode(const char *lpszURL, char *szURL);
	bool ParseURL(const char *lpszURL,char *strServer,char *strObject);
	int  GetInformation(const char *lpszReceived,long &nHeaderLength,long &nContentLength,time_t &TimeLastModified);
	void ConvertTime(const char *lpszTime,time_t &TimeLastModified);

	void StringMakeLower(char *str);

private:
	unsigned int m_hSocket;

	char m_strDownURL[4096];
	char m_strServer[128];

	char m_strSaveFileName[260];

	long  m_nHeaderLength;
	long  m_nContentLength;
	long  m_nFileTotalSize;
	time_t m_TimeLastModified;

	bool m_bStopDownload;
	int  m_nErrorCode;

	//�ص�����
	DownloadCallbackProc m_lpCallbackProc;

	//ʹ�ô���
	bool m_bProxy;
	char m_strProxyServer[128];
	unsigned short m_nProxyPort;

	bool m_bProxyAuthorization;
	char m_szProxyUser[128];
	char m_szProxyPwd[128];
};

#endif //_HTTPDOWNLOAD_H_
//#define IBM
