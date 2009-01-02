/*
 * Apache.h
 *
 * Copyright (c) 1999 by Vincent Oostindie
 *
 * Class Apache provides a simple wrapper for the Apache Web Server.
 * When an Apache object is created, it looks in the current directory
 * for the Apache executable (APACHE.EXE). Check if the object can be
 * used by calling canRun() right after creation.
 *
 * Use the methods Start(), Restart() and Stop() to execute the 
 * corresponding actions on the server. Every method will block
 * until execution has failed or succeeded. (Especially stopping the
 * server will take some time.)
 *
 * The method isRunning() will inform you about the current state of
 * the Apache Web Server.
 */

#if !defined(AFX_APACHE_H__176A97AE_0A1C_11D3_A2C8_9E1665122048__INCLUDED_)
#define AFX_APACHE_H__176A97AE_0A1C_11D3_A2C8_9E1665122048__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Apache  
{
private:
	LPSTR lpFile;
	bool bInitOk;

	BOOL Execute(LPCTSTR lpParameters);
public:
	Apache();
	virtual ~Apache();

	bool canRun();
	bool isRunning();
	BOOL Start();
	BOOL Restart();
	BOOL Stop();
};

#endif // !defined(AFX_APACHE_H__176A97AE_0A1C_11D3_A2C8_9E1665122048__INCLUDED_)
// [end of file]