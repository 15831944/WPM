#include "stdafx.h"
#include "ServiceInstaller.h"
#include "Service/APIService.h"
#include "Service/WPMService.h"
#include "Configure.h"
#include "Service/WinPrivilege.h"
#include "Utilities/Singleton.h"
#include "Utilities/eOSVer.h"
#include <ShlObj.h>

#include <process.h>


#define MAX_PATHEX (MAX_PATH * 2)


SERVICEDATA	g_ServiceData =
{
	SVC_NAME,
	SVC_DISPLAY,
	SERVICE_WIN32_OWN_PROCESS,
	SERVICE_AUTO_START,
	SERVICE_ERROR_NORMAL,
	_T(""),
	SVC_ORDERGROUP_2,
	0,
	_T(""),
	SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN,
	SVC_DESCRIPTION
}; // ������������

//////////////////////////////////////////////////////////////////////////
CServiceInstaller::CServiceInstaller(WPMService* pService)
	: m_pService(pService)
{
	wchar_t strAppPath[MAX_PATHEX] = {0};
	wchar_t strTargetPath[MAX_PATHEX] = {0};
	std::wstring AppdirLog;

	::SHGetFolderPathW(NULL, CSIDL_COMMON_APPDATA , NULL, SHGFP_TYPE_DEFAULT, strTargetPath);
	m_strAppDir = strTargetPath;
	m_strAppDir += L"\\";
	m_strAppDir += SVC_MODULE_DIR;
	CreateDirectory(m_strAppDir.c_str(), NULL);
	m_strAppDir += L"\\";

	::GetModuleFileNameW(NULL,strAppPath,MAX_PATHEX);
	m_strCurModule = strAppPath;
	DebugOutputMsg(_T("m_strAppDir path =[%s]"), m_strAppDir.c_str());
	//AppdirLog = m_strAppDir + L"wprotectmanager.log";
	//
	// DWORD dwMask ���룬������Щ��־��Ҫ����д��
	// DWORD dwFlag ��־λ
	// DWORD dwLogBufSize ��־�������Ĵ�С��0Ϊ��ʹ�û��壬-1Ϊʹ��Ĭ�ϻ����С(100K)������Ϊʵ�ʻ������Ĵ�С
	// DWORD dwMaxLogFileSize ��־�ļ������ߴ�(��λ��KB)��0�����Զ�ɾ����־�ļ�
	// DWORD dwMaxReserveSize ��־�ļ���������ߴ�(��λ��KB)��0����ȫ��ɾ����������
	// LPCTSTR pProjectName ��Ŀ���ƣ����ΪNULL��ȡexe�ļ���
	// LPCTSTR pLogFilePath ��־�ļ�·��������Ϊ����·�������·�������Ϊ���·����ǰ�����exe���ڵ�Ŀ¼�����ΪNULL����Ϊexe���ڵ�Ŀ¼+log\\+pProjectName.log

	
	//    Dump::log::InitProgramLog(LOG_MASK_ALL,FLAG_INIT | LOG_RELEASE | LOG_SAFE , 0, 0, 0, NULL, AppdirLog.c_str());

	//Dump::log::InitProgramLog(LOG_MASK_ALL,FLAG_INIT | LOG_RELEASE , 0, 0, 0, L"ProtectManager", Appdir.c_str());
}

CServiceInstaller::~CServiceInstaller(void)
{

}

bool CServiceInstaller::WriteUninstallInfo()
{
	//BeginFun

	bool bRet=false;
	DWORD dwDesired = 0;
	DWORD dwType = REG_SZ;
	HKEY hkey = NULL;
	DWORD dwDisposition = 0;
	wchar_t strPath[MAX_PATHEX] = {0};
	std::wstring szSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\";
	szSubKey += REG_UNINSTALL_NAME;

	::GetModuleFileNameW(NULL,strPath,MAX_PATHEX);
	std::wstring strDisIcon = m_strAppDir + SVC_MODULENAME;
	std::wstring strUninstString = strDisIcon;
	strUninstString += L" -";
	strUninstString += CMDLINE_PARAM_NAME_UNINSTALL;

	std::wstring strVersion = Singleton<CConfigure>::instance()->m_strVer;
	std::wstring strDisName = SVC_PRODUCT_NAME;
	strDisName += strVersion;
	std::wstring strPublisher = REG_UNINSTALL_PUBLISHER;


	if (Singleton<CConfigure>::instance()->m_bIsWow64)
	{
		dwDesired |= KEY_WOW64_32KEY;
	}

	do 
	{
		if (::RegCreateKeyExW(HKEY_LOCAL_MACHINE,szSubKey.c_str(),0,NULL,REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS|dwDesired,NULL,&hkey,&dwDisposition) !=ERROR_SUCCESS)
			break;

		RegSetValueExW(hkey, L"DisplayName", 0, REG_SZ, (const BYTE*)(strDisName).c_str(), sizeof(wchar_t)*(strDisName.length()+1));
		RegSetValueExW(hkey, L"DisplayVersion", 0, REG_SZ, (const BYTE*)(strVersion).c_str(), 2*wcslen(strVersion.c_str()));
		RegSetValueExW(hkey, L"publisher", 0, REG_SZ, (const BYTE*)(strPublisher).c_str(), 2*wcslen(strPublisher.c_str()));
		RegSetValueExW(hkey, L"UninstallString", 0, REG_SZ, (const BYTE*)(strUninstString).c_str(), 2*wcslen(strUninstString.c_str()));
		RegSetValueExW(hkey, L"DisplayIcon", 0, REG_SZ, (const BYTE*)strDisIcon.c_str(), 2*wcslen(strDisIcon.c_str()));

		bRet = true;

	} while (false);

	if(hkey!=NULL)
		RegCloseKey(hkey);

//EndFun
	return bRet;

}


bool CServiceInstaller::ClearRegInfo()
{
	//BeginFun
	
	bool bRet=false;
	DWORD dwDesired = 0;
	HKEY hkey = NULL;
	//std::wstring szSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";

	if (Singleton<CConfigure>::instance()->m_bIsWow64)
	{
		dwDesired |= KEY_WOW64_32KEY;
	}

	do 
	{
		if(RegOpenKeyExW(HKEY_LOCAL_MACHINE,L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",0,KEY_READ|KEY_WRITE|dwDesired,&hkey)!=ERROR_SUCCESS)
			break;

		::SHDeleteKey(hkey, REG_UNINSTALL_NAME);

		RegCloseKey(hkey);

		if(RegOpenKeyExW(HKEY_LOCAL_MACHINE,L"SOFTWARE",0,KEY_READ|KEY_WRITE|dwDesired,&hkey)!=ERROR_SUCCESS)
			break;

		::SHDeleteKey(hkey, REG_SECONDCONTROL_NAME);

		bRet = true;

	} while (false);

	if(hkey!=NULL)
		RegCloseKey(hkey);

//EndFun

	return bRet;


}


void CServiceInstaller::PreSelfInstall(const std::wstring& strCmdLine)
{
	//BeginFun
	
	std::wstring strTarget = m_strAppDir + SVC_MODULENAME;
	std::wstring strSrc = m_strCurModule;

	if(!PathFileExists(strTarget.c_str()))
	{
		CopyFile(strSrc.c_str(),strTarget.c_str(), FALSE);
	}


	Utilities::CeOSVer osv;
	if(!osv.IsVistaOrLater())
	{
		// NTϵ����Ҫ����Ȩ��
		Singleton<CWinPrivilege>::instance()->EnableSeviceDebugPrivilege();
	}
	//strCmdLine;
	std::wstring strCmd =  L" -";
	strCmd += CMDLINE_PARAM_NAME_RUN;
	Singleton<CWinPrivilege>::instance()->ExecuteW(strTarget.c_str(),strCmd.c_str(),NULL,true,false);

	//EndFun
}

void CServiceInstaller::SelfUninstall()
{

	std::wstring strTarget = m_strAppDir + SVC_MODULENAME;
	if(PathFileExists(strTarget.c_str()))
	{
		//Create the service object
		APIService::UninstAndStopService();
	}

	Sleep(2000);
	//send uninstall report

	//clear uninstall info
	ClearRegInfo();


	MoveFileExW(strTarget.c_str(),L"",MOVEFILE_DELAY_UNTIL_REBOOT);
	MoveFileExW(m_strAppDir.c_str(),L"",MOVEFILE_DELAY_UNTIL_REBOOT);

	Singleton<CWinPrivilege>::instance()->DelSelf();

	//EndFun
}
