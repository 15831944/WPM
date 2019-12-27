/************************************************************************/
/* 
    Date : 10/15/2012
	File : ZBASE64.H
    11
       
	Version : 1.0
	Copyright
*/
/************************************************************************/  

#pragma once

#include <string>

using namespace std;

namespace Utilities{

class CeBase64
{
public:
	CeBase64();
	~CeBase64();

	/********************************************************* 
	* Function 	:	���������ݽ���base64����
	* Parameter	:	[in]pIn		��Ҫ���б��������
					[in]uInLen  ����������ֽ���
					[out]strOut ����Ľ���base64����֮����ַ���
	* Return	: 	bool
	* Author	:	Eadwin  (lidi@elex-tech.com)
	* Date		:	10/16/2012
	**********************************************************/ 
	bool static Encode(const unsigned char *pIn, unsigned long uInLen, string& strOut);

	/********************************************************* 
	* Function 	:	���������ݽ���base64����
	* Parameter	:	[in]pIn		��Ҫ���б��������
					[in]uInLen  ����������ֽ���
					[out]pOut		����Ľ���base64����֮����ַ���
					[out]strOut ����Ľ���base64����֮����ַ���
	* Return	: 	bool
	* Author	:	Eadwin  (lidi@elex-tech.com)
	* Date		:	10/16/2012
	**********************************************************/ 
	bool static Encode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen);
	
	/********************************************************* 
	* Function 	:	���������ݽ���base64����
	* Parameter	:	[in]strIn		��Ҫ���н��������
					[out]pOut		�������֮��Ľ�������
					[out]uOutLen	����Ľ���֮����ֽ�������
	* Return	: 	bool
	* Author	:	Eadwin  (lidi@elex-tech.com)
	* Date		:	10/16/2012
	**********************************************************/ 
	bool static Decode(const string& strIn, unsigned char *pOut, unsigned long *uOutLen);

	/********************************************************* 
	* Function 	:	���������ݽ���base64����
	* Parameter	:	[in]pIn			��Ҫ���н��������
					[in]uInLen		����������ֽ���
					[out]pOut		�������֮��Ľ�������
					[out]uOutLen	����Ľ���֮����ֽ�������
	* Return	: 	bool
	* Author	:	Eadwin  (lidi@elex-tech.com)
	* Date		:	10/16/2012
	**********************************************************/ 
	bool static Decode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen);
	
};

}