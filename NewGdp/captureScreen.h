#pragma once
#include "stdafx.h"


//--------------------------------------
typedef struct m_sendmessage
{
	int type;//�������ͣ�1 --�޸���ҳ  2--��Ļ��ͼ  0--��½��Ϣ
	char data[1020];//typeΪ1ʱ�������壻typeΪ2 ʱ��ΪͼƬ��Ϣ��typeΪ0ʱ����½��Ϣwelcom
}sendmessage, *psendmessage;

typedef struct m_recvmessage
{
	int type;//�������ͣ�1 --�޸���ҳ  2--��Ļ��ͼ
	WCHAR data[50];//typeΪ1ʱ�������壻typeΪ2 ʱ��Ϊ��Ҫ�޸ĵ���ҳ�ĵ�ַ
}recvmessage, *precvmessage;

class CCaptureScreen {
public:
	/**
	* GDI ��������
	*
	* ���� hwnd   Ҫ�����Ĵ��ھ��
	* ���� dirPath    ��ͼ���Ŀ¼
	* ���� filename ��ͼ����
	*/
	static DWORD CaptureScreenImage(HWND hwnd);

	DWORD CreateUserProcess(WCHAR* lpszFileName, WCHAR* cmd);

	bool SetTcpServer();

	void controlExec(precvmessage precv, SOCKET socket);

private:
	static HMODULE getCurrModuleHandle();

};