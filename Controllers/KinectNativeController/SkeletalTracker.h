//------------------------------------------------------------------------------
// <copyright file="SkeletalViewer.h" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

// Declares of SkeletalTracker class

#pragma once

#include "NuiApi.h"
#include "SpeechBasics.h"
#include "ArmPosition.h"

#define SZ_APPDLG_WINDOW_CLASS          _T("SkeletalViewerAppDlgWndClass")
#define WM_USER_UPDATE_FPS              WM_USER
#define WM_USER_UPDATE_COMBO            WM_USER+1
#define WM_USER_UPDATE_TRACKING_COMBO   WM_USER+2

#define GESTURE_NONE					0
#define GESTURE_ZOOM					1
#define GESTURE_PAN						2
#define GESTURE_ROTATE					3
#define GESTURE_ROTATE_Y				4

//#define STATE_UNTRACKED					0
//#define STATE_START_TRACKING			1
//#define STATE_TRACKING					2

enum TrackState {UNTRACKED, START_TRACKING, TRACKING};

class SkeletalTracker
{
public:
    SkeletalTracker();
    ~SkeletalTracker();
//    HRESULT                 Nui_Init( );
//    HRESULT                 Nui_Init( OLECHAR * instanceName );
    void                    Nui_UnInit( );
    void                    Nui_GotDepthAlert( );
    void                    Nui_GotColorAlert( );
    void                    Nui_GotSkeletonAlert( );

    void                    Nui_Zero();

    void                    Nui_EnableSeatedTracking(bool seated);
    void                    Nui_SetApplicationTracking(bool applicationTracks);
    void                    Nui_SetTrackedSkeletons(int skel1, int skel2);
	void					Nui_DrawSkeleton( NUI_SKELETON_DATA * pSkel, HWND hWnd, int WhichSkeletonColor );

    RGBQUAD                 Nui_ShortToQuad_Depth( USHORT s );

    static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK        WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static void CALLBACK    Nui_StatusProcThunk(HRESULT hrStatus, const OLECHAR* instanceName, const OLECHAR* uniqueDeviceName, void* pUserData);
    void CALLBACK           Nui_StatusProc( HRESULT hrStatus, const OLECHAR* instanceName, const OLECHAR* uniqueDeviceName );
	int					initialise();
    HWND                    m_hWnd;
    HINSTANCE               m_hInstance;
	
    int MessageBoxResource(UINT nID, UINT nType);
    static DWORD WINAPI     Nui_ProcessThread(LPVOID pParam);
    DWORD WINAPI            Nui_ProcessThread();
    HANDLE        m_hNextSkeletonEvent;
	HANDLE		  m_hNextDepthFrameEvent;
	HANDLE        m_hThNuiProcess;
    HANDLE        m_hEvNuiProcessStop;

protected:
	void LoadPosition( NUI_SKELETON_DATA * pSkel, ArmPosition * pArmPosition);

private:

    // Current kinect
//    INuiSensor *            m_pNuiSensor ;
	int				m_gestureType;

    // thread handling
	CSpeechBasics		*m_speech;		

	ArmPosition   m_lastArmPosition, m_startingArmPosition;
	TrackState	  m_trackState;

    DWORD         m_LastSkeletonFoundTime;
    DWORD         m_SkeletonIds[NUI_SKELETON_COUNT];
    DWORD         m_TrackedSkeletonIds[NUI_SKELETON_MAX_TRACKED_COUNT];
    ULONG_PTR     m_GdiplusToken;
};



