////////////////////////////////////////////////////////////////
// MSDN Magazine -- August 2002
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio 6.0 on Windows XP.
//
// Some adaptation from FastFrench to make it more reliable 

#include "stdafx.h"
//lib: psapi.lib
#include    <windows.h>
#include	<stdio.h>
#include	<string.h>
//#include	<conio.h>
//#include <iostream>

#include "EnumProc.h"
#include "psapi.h"

#define _MAX_PATH 260

////////////////////////////////////////////////////////////////
// CProcessIterator - Iterates all processes
//
CProcessIterator::CProcessIterator()
{
	m_pids = NULL;
}

CProcessIterator::~CProcessIterator()
{
	delete [] m_pids;
}

//////////////////
// Get first process: Call EnumProcesses to init array. Return first one.
//
DWORD CProcessIterator::First()
{
	m_current = (DWORD)-1;
	m_count = 0;
	DWORD nalloc = 1024;
	do {
		delete [] m_pids;
		m_pids = new DWORD [nalloc];
		if (EnumProcesses(m_pids, nalloc*sizeof(DWORD), &m_count)) {
			m_count /= sizeof(DWORD);
			m_current = 1;						 // skip IDLE process
		}
	} while (nalloc <= m_count);

	return Next();
}

////////////////////////////////////////////////////////////////
// CProcessModuleIterator - Iterates all modules in a process
//
CProcessModuleIterator::CProcessModuleIterator(DWORD pid)
{
	m_hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE, pid);
}

CProcessModuleIterator::~CProcessModuleIterator()
{
	CloseHandle(m_hProcess);
	delete [] m_hModules;
}

//////////////////
// Get first module in process. Call EnumProcesseModules to
// initialize entire array, then return first module.
//
HMODULE CProcessModuleIterator::First()
{
	m_count = 0;
	m_current = (DWORD)-1; 
	m_hModules = NULL;
	if (m_hProcess) {
		DWORD nalloc = 1024;
		do {
			delete [] m_hModules;
			m_hModules = new HMODULE [nalloc];
			if (EnumProcessModules(m_hProcess, m_hModules,
				nalloc*sizeof(DWORD), &m_count)) {
				m_count /= sizeof(HMODULE);
				m_current = 0;
			}
		} while (nalloc <= m_count);
	}
	return Next();
}

////////////////////////////////////////////////////////////////
// CWindowIterator - Iterates all top-level windows (::EnumWindows)
//
CWindowIterator::CWindowIterator(DWORD nAlloc)
	: m_current(0), m_count(0)
{
//	assert(nAlloc>0);
	m_hwnds = new HWND [nAlloc];
	m_nAlloc = nAlloc;
}

CWindowIterator::~CWindowIterator()
{
	delete [] m_hwnds;
}

// callback passes to virtual fn
BOOL CALLBACK CWindowIterator::EnumProc(HWND hwnd, LPARAM lp)
{
	return ((CWindowIterator*)lp)->OnEnumProc(hwnd);
}

//////////////////
// Virtual enumerator proc: add HWND to array if OnWindows is TRUE.
// You can override OnWindow to filter windows however you like.
//
BOOL CWindowIterator::OnEnumProc(HWND hwnd)
{
	if (OnWindow(hwnd)) {
		if (m_count < m_nAlloc)
			m_hwnds[m_count++] = hwnd;
	}
	return TRUE; // keep looking
}

////////////////////////////////////////////////////////////////
// CMainWindowIterator - Iterates the main windows of a process.
// Overrides CWindowIterator::OnWindow to filter.
//
CMainWindowIterator::CMainWindowIterator(DWORD pid, BOOL bVis,
	DWORD nAlloc) : CWindowIterator(nAlloc), m_pid(pid), m_bVisible(bVis)
{
}

CMainWindowIterator::~CMainWindowIterator()
{
}

//////////////////
// OnWindow:: Is window's process ID the one i'm looking for?
// Set m_bVisible=FALSE to find invisible windows too.
//
BOOL CMainWindowIterator::OnWindow(HWND hwnd)
{
	if (!m_bVisible || (GetWindowLong(hwnd,GWL_STYLE) & WS_VISIBLE)) {
		DWORD pidwin;
		GetWindowThreadProcessId(hwnd, &pidwin);
		if (pidwin==m_pid)
			return TRUE;
	}
	return FALSE;
}

////////////////////////////////////////////////////////////////
// CFindKillProcess - to find/kill a process by module name.
//
//CFindKillProcess::CFindKillProcess()
//{
//}


//////////////////
// Search for process whose module name matches parameter.
// Finds "foo" or "foo.exe"
//
CFindKillProcess::CFindKillProcess(LPCTSTR cfg_s_ModName)
{
	CProcessIterator itp;
	m_pid = 0;
	for (DWORD pid=itp.First(); pid; pid=itp.Next()) {
		TCHAR name[_MAX_PATH];
		CProcessModuleIterator itm(pid);
		HMODULE hModule = itm.First(); // .EXE
		if (hModule) {
			GetModuleBaseName(itm.GetProcessHandle(),
				hModule, name, _MAX_PATH);
			char scfg_s_ModName [_MAX_PATH];
			strcpy(scfg_s_ModName, cfg_s_ModName);
			//string scfg_s_ModName = cfg_s_ModName;
			if (strcmpi(scfg_s_ModName,name)==0)
				{m_pid = pid; break;}
			strcat(scfg_s_ModName, ".exe");
			if (strcmpi(scfg_s_ModName,name)==0)
				{m_pid = pid; break;}
		}
	}
	if (m_pid)
		m_hp = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE|PROCESS_QUERY_INFORMATION,FALSE,m_pid);
	else
		m_hp = NULL;
	GetCPUUsage();
}

CFindKillProcess::CFindKillProcess(DWORD pid, HANDLE ph)
{
	m_pid = pid;
	if (ph != NULL)
		m_hp = ph;
	else
		if (m_pid!=NULL)
			m_hp = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE|PROCESS_QUERY_INFORMATION,FALSE,m_pid);
	else
		m_hp = NULL;
	GetCPUUsage();
}
/*
typedef struct _IO_COUNTERS {
    ULONGLONG  ReadOperationCount;
    ULONGLONG  WriteOperationCount;
    ULONGLONG  OtherOperationCount;
    ULONGLONG ReadTransferCount;
    ULONGLONG WriteTransferCount;
    ULONGLONG OtherTransferCount;
} IO_COUNTERS;
typedef IO_COUNTERS *PIO_COUNTERS;
BOOL GetProcessIoCounters(
  HANDLE hProcess,           // handle to process
  PIO_COUNTERS lpIoCounters  // I/O accouting information
);

BOOL GetProcessWorkingSetSize(
  HANDLE hProcess,                 // handle to the process
  PSIZE_T lpMinimumWorkingSetSize, // minimum working set size
  PSIZE_T lpMaximumWorkingSetSize  // maximum working set size
);

_heapmin();

*/

DWORD CFindKillProcess::GetCPUUsage() // utilisation CPU du process, sur échelle de 0 à 10000: 10000 = 100%
{
FILETIME T1, T2, T3, T4;
unsigned __int64 TotalCPU;
DWORD Result;
DWORD dw_now;
if (!GetProcessTimes(
  m_hp,           // handle to process
  &T1, // process creation time
  &T2,     // process exit time
  &T3,   // process kernel-mode time
  &T4      // process user-mode time
)) return 0xFFFFFFFF;
//UInt64
ULARGE_INTEGER Tmp1, Tmp2;
Tmp1.HighPart = T3.dwHighDateTime;
Tmp2.HighPart = T4.dwHighDateTime;
Tmp1.LowPart = T3.dwLowDateTime;
Tmp2.LowPart = T4.dwLowDateTime;

TotalCPU = Tmp1.QuadPart + Tmp2.QuadPart;//(ULARGE_INTEGER) T3.dwLowDateTime + (ULARGE_INTEGER) T4.dwLowDateTime + ((ULARGE_INTEGER)T3.dwHighDateTime + (ULARGE_INTEGER)T4.dwHighDateTime) << 32;
dw_now = GetTickCount();
if (m_LastTime == dw_now) return 0;
Result = (DWORD) ((TotalCPU - m_LastCPU)/(dw_now - m_LastTime));
m_LastTime = dw_now;
m_LastCPU  = TotalCPU;
return Result;
}

DWORD CFindKillProcess::GetMemoryUsage() // utilisation CPU du process, sur échelle de 0 à 10000: 10000 = 100%
{
PROCESS_MEMORY_COUNTERS pmc;
if ( GetProcessMemoryInfo( m_hp, &pmc, sizeof(pmc)) )
    {
        //printf( "\tPageFaultCount: 0x%08X\n", pmc.PageFaultCount );
        /*printf( "\tPeakWorkingSetSize: %d Ko\n", 
                  pmc.PeakWorkingSetSize/(1024));*/
		return (DWORD)pmc.WorkingSetSize;
        /*printf( "\tQuotaPeakPagedPoolUsage: 0x%08X (%d Ko)\n", 
                  pmc.QuotaPeakPagedPoolUsage, pmc.QuotaPeakPagedPoolUsage/1024 );
        printf( "\tQuotaPagedPoolUsage: 0x%08X (%d Ko)\n", 
                  pmc.QuotaPagedPoolUsage, pmc.QuotaPagedPoolUsage/1024 );
        printf( "\tQuotaPeakNonPagedPoolUsage: 0x%08X (%d Ko)\n", 
                  pmc.QuotaPeakNonPagedPoolUsage, pmc.QuotaPeakNonPagedPoolUsage/1024 );
        printf( "\tQuotaNonPagedPoolUsage: 0x%08X (%d Ko)\n", 
                  pmc.QuotaNonPagedPoolUsage, pmc.QuotaNonPagedPoolUsage/1024 );
        printf( "\tPagefileUsage: 0x%08X (%d Ko)\n", pmc.PagefileUsage, pmc.PagefileUsage/1024 ); 
        printf( "\tPeakPagefileUsage: 0x%08X (%d Ko)\n", 
                  pmc.PeakPagefileUsage, pmc.PeakPagefileUsage/1024 );*/
    }
return 0;
}

bool CFindKillProcess::IsResponding()
{
	DWORD ExitCode;
	if (m_hp==NULL) return FALSE;
	GetExitCodeProcess(m_hp, &ExitCode);
	if (ExitCode!=STILL_ACTIVE) return FALSE;
	return WaitForInputIdle(m_hp, 20000)==0;
}


CFindKillProcess::~CFindKillProcess()
{
	if (m_hp!=NULL) CloseHandle(m_hp);
	m_hp = NULL;
	m_pid = 0;
}
	
//////////////////
// Kill a process cleanly: Close main windows and wait.
// bZap=TRUE to force kill.
//
BOOL CFindKillProcess::KillProcess(int iDelay)
{
	CMainWindowIterator itw(m_pid);
	for (HWND hwnd=itw.First(); hwnd; hwnd=itw.Next()) {
		DWORD bOKToKill = FALSE;
		SendMessageTimeout(hwnd, WM_QUERYENDSESSION, 0, 0,
			SMTO_ABORTIFHUNG|SMTO_NOTIMEOUTIFNOTHUNG, iDelay, &bOKToKill); // 60 secondes pour sauver les persos et quitter
	//if (!bOKToKill)
			//return FALSE;  // window doesn't want to die: abort
		
		// added by asmodae, passes message WM_CLOSE and WM_NCDESTROY 
		// directly for the case of the NWServer process.
		//DestroyWindow(hwnd);
		//PostMessage(hwnd,WM_DESTROY,0,0);

	}

	// I've closed the main windows; now wait for process to die. 
	DWORD ExitCode = 0;
	if (m_hp) {
		if (WaitForSingleObject(m_hp, iDelay) != WAIT_OBJECT_0) TerminateProcess(m_hp,0); // encore 60 secondes de plus pour quitter
		
		// Now check if process not still active
		GetExitCodeProcess(m_hp,		// handle to the process
							&ExitCode   // termination status
							);
		return ExitCode!=STILL_ACTIVE;
	}
	

	return TRUE;
}

// PROCESSENTRY32 MODULEENTRY32 ms-help://MS.VSCC/MS.MSDNVS.1036/perfmon/toolhelp_99rn.htm
