#pragma once

#include <Windows.h>
#include <stdint.h>

#include "evil.h"

static HMODULE hHxD = NULL;
static bool IsSupported = false;

static unsigned int crc32b(unsigned char* message, int len) {
	unsigned int byte, crc, mask;
	crc = 0xFFFFFFFF;
	for (int i = 0; i < len; i++) {
		byte = message[i];
		crc = crc ^ byte;
		for (int j = 7; j >= 0; j--) {
			int bit = crc & 1;
			crc >>= 1;
			if (bit) {
				crc ^= 0xEDB88320;
			}
		}
	}
	return ~crc;
}

#include <stdio.h>
int SetupEvil()
{
	hHxD = GetModuleHandle(NULL);
	if (!hHxD)
	{
		return 1;
	}

	// Check the correct version of HxD by checksumming PE header (incl. compiler timestamp)
	uint8_t peHeader[0x100];
	memcpy(peHeader, (uint8_t*) ((uintptr_t)hHxD + 0x100), sizeof(peHeader));
	uint32_t crc = crc32b(peHeader, sizeof(peHeader));
	if (crc != 0x9C7EC199 && crc != 0x2F206EB9) { // HxD Version 2.4.0.0 (x86-64)
		return 2;
	}

	// OK
	IsSupported = true;
	return 0;
}

uint64_t CurSelectionSize()
{
	/*
	Look for ascii "FDesignSize", there is a vtable above it.
	The last function in the vtable is the one we want to look at.
	__int64 __fastcall wow_bigly(__int64 a1, char a2, __int64 a3, int a4)
	{
	  _QWORD _0[9]; // [rsp+0h] [rbp+0h] BYREF
	  __int64 v6; // [rsp+50h] [rbp+50h]

	  _0[5] = _0;
	  v6 = a1;
	  if ( a2 )
		v6 = sub_40C0F0(a1, a2);
	  sub_4FCFB0(v6, 0i64, a3);
	  if ( qword_92A930 && a4 >= 0 )
		qword_92A930(this_is_the_base_ptr, v6); // <-- this_is_the_base_ptr
	  if ( a2 )
		v6 = sub_40C130(v6);
	  return v6;
	}
	*/

	// ["HxD.exe" + 0052A938] = pointer to globalObj
	// [globalObj + b8] = pointer to curFile
	// [curFile + 8] = pointer to editorStruct
	// [editorStruct + 798] = selection size
	void* globalObj = *(void**)((uintptr_t) hHxD + 0x0052A938);
	if (!globalObj)
	{
		// WTF? This shouldn't happen
		return 0;
	}
	void* curFile = *(void**)((uintptr_t)globalObj + 0xb8);
	if (!curFile)
	{
		// No file open
		return 0;
	}
	void* editorStruct = *(void**)((uintptr_t)curFile + 0x8);
	uint64_t selectionSize = *(uint64_t*)((uintptr_t)editorStruct + 0x798);
	return selectionSize;
}

int ClampDataSize(int ByteCount)
{
	if (!IsSupported)
	{
		return ByteCount > 0x1000 ? 0x1000 : ByteCount;
	}
	else
	{
		uint64_t size = CurSelectionSize();
		return size > 0x1000 ? 0x1000 : size;
	}
}
