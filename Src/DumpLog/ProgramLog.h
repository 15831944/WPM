#if !defined(__ISAFE_PROGRAMLOG_H__)
#define __ISAFE_PROGRAMLOG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

namespace Dump{namespace log{

	#ifdef __cplusplus
		extern "C"
		{
	#endif

			// ��������ͬInitProgramLog
			void DeleteProgramLog(LPCTSTR pProjectName=NULL,LPCTSTR pLogFilePath=NULL);

			// InitProgramLog�����Ĳ�������
			// DWORD dwMask�Ķ���
	#define LOG_MASK_ALL		0xFFFFFFFF

			// DWORD dwFlag�Ķ���
	#define LOG_DELETE_MASK		0x0000000F	// ������־�Ĵ�С
	#define LOG_DELETE_NO		0x00000000	// ���Զ�ɾ����־�ļ���dwMaxLogFileSize/dwMaxReserveSize�������κ�����
	#define LOG_DELETE_INIT		0x00000001	// �ڳ�ʼ����ʱ������ļ���С��ɾ����־�ļ�
	#define LOG_DELETE_CLOSE	0x00000002	// �ڹرյ�ʱ������ļ���С��ɾ����־�ļ�
	#define LOG_DELETE_WRITE	0x00000004	// ��д��־��ʱ������ļ���С��ɾ����־�ļ�

	#define LOG_RELEASE			0x80000000	// Release�汾Ҳ�����־��Ĭ��ֻ��Debug�汾�����־
	#define LOG_SAFE			0x40000000	// ��־�ļ��Ƿ����
	#define LOG_PROJECTNAME		0x20000000	// ��־�ļ��Ƿ���ʾ��Ŀ����
	#define LOG_THREADID		0x10000000	// ��ʾ�߳�ID
	#define LOG_TIME			0x08000000	// ��־�ļ��Ƿ���ʾʱ���

	#define FLAG_INIT			(LOG_DELETE_NO | LOG_PROJECTNAME | LOG_THREADID | LOG_TIME)

			#define __LOG_ENABLE__

	#ifdef __LOG_ENABLE__

			// DWORD dwMask ���룬������Щ��־��Ҫ����д��
			// DWORD dwFlag ��־λ
			// DWORD dwLogBufSize ��־�������Ĵ�С��0Ϊ��ʹ�û��壬-1Ϊʹ��Ĭ�ϻ����С(100K)������Ϊʵ�ʻ������Ĵ�С
			// DWORD dwMaxLogFileSize ��־�ļ������ߴ�(��λ��KB)��0�����Զ�ɾ����־�ļ�
			// DWORD dwMaxReserveSize ��־�ļ���������ߴ�(��λ��KB)��0����ȫ��ɾ����������
			// LPCTSTR pProjectName ��Ŀ���ƣ����ΪNULL��ȡexe�ļ���
			// LPCTSTR pLogFilePath ��־�ļ�·��������Ϊ����·�������·�������Ϊ���·����ǰ�����exe���ڵ�Ŀ¼�����ΪNULL����Ϊexe���ڵ�Ŀ¼+log\\+pProjectName.log

			void InitProgramLog(DWORD dwMask=LOG_MASK_ALL,DWORD dwFlag=FLAG_INIT,DWORD dwLogBufSize=-1,DWORD dwMaxLogFileSize=0,DWORD dwMaxReserveSize=0,LPCTSTR pProjectName=NULL,LPCTSTR pLogFilePath=NULL);
			void CloseProgramLog();

			void FlushProgramLog();	// �ѻ������־д���ļ�

			void WriteProgramLogString(DWORD dwMask,LPCTSTR lpszLogText);
			void WriteProgramLogBin(DWORD dwMask,LPCTSTR lpszFront,LPCTSTR lpszBack,LPCTSTR lpszBuf,DWORD uBufLength);
			void WriteProgramLog(DWORD dwMask,LPCTSTR lpszFormat,...);
			void WriteProgramLogNoMask(LPCTSTR lpszFormat,...);

	#define WriteProgramLogStringNoMask(lpszLogText)						ISAFE_WriteProgramLogString(ISAFE_LOG_MASK_ALL,lpszLogText)
	#define WriteProgramLogBinNoMask(lpszFront,lpszBack,lpszBuf,nBufLength)	ISAFE_WriteProgramLogBin(ISAFE_LOG_MASK_ALL,lpszFront,lpszBack,lpszBuf,nBufLength)

	#else	// __LOG_ENABLE__

	#define InitProgramLog														__noop
	#define CloseProgramLog()													__noop
	#define FlushProgramLog()													__noop
	#define WriteProgramLogString(dwMask,lpszLogText)							__noop
	#define WriteProgramLogBin(dwMask,lpszFront,lpszBack,lpszBuf,nBufLength)	__noop
	#define WriteProgramLog														__noop
	#define WriteProgramLogNoMask												__noop

	#define WriteProgramLogStringNoMask(lpszLogText)							__noop
	#define WriteProgramLogBinNoMask(lpszFront,lpszBack,lpszBuf,nBufLength)		__noop

	#endif	// __LOG_ENABLE__

	#ifdef __cplusplus
		}
	#endif

}}


	
#endif // !defined(__ISAFE_PROGRAMLOG_H__)
