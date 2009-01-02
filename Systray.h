/*
 * Systray.h
 *
 * Copyright (c) 1999 by Vincent Oostindie
 *
 * Class Systray provides easy access to the system tray. Simply create an
 * object of this class, and set the correct properties. 
 *
 * Show() places the object in the system tray, while Hide() removes it.
 * Modifications to the object (by calling set-methods) are reflected
 * on the system tray immediately.
 *
 * Note that this class doesn't use MFC.
 */

#if !defined(AFX_SYSTRAY_H__56F7FB81_0ABF_11D3_A2C8_AAE4D4001348__INCLUDED_)
#define AFX_SYSTRAY_H__56F7FB81_0ABF_11D3_A2C8_AAE4D4001348__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "shellapi.h"

class Systray  
{
private:
	NOTIFYICONDATA m_nidTrayIcon;
	bool m_bVisible;
	
	void Update();

public:
	Systray(HWND hWnd, UINT uID);
	virtual ~Systray();

	// Set methods
	void setIcon(const HICON hIcon);
	void setCallbackMessage(const UINT uCallbackMessage);
	void setToolTip(LPCTSTR lpToolTip);

	// Get methods
	inline HWND hWnd() const { return m_nidTrayIcon.hWnd; };
	inline UINT getID() const { return m_nidTrayIcon.uID; };
	inline HICON getIcon() const { return m_nidTrayIcon.hIcon; };
	inline UINT getCallbackMessage() const { return m_nidTrayIcon.uCallbackMessage; };
	inline LPCTSTR getToolTip() { return m_nidTrayIcon.szTip; };

	// Show and hide methods
	void Show();
	void Hide();
};

#endif // !defined(AFX_SYSTRAY_H__56F7FB81_0ABF_11D3_A2C8_AAE4D4001348__INCLUDED_)
// [end of file]