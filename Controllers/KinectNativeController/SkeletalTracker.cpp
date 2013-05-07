//------------------------------------------------------------------------------
// <copyright file="NuiImpl.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

// Implementation of SkeletalTracker methods dealing with NUI processing


//#pragma keyword("interface", on)

#include "stdafx.h"
#include "SkeletalTracker.h"
#include "kinectsetup.h"
#include "NuiApi.h"
#include "NuiSkeleton.h"

#include <sys/timeb.h>
//#include <time.h>

#undef OutputDebugString
#define OutputDebugString(x) wprintf(x)

#define INERTIA 0
#define START_DELAY 0
#define AWAY_FROM_BODY 2500
#define CLOSE_TO_BODY 2500
#define Y_ARM_RANGE 200
#define X_ARM_RANGE 200
#define X_BIAS 25
#define Y_BIAS 25

int getMilliCount(){
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

int getMilliSpan(int nTimeStart){
	int nSpan = getMilliCount() - nTimeStart;
	if(nSpan < 0)
		nSpan += 0x100000 * 1000;
	return nSpan;
}


//-------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------
SkeletalTracker::SkeletalTracker() : m_hInstance(NULL), m_trackState(UNTRACKED)
{
//	m_pNuiSensor = NULL;
//    Nui_Zero();
	m_gestureType = GESTURE_NONE;
	m_speech = new CSpeechBasics();

	//printf("created Skeletal class");
	//wprintf(L"WPRINT created Skeletal class");
}

//-------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------
SkeletalTracker::~SkeletalTracker()
{
    Nui_UnInit();
    Nui_Zero();
}
//-------------------------------------------------------------------
// Nui_Zero
//
// Zero out member variables
//-------------------------------------------------------------------
void SkeletalTracker::Nui_Zero()
{
//    if (m_pNuiSensor)
    {
 //       m_pNuiSensor->Release();
 //       m_pNuiSensor = NULL;
    }
    m_hNextSkeletonEvent = NULL;
    m_hThNuiProcess = NULL;
    m_hEvNuiProcessStop = NULL;
    m_LastSkeletonFoundTime = 0;
	delete m_speech;
	m_speech = NULL;
	m_trackState = UNTRACKED;
    ZeroMemory(m_SkeletonIds,sizeof(m_SkeletonIds));
    ZeroMemory(m_TrackedSkeletonIds,sizeof(m_SkeletonIds));
}

void CALLBACK SkeletalTracker::Nui_StatusProcThunk( HRESULT hrStatus, const OLECHAR* instanceName, const OLECHAR* uniqueDeviceName, void * pUserData )
{
    reinterpret_cast<SkeletalTracker *>(pUserData)->Nui_StatusProc( hrStatus, instanceName, uniqueDeviceName );
}

//-------------------------------------------------------------------
// Nui_StatusProc
//
// Callback to handle Kinect status changes
//-------------------------------------------------------------------
void CALLBACK SkeletalTracker::Nui_StatusProc( HRESULT hrStatus, const OLECHAR* instanceName, const OLECHAR* uniqueDeviceName )
{
    // Update UI
//    PostMessageW( m_hWnd, WM_USER_UPDATE_COMBO, 0, 0 );

    if( SUCCEEDED(hrStatus) )
    {
        if ( S_OK == hrStatus )
        {
//			Nui_Init();
        }
    }
    else
    {
        Nui_UnInit();
        Nui_Zero();
    }
}

//-------------------------------------------------------------------
// Nui_UnInit
//
// Uninitialize Kinect
//-------------------------------------------------------------------
void SkeletalTracker::Nui_UnInit( )
{

    // Stop the Nui processing thread
    if ( NULL != m_hEvNuiProcessStop )
    {
        // Signal the thread
        SetEvent(m_hEvNuiProcessStop);

        // Wait for thread to stop
        if ( NULL != m_hThNuiProcess )
        {
            WaitForSingleObject( m_hThNuiProcess, INFINITE );
            CloseHandle( m_hThNuiProcess );
        }
        CloseHandle( m_hEvNuiProcessStop );
    }

//    if ( m_pNuiSensor )
    {
//        m_pNuiSensor->NuiShutdown( );
		NuiShutdown( );
    }
    if ( m_hNextSkeletonEvent && ( m_hNextSkeletonEvent != INVALID_HANDLE_VALUE ) )
    {
        CloseHandle( m_hNextSkeletonEvent );
        m_hNextSkeletonEvent = NULL;
    }

//    if ( m_pNuiSensor )
    {
//        m_pNuiSensor->Release();
//        m_pNuiSensor = NULL;
    }

}

DWORD WINAPI SkeletalTracker::Nui_ProcessThread(LPVOID pParam)
{
    SkeletalTracker *pthis = (SkeletalTracker *) pParam;
    return pthis->Nui_ProcessThread();
}

//-------------------------------------------------------------------
// Nui_ProcessThread
//
// Thread to handle Kinect processing
//-------------------------------------------------------------------
DWORD WINAPI SkeletalTracker::Nui_ProcessThread()
{
    const int numEvents = 3;
    HANDLE hEvents[numEvents] = { m_hEvNuiProcessStop, m_hNextSkeletonEvent, m_speech->GetSpeechEvent() };

    //blank the skeleton display on startup
    m_LastSkeletonFoundTime = 0;

    // Main thread loop
    bool continueProcessing = true;
    while ( continueProcessing )
    {
        // Wait for any of the events to be signalled
//        nEventIdx = WaitForMultipleObjects( numEvents, hEvents, FALSE, 100 );
        DWORD dwEvent = MsgWaitForMultipleObjectsEx(numEvents, hEvents, INFINITE, QS_ALLINPUT, MWMO_INPUTAVAILABLE);

        // Process signal events
        switch ( dwEvent )
        {
            case WAIT_TIMEOUT:
                continue;

            // If the stop event, stop looping and exit
            case WAIT_OBJECT_0:
                continueProcessing = false;
                continue;

            case WAIT_OBJECT_0 + 1:
                Nui_GotSkeletonAlert( );
                break;

            case WAIT_OBJECT_0 + 2:
                m_speech->ProcessSpeech( );
                break;
		}

    }

    return 0;
}

int SkeletalTracker::initialise()
{
	return m_speech->CreateFirstConnected();

}

void SkeletalTracker::LoadPosition( NUI_SKELETON_DATA * pSkel, ArmPosition * pArmPosition)
{
	int i = NUI_SKELETON_POSITION_HAND_RIGHT;
	NuiTransformSkeletonToDepthImage( pSkel->SkeletonPositions[i], &(pArmPosition->m_rightHand.m_x), &(pArmPosition->m_rightHand.m_y), (USHORT*) &( pArmPosition->m_rightHand.m_z) );

	i = NUI_SKELETON_POSITION_HAND_LEFT;
	NuiTransformSkeletonToDepthImage( pSkel->SkeletonPositions[i], &(pArmPosition->m_leftHand.m_x), &(pArmPosition->m_leftHand.m_y), (USHORT*) &(pArmPosition->m_leftHand.m_z) );

	i = NUI_SKELETON_POSITION_HEAD;
	NuiTransformSkeletonToDepthImage( pSkel->SkeletonPositions[i], &(pArmPosition->m_body.m_x), &(pArmPosition->m_body.m_y), (USHORT*) &(pArmPosition->m_body.m_z) );

	pArmPosition->m_startTime = getMilliCount();
}


void SkeletalTracker::Nui_DrawSkeleton( NUI_SKELETON_DATA * pSkel, HWND hWnd, int WhichSkeletonColor )
{
    
	ArmPosition currentArmPosition;

	LoadPosition(pSkel, &currentArmPosition);
	{

		if( currentArmPosition.awayFromBody(AWAY_FROM_BODY))
		{
			XYZPoint* pPointer = NULL;
			float xBias = 0;
			float yBias = Y_BIAS;

			if (abs(currentArmPosition.m_body.m_z - currentArmPosition.m_leftHand.m_z) < CLOSE_TO_BODY )
			{
				pPointer = &currentArmPosition.m_rightHand;
				xBias = - X_BIAS;
			}
			else if (abs(currentArmPosition.m_body.m_z - currentArmPosition.m_rightHand.m_z) < CLOSE_TO_BODY )
			{
				pPointer = &currentArmPosition.m_leftHand;
				xBias = + X_BIAS;
			}
			else {
//				wprintf(L"body sep: %d, left: %d, right: %d \n", currentArmPosition.m_body.m_z, currentArmPosition.m_leftHand.m_z, currentArmPosition.m_rightHand.m_z);
			}
			if (pPointer != NULL) 
			{
//				wprintf(L"Pointing \n");
				float x = (float)(pPointer->m_x + xBias - currentArmPosition.m_body.m_x) / X_ARM_RANGE + 0.5f;
				float y =  (float)(pPointer->m_y - yBias - currentArmPosition.m_body.m_y) / Y_ARM_RANGE + 0.5f;
//				wprintf(L"Pointing body sep: %d, left: %d, right: %d, pointing at (%f, %f) \n", currentArmPosition.m_body.m_x, currentArmPosition.m_leftHand.m_x, currentArmPosition.m_rightHand.m_x, x, y);
				point(x, 1-y);
			}
			else 
			{
				boolean zooming = false;
				boolean panning = false;
				boolean rotating = false;
				boolean rotatingY = false;
				ArmPosition deltaArmPosition;
				deltaArmPosition.minus(currentArmPosition, m_lastArmPosition);
				int sumXdx = deltaArmPosition.m_leftHand.m_x +  deltaArmPosition.m_rightHand.m_x;
				int sumYdx = deltaArmPosition.m_leftHand.m_y +  deltaArmPosition.m_rightHand.m_y;
				SHORT sumZdx = deltaArmPosition.m_leftHand.m_z +  deltaArmPosition.m_rightHand.m_z;
				int diffXdx = deltaArmPosition.m_leftHand.m_x -  deltaArmPosition.m_rightHand.m_x;
				int diffYdx = deltaArmPosition.m_leftHand.m_y -  deltaArmPosition.m_rightHand.m_y;
				SHORT diffZdx = deltaArmPosition.m_leftHand.m_z -  deltaArmPosition.m_rightHand.m_z;

				int panningVal = (int)( (abs(sumYdx) + abs(sumXdx)));
				int rotationVal = abs(diffYdx);
				int rotationValY = abs(diffZdx) / 10;
				int zoomVal = abs(diffXdx);
				int inertia = INERTIA;
	//			printf("Pan: %d, rot: %d, rotY: %d (%d - %d), zoom: %d\n", panningVal, rotationVal, rotationValY, deltaArmPosition.m_leftHand.m_z, deltaArmPosition.m_rightHand.m_z, zoomVal);
				if (m_gestureType == GESTURE_PAN)
				{
					panningVal = (int) ((float)panningVal * 2);
				}
				else if (m_gestureType == GESTURE_ZOOM)
				{
					zoomVal = (int) ((float)zoomVal * 2);
				}
				else if (m_gestureType == GESTURE_ROTATE)
				{
					rotationVal = (int) ((float)rotationVal * 2.0f);
				}
				else if (m_gestureType == GESTURE_ROTATE_Y)
				{
					rotationValY = (int) ((float)rotationValY);
				}
				else
				{
					inertia = 0;
				}
		
				if (panningVal >= rotationVal && panningVal >= zoomVal && panningVal >= rotationValY && panningVal > inertia)
				{
					panning = true;
				}
				else if (zoomVal > panningVal && zoomVal > rotationVal && zoomVal >= rotationValY && zoomVal > inertia)
				{
					zooming = true;
				}
				else if (rotationVal > panningVal && rotationVal > zoomVal && rotationVal >= rotationValY && rotationVal > inertia + 3)
				{
					rotating = true;
				}
				else if (rotationValY > panningVal && rotationValY > zoomVal && rotationValY >= rotationVal && rotationValY > inertia + 3)
				{
					rotatingY = true;
				}
		//		wsprintf(output, L" panning %i, zooming %i, rotating %i\n\r", panningVal, zoomVal, rotationVal) ;
		//		OutputDebugString(output);
				if (zooming)
				{
		//			printf("starting zoom");
		//			wprintf(L"WPRINT zoom");
		//			wsprintf(output, L" body = %i, l = %i r = %i ", newBody.x, newLeftHand.x, newRightHand.x) ;
		//			OutputDebugString(output);
					if (m_gestureType == GESTURE_NONE)
					{

					}
					else
					{
						int zoom = diffXdx; 
		//				wsprintf(output, L" zooom %i \r\n", zoom) ;
		//				OutputDebugString(output);
						zoomNow(zoom);
					}
					m_gestureType = GESTURE_ZOOM;
					//printf("ending zoom");
					//wprintf(L"WPRINT ending zoom");
				}
				else if (rotating)
				{
					if (m_gestureType == GESTURE_NONE)
					{

					}
					else
					{
						int rotation = diffYdx;
		//				wsprintf(output, L" zooom %i \r\n", zoom) ;
		//				OutputDebugString(output);
						rotateNow(0,0,rotation);
					}
					m_gestureType = GESTURE_ROTATE;
					//printf("ending zoom");
					//wprintf(L"WPRINT ending zoom");
				}
				else if (rotatingY)
				{
					if (m_gestureType == GESTURE_NONE)
					{

					}
					else
					{
						int rotationX = 0;
						int rotationY = 0;
						if (abs(currentArmPosition.m_leftHand.m_x - currentArmPosition.m_rightHand.m_x) > 
							abs(currentArmPosition.m_leftHand.m_y - currentArmPosition.m_rightHand.m_y) ) 
						{
							rotationY = diffZdx;
						}
						else 
						{
							if (currentArmPosition.m_leftHand.m_y > currentArmPosition.m_rightHand.m_y)
							{ 
								diffZdx = -diffZdx;
							}
							rotationX = diffZdx;
						}
		//				wsprintf(output, L" zooom %i \r\n", zoom) ;
		//				OutputDebugString(output);
						rotateNow(rotationX, rotationY, 0);
					}
					m_gestureType = GESTURE_ROTATE_Y;
					//printf("ending zoom");
					//wprintf(L"WPRINT ending zoom");
				}
				else if (panning)
				{
		//			wsprintf(output, L" body = %i, l = %i r = %i ", newBody.x, newLeftHand.x, newRightHand.x) ;
		//			OutputDebugString(output);
					if (m_gestureType == GESTURE_NONE)
					{

					}
					else
					{
						int panX = sumXdx;
						int panY = sumYdx;
		//				wsprintf(output, L" pan %i,%i \r\n", panX, panY) ;
		//				OutputDebugString(output);
						panNow(panX, panY);
					}
					m_gestureType = GESTURE_PAN;
				}
				else
				{
					m_gestureType = GESTURE_NONE;
				}
			}
		}
		else
		{
			m_trackState = UNTRACKED;
//			printf("resetting Tracking\n");
			m_gestureType = GESTURE_NONE;
		}
	}

	m_lastArmPosition.copyFrom(currentArmPosition);
}


//-------------------------------------------------------------------
// Nui_GotSkeletonAlert
//
// Handle new skeleton data
//-------------------------------------------------------------------
void SkeletalTracker::Nui_GotSkeletonAlert( )
{
    NUI_SKELETON_FRAME SkeletonFrame = {0};

    bool bFoundSkeleton = false;

    if ( SUCCEEDED(NuiSkeletonGetNextFrame( 0, &SkeletonFrame )) )
//    if ( SUCCEEDED(m_pNuiSensor->NuiSkeletonGetNextFrame( 0, &SkeletonFrame )) )
    {
        for ( int i = 0 ; i < NUI_SKELETON_COUNT ; i++ )
        {
//            if( SkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED ||
//                (SkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_POSITION_ONLY ))
            {
                bFoundSkeleton = true;
            }
        }
    }

    // no skeletons!
    if( !bFoundSkeleton )
    {
        return;
    }

    // smooth out the skeleton data
	NUI_TRANSFORM_SMOOTH_PARAMETERS smoothParams;
	smoothParams.fSmoothing = 0.6f;
	smoothParams.fCorrection = 0.3f;
	smoothParams.fPrediction = 0.6f;
	smoothParams.fJitterRadius = 0.6f;
	smoothParams.fMaxDeviationRadius = 0.6f;
    HRESULT hr = NuiTransformSmooth(&SkeletonFrame,&smoothParams);
//    HRESULT hr = NuiTransformSmooth(&SkeletonFrame,&smoothParams);
    if ( FAILED(hr) )
    {
        return;
    }

    // we found a skeleton, re-start the skeletal timer

    bool bSkeletonIdsChanged = false;

	USHORT currentMaxDepth = NUI_IMAGE_DEPTH_MAXIMUM;
	int selectedSkeleton = -1;
    for ( int i = 0 ; i < NUI_SKELETON_COUNT; i++ )
    {

	
        if ( m_SkeletonIds[i] != SkeletonFrame.SkeletonData[i].dwTrackingID )
        {
            m_SkeletonIds[i] = SkeletonFrame.SkeletonData[i].dwTrackingID;
            bSkeletonIdsChanged = true;
        }

        // Show skeleton only if it is tracked, and the center-shoulder joint is at least inferred.
        if ( SkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED)
        {
            LONG   x, y;
            USHORT depth;

            // Transform skeleton coordinates to depth image
            NuiTransformSkeletonToDepthImage(SkeletonFrame.SkeletonData[i].Position, &x, &y, &depth);
			if (depth < currentMaxDepth) 
			{
				selectedSkeleton = i;
				currentMaxDepth = depth;
			}
        }
        else if ( SkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_POSITION_ONLY )
        {
//	         OutputDebugString( L"Skeleton position only\r\n" );
        }
    }
	if (selectedSkeleton > -1) 
	{
		Nui_DrawSkeleton( &SkeletonFrame.SkeletonData[selectedSkeleton], NULL, selectedSkeleton );
	}

}