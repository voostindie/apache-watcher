/*
 * ApacheWatcher.cpp
 *
 * version 1.3
 *
 * Copyright (c) 1999 by Vincent Oostindie
 *
 * ApacheWatcher sits in the system tray and does the following:
 *	- On startup, it starts the Apache Web Server (if it isn't running already), but
 *    only if the '-r' of '-R' option was given.
 *	- On shutdown, it stops the Apache Web Server (if it hasn't stopped already)
 *
 * Put this program in your Startup group to automatically run the Apache Web
 * Server at startup. You can control the state of the server by double clicking
 * on the Apache icon on the system tray, or by choosing the preferred item on
 * the popup menu.
 */

#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "Systray.h"
#include "Apache.h"

// Application name
LPCTSTR lpAppName = "ApacheWatcher 1.3";
// ID for use in the system tray
const UINT ID = 1024;

// Global variables
HWND hWndMain;				// Main window
HMENU hMenu;				// Menu
HINSTANCE hInstanceMain;	// Main instance
HICON hIconOn;				// Icon: On
HICON hIconOff;				// Icon: Off
HICON hIconStartStop;		// Icon: Starting and stopping
Systray * systray;			// System tray object
Apache * apache;			// Apache Web Server controller object

// Change the settings of a single menuitem
inline void setMenu(UINT uMenuItem, UINT uSetting)
{
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE;
	mii.fState = uSetting;
	SetMenuItemInfo(hMenu, uMenuItem, TRUE, &mii);
}

// Enable or disable the Apache Web Server and set the system tray icon and 
// its menu to the correct values.
void setApache(bool bState)
{
	if (bState) // Enable Apache
	{
		systray->setIcon(hIconStartStop);
		systray->setToolTip("Apache Web Server is starting");
		apache->Start();
		setMenu(0, MFS_DISABLED);
		setMenu(1, MFS_ENABLED | MFS_DEFAULT);
		setMenu(2, MFS_ENABLED);
		systray->setToolTip("Apache Web Server is running");
		systray->setIcon(hIconOn);
	}
	else // Disable Apache
	{
		systray->setIcon(hIconStartStop);
		systray->setToolTip("Apache Web Server is stopping");
		apache->Stop();
		setMenu(0, MFS_ENABLED | MFS_DEFAULT);
		setMenu(1, MFS_DISABLED);
		setMenu(2, MFS_DISABLED);
		systray->setToolTip("Apache Web Server has stopped");
		systray->setIcon(hIconOff);
	}
}

// Override WM_CREATE, WM_ACTIVATE, WM_DESTROY and WM_COMMAND

// OnCreate is called when a window is created; that is: 
// after the call to CreateWindow in WinMain
static BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	hWndMain = hWnd;

	// Load the menu
	hMenu = LoadMenu(hInstanceMain, MAKEINTRESOURCE(IDR_MAIN));
	hMenu = GetSubMenu(hMenu, 0);

	// Load the icons
	hIconOn = LoadIcon(hInstanceMain, MAKEINTRESOURCE(IDI_ICON1));
	hIconOff = LoadIcon(hInstanceMain, MAKEINTRESOURCE(IDI_ICON2));
	hIconStartStop = LoadIcon(hInstanceMain, MAKEINTRESOURCE(IDI_ICON3));

	// Create the system tray icon and set defaults
	systray = new Systray(hWndMain, ID);
	systray->setCallbackMessage(WM_USER);
	systray->setIcon(hIconOn);
	
	// Find out if the server should be started
	LPTSTR strCommand = GetCommandLine();
	bool bStart = false;
	if (strstr(strCommand, "-r") != NULL || strstr(strCommand, "-R") != NULL)
		bStart = true;

	// Run the server
	apache = new Apache();
	if (apache->canRun())
	{
		setApache(bStart);
		systray->Show();
		return TRUE;
	}
	else
		return FALSE;
}

// OnActivate sets the application to idle on deactivation, so it doesn't 
// consume to much CPU time
static void OnActivate(HWND hWnd, UINT uState, HWND hWndActDeact, BOOL fMinimized)
{
	if (uState == WA_INACTIVE)
		 // Make applcation idle
		SetPriorityClass(hInstanceMain, IDLE_PRIORITY_CLASS);
	else
		// Make application active
		SetPriorityClass(hInstanceMain, NORMAL_PRIORITY_CLASS);
}

// OnDestroy is called when the application is closed, normally by
// selecting Quit on the menu
static void OnDestroy(HWND hWnd)
{
	// Stop the server;
	setApache(false);
	
	// Remove tray icon and apache from memory
	delete systray;
	delete apache;

	// Quit the application (better: the message loop)
	PostQuitMessage(0);
}

// OnCommand is called when the user selects a menuitem
static void OnCommand(HWND hWnd, int ID, HWND hWndCtl, UINT codeNotify)
{
	// Find out which menuitem was selected
	switch (ID)
	{
		case IDR_START: // Start the server
		{
			setApache(true);
			break;
		}
		case IDR_STOP: // Stop the server
		{
			setApache(false);
			break;
		}
		case IDR_RESTART: // Restart the server
		{
			systray->setIcon(hIconStartStop);
			apache->Restart();
			systray->setIcon(hIconOn);
			break;
		}
		case IDR_ABOUT: // Show an about box
		{
			MessageBox(NULL, 
				"ApacheWatcher version 1.3\n\nCopyright (c) 1999 by Vincent Oostindië\n"
				"http://www.vincent.demon.nl", lpAppName, MB_OK);
			break;
		}
		case IDR_QUIT: // Quit the application
		{
			systray->setIcon(hIconStartStop);
			apache->Stop();
			DestroyWindow(hWnd);
			break;
		}
	}
}

// Callback message for events and buttonclicks
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage) 
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);		// OnCreate handles WM_CREATE
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);	// OnDestroy handles WM_DESTROY
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);	// OnCommand handles WM_COMMAND
		HANDLE_MSG(hWnd, WM_ACTIVATE, OnActivate);	// OnActivate handles WM_ACTIVATE
		case WM_USER:
			switch (LOWORD(lParam))
			{
				case WM_LBUTTONDBLCLK: // Double left click: start or stop the Web Server
				{
					if (apache->isRunning())
					{
						setApache(false);
					}
					else
						setApache(true);
					break;
				}
				case WM_RBUTTONUP: // Right click: show the popupmenu
				{
					POINT p;
					GetCursorPos(&p);
				    SetForegroundWindow(hWnd);
					TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, p.x, p.y, 0, hWndMain, NULL);
					PostMessage(hWnd, WM_NULL, 0, 0);
					break;
				}
			}
		return 0;
		default: break;
	}

	return (DefWindowProc(hWnd, uMessage, wParam, lParam)); // Handle all other events

}

// The main method:
//  - Make sure only 1 instance is active
//  - Create a window and register it's class
//  - Run the message loop
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	// Make sure the application only runs once
	HANDLE mutex = CreateMutex(NULL, TRUE, "ApacheWatcher");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, "Multiple instance are not allowed", lpAppName, MB_OK);
		return FALSE;
	}

	hInstanceMain = hInstance;

	// Initialize a window and register it's class
	WNDCLASS wc;
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = lpAppName;
	if (!RegisterClass(&wc)) 
	{
		ReleaseMutex(mutex);
		MessageBox(NULL, "Could not register class", lpAppName, MB_OK);
		return FALSE;
	}

	// Create the window
	HWND hWnd = CreateWindowEx(0, lpAppName, lpAppName, 0, 0, 0, 1, 1, NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
	{
		ReleaseMutex(mutex);	
		MessageBox(NULL, "Could not create window", lpAppName, MB_OK);
		return FALSE;
	}

	// Start the message loop
	MSG msg;
    while (GetMessage(&msg, (HWND) NULL, 0, 0)) 
    { 
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    } 

	// Release the mutex and exit
	ReleaseMutex(mutex);
    return msg.wParam; 
}
// [end of file]