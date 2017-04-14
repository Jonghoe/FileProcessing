#include <stdio.h>
#include <stdlib.h> 
#include<Windows.h>
#include<time.h>

typedef enum TYPE { COPY, REMOVE, DEFAULT } funcType;

// mybackup.log 파일
FILE* LOG;
int srcLen,dstLen;
BOOL _Body(const wchar_t *sDir, const wchar_t *dDir, funcType t);

void CharToWChar(const char* src, wchar_t* dst)
{
	size_t len = strlen(src) + 1;
	size_t convertedChars = 0;
	// char -> wchar_t로 타입캐스팅
	mbstowcs_s(&convertedChars, dst, len, src, _TRUNCATE);
}

void _PrintLog(const wchar_t* log)
{
	time_t tm = time(0);
	struct tm* now = localtime(&tm);
	fprintf(LOG, "time: %4d.%2d.%2d || %2dh %2dm\t", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min);
	fwprintf(LOG, log);
	fprintf(LOG, "\n");
}

void _FileCopy(const WIN32_FIND_DATA* file,const wchar_t* sPath,const wchar_t* dPath)
{
	CopyFile(sPath, dPath, FALSE);
	
	wchar_t wLog[1028];	
	wsprintf(wLog, L"src%s is copied.\t", sPath+srcLen);		
	_PrintLog(wLog);
}

void _CreateDirectory(const wchar_t* dPath,const WIN32_FIND_DATA* file)
{
	CreateDirectory(dPath, NULL);
	wchar_t wLog[1028];
	wsprintf(wLog, L"DIR src%s is created.\t", dPath+srcLen);
	_PrintLog(wLog);	
}

void _RemoveFile(WIN32_FIND_DATA* file, wchar_t* src)
{
	if(file->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		RemoveDirectory(src);
	}
	else {
		_wremove(src);
	}
	wchar_t wLog[1028];
	wsprintf(wLog, L"dst%s is removed.\t", src+dstLen);	
	_PrintLog(wLog);
}

BOOL _FindFile(WIN32_FIND_DATA* src, const WIN32_FIND_DATA* dst, const wchar_t* path) 
{		
	HANDLE hFile = FindFirstFile(path, src);
	while (lstrcmpW(src->cFileName, dst->cFileName) != 0 && FindNextFile(hFile, src)==TRUE);
	
	if (hFile != NULL) {
		FindClose(hFile);
	}
	
	return lstrcmpW(src->cFileName,dst->cFileName)==0;
}

void _Copy_Body(WIN32_FIND_DATA* dFile, WIN32_FIND_DATA* sFile, wchar_t* dst, wchar_t* src)
{
	// 디렉토리 인경우 디렉토리를 생성하고 이동하여 생성
	if (sFile->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {			
		_CreateDirectory(dst, NULL);			
		_Body(src, dst, COPY);
	}
	//파일인 경우 복사
	else {
		_FileCopy(sFile, src, dst);
	}
}

void _Remove_Body(WIN32_FIND_DATA* dFile, WIN32_FIND_DATA* sFile, wchar_t* dst, wchar_t* src)
{
	//디렉토리인 경우
	if (sFile->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		// 하위 디렉토리가 존재하는 경우 하위 디렉토리로 이동
		if (_FindFile(dFile, sFile, dst)) {
			_Body(src, dst, REMOVE);
		}
		else {
			_Body(src, dst, REMOVE);
			_RemoveFile(sFile, src);
		}		
	}
	//파일인 경우
	else {		
		// 파일이 존재하지 않는 경우 삭제
		if (!_FindFile(dFile, sFile, dst)&&lstrcmp(sFile->cFileName,L"mybackup.log")!=0) {
			_RemoveFile(sFile, src);
		}
	}
}

void _Default_Body(WIN32_FIND_DATA* dFile, WIN32_FIND_DATA* sFile, wchar_t* dst, wchar_t* src)
{
	//디렉토리인 경우
	if (sFile->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		// 하위 디렉토리가 존재하지 않는 경우 전체 파일을 복사한다.
		if (!_FindFile(dFile, sFile, dst)) {
			_CreateDirectory(dst, sFile);
			_Body(src, dst, COPY);
		}
		else {	
			_Body(src, dst, DEFAULT);
		}
	}
	//파일인 경우
	else {
		BOOL isUpdated = FALSE;
		// 파일이 dst에 있는지 체크
		// 복사할 파일이 존재하면 업데이트가 최신인지 확인
		if (_FindFile(dFile, sFile, dst)) {
			isUpdated = dFile->ftLastWriteTime.dwHighDateTime == sFile->ftLastWriteTime.dwHighDateTime;
		}
		// 업데이트 되지 않은 파일인 경우 업데이트한다.
		if (!isUpdated) {
			_FileCopy(sFile, src, dst);
		}
	}
}

BOOL _Body(const wchar_t *sDir, const wchar_t *dDir,funcType t)
{
	WIN32_FIND_DATA fdFile;
	WIN32_FIND_DATA dFile;

	HANDLE hSrc = NULL;
	HANDLE hDstFirst = NULL;
	wchar_t sPath[2048];
	wchar_t dPath[2048];

	//Specify a file mask. *.* = We want everything! 
	wsprintf(sPath, L"%s\\*.*", sDir);
	wsprintf(dPath, L"%s\\*.*", dDir);

	// src와 dst path 체크
	if ((hSrc = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE) {
		fwprintf(LOG,L"SRC Path not found: [%s]\n", sDir);
		return FALSE;
	}
	// dst의 경우 원본 파일외의 파일을 삭제할때 src에는 없는 path이므로 확인 하지 않는다.
	if (t!=REMOVE && (hDstFirst = FindFirstFile(dPath, &fdFile)) == INVALID_HANDLE_VALUE) {
		fwprintf(LOG,L"DST Path not found: [%s]\n", dDir);
		return FALSE;
	}

	do {
		// ".", ".." 무시
		if (wcscmp(fdFile.cFileName, L".") != 0
			&& wcscmp(fdFile.cFileName, L"..") != 0) {
			wsprintf(sPath, L"%s\\%s", sDir, fdFile.cFileName);
			wsprintf(dPath, L"%s\\%s", dDir, fdFile.cFileName);
			// 삭제, 복사, 기본 으로 돌아간다.
			if (t == REMOVE) {
				_Remove_Body(&dFile, &fdFile, dPath, sPath);
			}
			else if (t == COPY) {
				_Copy_Body(&dFile, &fdFile, dPath, sPath);
			}
			else {
				_Default_Body(&dFile, &fdFile, dPath, sPath);
			}
		}
	} while (FindNextFile(hSrc, &fdFile));

	// 핸들 해제
	if (hSrc != NULL)
		FindClose(hSrc);
	if (hDstFirst != NULL)
		FindClose(hDstFirst);
	return TRUE;
}


BOOL _RemoveNeedless(const wchar_t* sDir,const wchar_t* dDir)
{
	return _Body(sDir, dDir, REMOVE);
}

// 파일 경로 설정 입력받은 char -> wchar_t로 변환
void InitData(const char* src, const char* dst, wchar_t* wSrc, wchar_t* wDst)
{
	char LOGPath[2048];
	sprintf(LOGPath, "%s\\%s", dst, "mybackup.log");
	LOG = fopen(LOGPath, "a");
	CharToWChar(src, wSrc);
	CharToWChar(dst, wDst);
	srcLen = lstrlen(wSrc);
	dstLen = lstrlen(wDst);
	_PrintLog(L"================BackUp started================\t");
}

BOOL MyBackUp(const wchar_t* sDir, const wchar_t* dDir)
{
	_RemoveNeedless(dDir, sDir);
	return _Body(sDir, dDir, DEFAULT);
}

void ReleaseData()
{
	_PrintLog(L"================BackUp finished===============\t\n\n");
	fclose(LOG);
}

int main(int argc,char* argv[]) {
	wchar_t sDir[2048];
	wchar_t dDir[2048];
	
	InitData(argv[1], argv[2], sDir, dDir);
	
	MyBackUp(sDir, dDir);

	ReleaseData();
}
