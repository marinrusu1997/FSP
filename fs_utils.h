#ifndef FILESYSTEM_UTILS_HEADER
#define FILESYSTEM_UTILS_HEADER

#include <string>
#include <filesystem>

#ifdef LINUX
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#else
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include "accctrl.h"
#include "aclapi.h"
#include <locale>
#include <codecvt>
#include <Lmcons.h>
#pragma comment(lib, "advapi32.lib")
#endif


#define UTF8_UTF16_CONVERTER std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
#define W2A(w,a) \
	a = converter.to_bytes(w); 
#define A2W(a,w) \
	w = converter.from_bytes(a);

bool GetFileOwner(const std::string& Filename, std::string& Owner)
{
#ifdef LINUX
	struct stat FileInfo;
	stat(Filename.c_str(), &FileInfo);
	struct passwd *UserDatabaseEntry = getpwuid(FileInfo.st_uid);
	struct group  *GroupDatabaseEntry = getgrgid(FileInfo.st_gid);
	//
	if (UserDatabaseEntry != 0)
	{
		Owner = UserDatabaseEntry->pw_name;
		return true;
	}
	else
	{
		return false;
	}
#else
	DWORD dwRtnCode = 0;
	PSID pSidOwner = NULL;
	BOOL bRtnBool = TRUE;
	LPTSTR AcctName = NULL;
	LPTSTR DomainName = NULL;
	DWORD dwAcctName = 1, dwDomainName = 1;
	SID_NAME_USE eUse = SidTypeUnknown;
	HANDLE hFile;
	PSECURITY_DESCRIPTOR pSD = NULL;
	UTF8_UTF16_CONVERTER
		bool succes = false;

	{
		std::wstring Filename_w; //make temporary wide string
		A2W(Filename, Filename_w)

			// Get the handle of the file object.
			hFile = CreateFile(
				Filename_w.data(),
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
				NULL);
	}

	// Check GetLastError for CreateFile error code.
	if (hFile == INVALID_HANDLE_VALUE)
		goto err_section;

	// Get the owner SID of the file.
	dwRtnCode = GetSecurityInfo(
		hFile,
		SE_FILE_OBJECT,
		OWNER_SECURITY_INFORMATION,
		&pSidOwner,
		NULL,
		NULL,
		NULL,
		&pSD);

	// Check GetLastError for GetSecurityInfo error condition.
	if (dwRtnCode != ERROR_SUCCESS)
		goto err_section;

	// First call to LookupAccountSid to get the buffer sizes.
	bRtnBool = LookupAccountSid(
		NULL,           // local computer
		pSidOwner,
		NULL,
		(LPDWORD)&dwAcctName,
		NULL,
		(LPDWORD)&dwDomainName,
		&eUse);

	// Reallocate memory for the buffers.
	AcctName = (LPTSTR)GlobalAlloc(
		GMEM_FIXED,
		dwAcctName * sizeof(wchar_t));

	// Check GetLastError for GlobalAlloc error condition.
	if (AcctName == NULL)
		goto err_section;

	DomainName = (LPTSTR)GlobalAlloc(
		GMEM_FIXED,
		dwDomainName * sizeof(wchar_t));

	// Check GetLastError for GlobalAlloc error condition.
	if (DomainName == NULL)
		goto err_section;

	// Second call to LookupAccountSid to get the account name.
	bRtnBool = LookupAccountSid(
		NULL,                   // name of local or remote computer
		pSidOwner,              // security identifier
		AcctName,               // account name buffer
		(LPDWORD)&dwAcctName,   // size of account name buffer 
		DomainName,             // domain name
		(LPDWORD)&dwDomainName, // size of domain name buffer
		&eUse);                 // SID type

  // Check GetLastError for LookupAccountSid error condition.
	if (bRtnBool == FALSE)
		goto err_section;
	else if (bRtnBool == TRUE) {
		W2A(std::wstring(AcctName), Owner)
			succes = true;
	}

err_section:
	// Clean up resources
	CloseHandle(hFile);
	GlobalFree(AcctName);
	GlobalFree(DomainName);
	return succes;
#endif
}

void GetCurrentUserName(std::string &user_name)
{
#ifdef LINUX
	uid_t uid = geteuid();
	struct passwd *pw = getpwuid(uid);
	if (pw)
	{
		user_name = pw->pw_name;
	}
	user_name = "";
#else
	TCHAR username[UNLEN + 1];
	DWORD size = UNLEN + 1;
	GetUserName((TCHAR*)username, &size);

	UTF8_UTF16_CONVERTER
	W2A(std::wstring(username),user_name)
#endif // LINUX	
}

_NODISCARD bool IsCurrentUserOwner(const std::string& Filename)
{
	std::string Owner, CurrentUser;
	GetFileOwner(Filename, Owner);
	GetCurrentUserName(CurrentUser);

	return Owner == CurrentUser;
}


#endif //FILESYSTEM_UTILS_HEADER