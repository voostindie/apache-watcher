/*
 * Systray.cpp
 *
 * Copyright (c) 1999 by Vincent Oostindie
 *
 * See Systray.h for global class information
 */

#include "stdafx.h"
#include "Systray.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Systray::Systray(HWND hWnd, UINT uID)
{
	m_nidTrayIcon.cbSize = sizeof(NOTIFYICONDATA);
	m_nidTrayIcon.hWnd = hWnd;
	m_nidTrayIcon.uID = uID;
	m_nidTrayIcon.uFlags = 0;
	m_bVisible = false;
}

Systray::~Systray()
{
	Hide();
}


//////////////////////////////////////////////////////////////////////
// Set methods
//////////////////////////////////////////////////////////////////////

void Systray::setIcon(const HICON hIcon)
{
	m_nidTrayIcon.hIcon = hIcon;
	m_nidTrayIcon.uFlags |= NIF_ICON;
	Update();
}

void Systray::setCallbackMessage(const UINT uCallbackMessage)
{
	m_nidTrayIcon.uCallbackMessage = uCallbackMessage;
	m_nidTrayIcon.uFlags |= NIF_MESSAGE;
	Update();
}

void Systray::setToolTip(LPCTSTR lpToolTip)
{
	strncpy(m_nidTrayIcon.szTip, lpToolTip, sizeof(m_nidTrayIcon.szTip) - 1);
	m_nidTrayIcon.uFlags |= NIF_TIP;
	Update();
}


//////////////////////////////////////////////////////////////////////
// Show, Hide and Update methods
//////////////////////////////////////////////////////////////////////

void Systray::Show()
{
	if (m_bVisible) return;
	m_bVisible = true;
	Shell_NotifyIcon(NIM_ADD, &m_nidTrayIcon);
}

void Systray::Hide()
{
	if (!m_bVisible) return;
	m_bVisible = false;
	Shell_NotifyIcon(NIM_DELETE, &m_nidTrayIcon);
}

void Systray::Update()
{
	Shell_NotifyIcon(NIM_MODIFY, &m_nidTrayIcon);
}
// [end of file]