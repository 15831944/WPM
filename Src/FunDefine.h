#pragma once
#pragma warning(disable:4010)
//ÿ�����ܺ������ں�����ͷ�ͽ�β�����е���
//BeginFun�ں�����ͷ{֮��  
//EndFun�ں�����β Ret ֮ǰ
#define BeginFun 
//#define BeginFun   \
	__asm __emit 0x50 \
	__asm __emit 0x33 \
	__asm __emit 0xC0 \
	__asm __emit 0x33 \
	__asm __emit 0xC9 \
	__asm __emit 0x33 \
	__asm __emit 0xC1 \
	__asm __emit 0x33 \
	__asm __emit 0xD9 \
	__asm __emit 0x33 \
	__asm __emit 0xCB \
	__asm __emit 0x33 \
	__asm __emit 0xC0 \
	__asm __emit 0x33 \
	__asm __emit 0xC0 \
	__asm __emit 0x33 \
	__asm __emit 0xC0 \
	__asm __emit 0x33 \
	__asm __emit 0xC0 \
	__asm __emit 0x33 \
	__asm __emit 0xC0 \
	__asm __emit 0x33 \
	__asm __emit 0xC0 \
	__asm __emit 0x33 \
	__asm __emit 0xC0 \
	__asm __emit 0x74 \
	__asm __emit 0x04 \
	__asm __emit 0x90 \
	__asm __emit 0xE9 \
	__asm __emit 0x90 \
	__asm __emit 0xEB \
	__asm __emit 0x58 \

 #define EndFun
// #define EndFun   \
	__asm __emit 0x50 \
	__asm __emit 0x33 \
	__asm __emit 0xC0 \
	__asm __emit 0x33 \
	__asm __emit 0xC9 \
	__asm __emit 0x33 \
	__asm __emit 0xC1 \
	__asm __emit 0x33 \
	__asm __emit 0xD9 \
	__asm __emit 0x33 \
	__asm __emit 0xCB \
	__asm __emit 0x33 \
	__asm __emit 0xC0 \
	__asm __emit 0x00 \
	__asm __emit 0x33 \
	__asm __emit 0xC0 \
	__asm __emit 0x74 \
	__asm __emit 0x04 \
	__asm __emit 0x90 \
	__asm __emit 0xE9 \
	__asm __emit 0x90 \
	__asm __emit 0xEB \
	__asm __emit 0x58 \