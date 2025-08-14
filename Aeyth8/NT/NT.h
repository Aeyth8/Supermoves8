#pragma once

typedef long NTSTATUS;
typedef wchar_t* PWSTR;
typedef const wchar_t* PCWSTR;
typedef unsigned long* PULONG;
typedef unsigned short USHORT;

struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
};

typedef struct _UNICODE_STRING UNICODE_STRING;
typedef const UNICODE_STRING* PCUNICODE_STRING;
typedef void* PVOID;