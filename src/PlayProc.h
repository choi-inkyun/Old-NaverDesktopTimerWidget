#pragma once

#include "mmsystem.h"

class CPlayProc
{
public:
	CPlayProc(void);
	~CPlayProc(void);

	BOOL PlaySelectedFile();
	BOOL Stop();
	BOOL Pause();
	BOOL SelectFile(LPCTSTR szPath);
	LONG GetPlayTime(){ return m_nPlayTime; }
	BOOL IsPlaying(){ return m_bPlay; }
	void Seek(long nPos);

	LPCTSTR GetSongTitle(){ return m_szSongTitle; }
	LPCTSTR GetArtist(){ return m_szArtist; }

	LONG GetVolume();
	BOOL SetVolume(DWORD dwValue);

	void GetDisplayTag(LPCTSTR szPath, TCHAR* pszTag, LONG nBufLen);

private:
	BOOL m_bPlay;
	MCI_OPEN_PARMS mciOpen;
	MCI_PLAY_PARMS mciPlay;
	LONG m_nPlayTime;

	BOOL FindID3V1(LPCTSTR szPath);

	TCHAR m_szSongTitle[MAX_PATH+1];
	TCHAR m_szArtist[35];
};
