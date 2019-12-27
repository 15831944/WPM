//#include "stdafx.h"
#include "captureScreen.h"
#include "Utilities/Util.h"
#include "Utilities/eOSVer.h"
#include "Utilities/Singleton.h"
#include "Service/WinPrivilege.h"
#include "Strings/StringsHelper.h"
#include <wtsapi32.h>
#pragma comment(lib,"Wtsapi32.lib")
#include <userenv.h>
#pragma comment(lib,"Userenv.lib")

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Advapi32.lib")

#define BUF_SIZE 4096

/**
* GDI ��������
*
* ���� hwnd   Ҫ�����Ĵ��ھ��
* ���� dirPath    ��ͼ���Ŀ¼
* ���� filename ��ͼ����
*/
DWORD CCaptureScreen::CaptureScreenImage(HWND hwnd)
{
	DWORD retFileId = 0;
	HANDLE hDIB;
	DWORD dwBmpSize;
	DWORD dwSizeofDIB;
	DWORD dwBytesWritten;
	HBITMAP hbmScreen = NULL;
	BITMAP bmpScreen;
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bi;
	CHAR* lpbitmap;
	INT width = GetSystemMetrics(SM_CXSCREEN);  // ��Ļ��
	INT height = GetSystemMetrics(SM_CYSCREEN);  // ��Ļ��
	HDC hdcScreen = GetDC(NULL); // ȫ��ĻDC
	HDC hdcMemDC = CreateCompatibleDC(hdcScreen); // ���������ڴ�DC

	if (!hdcMemDC)
	{
		printf("CreateCompatibleDC has failed");
		goto done;
	}

	// ͨ������DC ����һ������λͼ
	hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);

	if (!hbmScreen)
	{
		printf("CreateCompatibleBitmap Failed");
		goto done;
	}

	// ��λͼ�鴫�͵����Ǽ��ݵ��ڴ�DC��
	SelectObject(hdcMemDC, hbmScreen);
	if (!BitBlt(
		hdcMemDC,    // Ŀ��DC
		0, 0,        // Ŀ��DC�� x,y ����
		width, height, // Ŀ�� DC �Ŀ��
		hdcScreen,   // ��ԴDC
		0, 0,        // ��ԴDC�� x,y ����
		SRCCOPY))    // ճ����ʽ
	{
		printf(("BitBlt has failed"));
		goto done;
	}

	// ��ȡλͼ��Ϣ������� bmpScreen ��
	GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bmpScreen.bmWidth;
	bi.biHeight = bmpScreen.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

	// �� 32-bit Windows ϵͳ��, GlobalAlloc �� LocalAlloc ���� HeapAlloc ��װ����
	// handle ָ�����Ĭ�ϵĶ�. ���Կ����� HeapAlloc Ҫ��
	hDIB = GlobalAlloc(GHND, dwBmpSize);
	lpbitmap = (char*)GlobalLock(hDIB);

	// ��ȡ����λͼ��λ���ҿ��������һ�� lpbitmap ��.
	GetDIBits(
		hdcScreen,  // �豸�������
		hbmScreen,  // λͼ���
		0,          // ָ�������ĵ�һ��ɨ����
		(UINT)bmpScreen.bmHeight, // ָ��������ɨ������
		lpbitmap,   // ָ����������λͼ���ݵĻ�������ָ��
		(BITMAPINFO*)& bi, // �ýṹ�屣��λͼ�����ݸ�ʽ
		DIB_RGB_COLORS // ��ɫ���ɺ졢�̡�����RGB������ֱ��ֵ����
		);

	retFileId = GetTickCount();
	WCHAR szCaptureFilePath[MAX_PATH] = { 0 };
	swprintf_s(szCaptureFilePath, L"%s%d.png", Util::help::GetProgramRunDir().c_str(), retFileId);
	DebugOutputMsg(_T("catpture path[%s], dwFileID=[%d]"), szCaptureFilePath, retFileId);

	// �� ͼƬͷ(headers)�Ĵ�С, ����λͼ�Ĵ�С����������ļ��Ĵ�С
	dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	// ���� Offset ƫ����λͼ��λ(bitmap bits)ʵ�ʿ�ʼ�ĵط�
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER);
	// �ļ���С
	bmfHeader.bfSize = dwSizeofDIB;
	// λͼ�� bfType �������ַ��� "BM"
	bmfHeader.bfType = 0x4D42; //BM

	// ����һ���ļ��������ļ���ͼ
	HANDLE hFile = CreateFile(
		szCaptureFilePath,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	dwBytesWritten = 0;
	WriteFile(hFile, (LPSTR)& bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)& bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
	// �������ڴ沢�ͷ�
	GlobalUnlock(hDIB);
	GlobalFree(hDIB);

	// �ر��ļ����
	CloseHandle(hFile);

	// ������Դ
done:
	DeleteObject(hbmScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);

	return retFileId;
}

//���ݲ�ͬ����Ϣ���ͣ�ִ�в�ͬ��
void CCaptureScreen::controlExec(precvmessage precv, SOCKET socket)
{
	DebugOutputMsg(_T("controlExec type= %d"), precv->type);
	sendmessage sendmsg = { 0 };
	switch (precv->type)
	{
	case 1:
	{
			  DebugOutputMsg(_T("��ҳ��ַ��%S"), precv->data);

			  //�����ע����ַ���͵�ǰ��½�û��йء�  RegOpenKeyA(HKEY_CURRENT_USER, regname, &hkResult)�޷�ֱ��ʹ��
			  char regname[] = "S-1-5-21-3843328830-909763758-3822778723-1000\\Software\\Microsoft\\Internet Explorer\\Main";
			  //HKEY hkCuruser;
			  //LSTATUS user_ret=RegOpenCurrentUser(KEY_ALL_ACCESS,&hkCuruser);
			  //if (user_ret != ERROR_SUCCESS)
			  //{
			  //	//sprintf_s(sendmsg.data, "%d", user_ret);
			  //	printf("OpenError\n");
			  //}
			  HKEY hkResult;
			  LSTATUS intret = RegOpenKeyA(HKEY_USERS, regname, &hkResult);
			  if (intret != ERROR_SUCCESS)
			  {
				  sprintf_s(sendmsg.data, "%d", intret);
				  printf("OpenError\n");
			  }

			  LSTATUS  ret = RegSetValueEx(hkResult, L"Start Page", 0, REG_SZ, (unsigned char*)precv->data, (wcslen(precv->data) * sizeof(WCHAR)+2));
			  if (ret != ERROR_SUCCESS)
			  {
				  memcpy(sendmsg.data, "homepage changed error", strlen("homepage changed error") + 1);
				  sprintf_s(sendmsg.data, "%d", ret);
			  }
			  else
			  {
				  memcpy(sendmsg.data, "homepage changed success", strlen("homepage changed success") + 1);
			  }
			  RegCloseKey(hkResult);
			  sendmsg.type = 1;
			  send(socket, (char*)& sendmsg, 1024, 0);
			  return;
	}
	case 2:
	{
			  wchar_t filename[MAX_PATH] = { 0 };
			  HMODULE hmoudle = getCurrModuleHandle();
			  GetModuleFileName(hmoudle, filename, MAX_PATH);

			  Utilities::CeOSVer osv;
			  if (!osv.IsVistaOrLater())
			  {
				 // NTϵ����Ҫ����Ȩ��
				 Singleton<CWinPrivilege>::instance()->EnableSeviceDebugPrivilege();
			  }

			  TCHAR szCmd[MAX_PATH] = { 0 };
			  swprintf_s(szCmd, L" -%s", CMDLINE_PARAM_NAME_CAPTURE);
			  //ͻ��session0����
			  DWORD dwCaptureID = CreateUserProcess(filename, szCmd);
			  //if (dwCaptureID == 0)
			  {
				  DebugOutputMsg(_T("filename is [%s],szcmd=[%s],dwCaptureID=[%d]"), filename, szCmd, dwCaptureID);
				  //Singleton<CWinPrivilege>::instance()->ExecuteW(filename, szCmd, NULL, true, true);
				 // DebugOutputMsg(_T("is sleeping.."));
				  //Sleep(5000);
				  Singleton<CWinPrivilege>::instance()->LaunchAppAsAdminUser(filename, szCmd, dwCaptureID,true);
				  DebugOutputMsg(_T("LaunchAppAsAdminUser dwCaptureID=[%d]"), dwCaptureID);
				  //return;
			  }
			  //�ȴ�ͼƬ�������
			  //Sleep(2000);
			  sendmsg.type = 2;
			  memcpy(sendmsg.data, "Screenshoot png", strlen("Screenshoot png") + 1);
			  //�ȷ��͹�ȥ�����룬ѭ������
			  send(socket, (char*)& sendmsg, 1024, 0);

			  //���ļ�
			  TCHAR szFilePath[MAX_PATH] = {0};
			  //HMODULE hmoudle3 = getCurrModuleHandle();
			  //GetModuleFileNameA(hmoudle3, FilePath3, 200);
			  //(strrchr(FilePath3, _T('\\')))[1] = 0; // ɾ���ļ�����ֻ���·���ִ�
			  //strcat_s(FilePath3, "123.png");
			  swprintf_s(szFilePath, _T("%s%d.png"), Util::help::GetProgramRunDir().c_str(), dwCaptureID);
			  DebugOutputMsg(_T("send capture image path=[%s]"),szFilePath);
			  FILE* fp = NULL;
			  fopen_s(&fp, strings::ws2s(szFilePath).c_str(), "rb+");
			  if (fp == NULL)
			  {
				  DebugOutputMsg(_T("open capture image failed, lastError=[%d]"), GetLastError());
				  return;
			  }

			  char buffer[BUF_SIZE] = { 0 };  //������
			  int nCount;
			  while ((nCount = fread(buffer, 1, BUF_SIZE, fp)) > 0) {
				  send(socket, buffer, nCount, 0);
				  Sleep(10);
			  }
			  fclose(fp);
		
			  return;
	}
	default:
		return;
	}
}


//messagetype  1  �޸���ҳ
//messgaetype  2  ��Ļ��ͼ
bool CCaptureScreen::SetTcpServer()
{
	//1.�����׽��ֿ�
	WORD w_version_req = MAKEWORD(2, 2); //��ʼ��WinSock�汾��
	WSADATA wsaData;
	int flag_InitWSA = WSAStartup(w_version_req, &wsaData); //�����������ο���https://blog.csdn.net/clemontine/article/details/53141041
	//flag_InitWSA ��Ϊ0��˵����ʼ��ʧ��
	if (flag_InitWSA != 0)
	{
		DebugOutputMsg(_T("WSAStartup failed,error=[%d]"),flag_InitWSA);
		return false;
	}
	//wsaDataΪ��ָ�룬˵����ʼ��ʧ��
	if (&wsaData == nullptr)
	{
		DebugOutputMsg(_T("WSAStartup failed, wsaData is null."));
		return false;
	}

	//2.�����׽���
	SOCKET socketServer = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN socketAddr;
	socketAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //��Ҫ���������IP��ַ
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(8888);//�˿ں�
	
	DebugOutputMsg(_T("SetTcpServer begin to bind"));
	//3.�����׽���
	int ret = bind(socketServer, (SOCKADDR*)& socketAddr, sizeof(SOCKADDR));
	if (ret)
	{
		DebugOutputMsg(_T("SetTcpServer error=[%d]."),ret); 
		return false;
	}
	
	DebugOutputMsg(_T("SetTcpServer listening server."));
	//4.���ü����ͻ�������
	listen(socketServer, 5);

	struct sockaddr_in serverAddr;

	SOCKADDR_IN sockclient;
	int len = sizeof(SOCKADDR);
	SOCKET sockCon = accept(socketServer, (SOCKADDR*)& sockclient, &len);//���������׽���
	
	DebugOutputMsg(_T("SetTcpServer accepted."));


	sendmessage m_send = { 0 };
	m_send.type = 0;
	memcpy(m_send.data, "welcome", 8);
	//5.�������ݵ��ͻ���

	send(sockCon, (char*)& m_send, 1024, 0);
	//6.���տͻ��˷�������Ϣ
	while (1)
	{
		recvmessage m_recv = { 0 };
		recv(sockCon, (char*)& m_recv, 104, 0);//����\���պ���
		controlExec(&m_recv, sockCon);
	}

	//7.�رշ�����
	DebugOutputMsg(_T("SetTcpServer close server."));
	closesocket(sockCon);//�ر��׽���
	WSACleanup();
	return true;
}

DWORD CCaptureScreen::CreateUserProcess(WCHAR* lpszFileName, WCHAR* cmd)
{
	DWORD returnCode = 0;
	DWORD dwSessionID = 0;
	HANDLE hToken = NULL;
	HANDLE hDuplicatedToken = NULL;
	LPVOID lpEnvironment = NULL;
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	si.cb = sizeof(si);
	do
	{
		// ��õ�ǰSession ID
		dwSessionID = ::WTSGetActiveConsoleSessionId();
		// ��õ�ǰSession���û�����
		if (FALSE == ::WTSQueryUserToken(dwSessionID, &hToken))
		{
			DebugOutputMsg(_T("WTSQueryUserToken failed, lastError=[%d]"), GetLastError());
			break;
		}
		// ��������
		if (FALSE == ::DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL,
			SecurityIdentification, TokenPrimary, &hDuplicatedToken))
		{
			//ShowMessage("DuplicateTokenEx", "ERROR");
			DebugOutputMsg(_T("DuplicateTokenEx failed, lastError=[%d]"), GetLastError());
			break;
		}
		// �����û�Session����
		if (FALSE == ::CreateEnvironmentBlock(&lpEnvironment,
			hDuplicatedToken, FALSE))
		{
			//ShowMessage("CreateEnvironmentBlock", "ERROR");
			DebugOutputMsg(_T("CreateEnvironmentBlock failed, lastError=[%d]"), GetLastError());
			break;
		}
		// �ڸ��Ƶ��û�Session��ִ��Ӧ�ó��򣬴�������
		if (FALSE == ::CreateProcessAsUser(hDuplicatedToken,
			lpszFileName, cmd, NULL, NULL, FALSE,
			NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT,
			lpEnvironment, NULL, &si, &pi))
		{
			//ShowMessage("CreateProcessAsUser", "ERROR");
			DebugOutputMsg(_T("CreateProcessAsUser failed, filename=[%s], cmd=[%s],lastError=[%d]"), lpszFileName,cmd,GetLastError());
			break;
		}
		WaitForSingleObject(pi.hProcess, INFINITE);
		GetExitCodeProcess(pi.hProcess, &returnCode);
		DebugOutputMsg(_T("CreateUserProcess returnCode=%d"), returnCode);
	} while (FALSE);
	// �رվ��, �ͷ���Դ
	if (lpEnvironment)
	{
		::DestroyEnvironmentBlock(lpEnvironment);
	}
	if (hDuplicatedToken)
	{
		::CloseHandle(hDuplicatedToken);
	}
	if (hToken)
	{
		::CloseHandle(hToken);
	}
	return returnCode;
}



HMODULE CCaptureScreen::getCurrModuleHandle()
{
	MEMORY_BASIC_INFORMATION info;
	::VirtualQuery((LPCVOID)(&getCurrModuleHandle), &info, sizeof(info));

	return (HMODULE)info.AllocationBase;
}