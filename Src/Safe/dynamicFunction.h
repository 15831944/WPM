#pragma once

//Dll�б�
/*
USERENV.dll
{
   CreateEnvironmentBlock
   DestroyEnvironmentBlock
}

PSAPI.dll
{
  EnumProcesses
  GetModuleFileNameExW
  EnumProcessModules
}

SHELL32.dll
{
 ShellExecuteExW
 SHGetFolderPathW
 SHChangeNotify

}



*/

//��ʼ�����غ���
void InitFunctionLoad();



//������������
typedef BOOL  (WINAPI *TypeCreateEnvironmentBlock)(
	_Out_     LPVOID *lpEnvironment,
	_In_opt_  HANDLE hToken,
	_In_      BOOL bInherit
	);

typedef BOOL (WINAPI *TypeDestroyEnvironmentBlock)(
	_In_  LPVOID lpEnvironment
	);





//��չ����

extern TypeCreateEnvironmentBlock g_CreateEnvironmentBlock;
extern TypeDestroyEnvironmentBlock g_DestroyEnvironmentBlock;


