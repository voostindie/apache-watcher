/*
 * Apache.cpp
 *
 * Copyright (c) 1999 by Vincent Oostindie
 *
 * See Apache.h for global class information
 */

#include "stdafx.h"
#include "shellapi.h"
#include "stdlib.h"
#include "io.h"
#include "direct.h"
#include "Apache.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Apache::Apache()
{
	bInitOk = false;
	lpFile = "APACHE.EXE";

	// Search for the Apache executable
	WIN32_FIND_DATA fileData;
	HANDLE hFile = FindFirstFile(lpFile, &fileData);
	if (hFile == INVALID_HANDLE_VALUE) return; // not found
	FindClose(hFile);
	
	bInitOk = true;
}

Apache::~Apache()
{
	Stop();
	if (lpFile != NULL)
		delete [] lpFile;
}

// Report if the Apache executable was found
bool Apache::canRun()
{
	return bInitOk;
}

// Report if the server is running or not
bool Apache::isRunning()
{
	HWND hwndApache = FindWindow("tty", "APACHE");
	if (hwndApache == NULL)
		return false;
	else
		return true;
}

// Start the executable with the given parameters
inline BOOL Apache::Execute(LPCTSTR lpParameters)
{
	if ((int)ShellExecute(GetDesktopWindow(), NULL, lpFile, lpParameters, NULL, SW_HIDE) < 33)
		return FALSE;
	else
		return TRUE;
}

// Start the server
BOOL Apache::Start()
{
	if (isRunning())
		return TRUE;
	
	if (!Execute(NULL)) return FALSE;

	// Block until the server is running
	while (!isRunning());
	
	return TRUE;
}

// Restart the server
BOOL Apache::Restart()
{
	if (!isRunning()) 
		return Start();

	if (!Execute("-k restart")) return FALSE;

	return TRUE;
}

// Stop the server
BOOL Apache::Stop()
{
	if (!isRunning())
		return TRUE;

	if (!Execute("-k shutdown")) return FALSE;
	
	// Block until the server has really stopped
	while (isRunning());

	return TRUE;
}
// [end of file]