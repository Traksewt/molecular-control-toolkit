//

#include "stdafx.h"
#include "kinectsetup.h"
#include "NuiApi.h"

#include "SkeletalTracker.h"
#include "SpeechBasics.h"

JavaVM *jvm; 
jmethodID zoomMethodId;
jmethodID panMethodId;
jmethodID rotateMethodId;
jmethodID speechMethodId;
jmethodID resetMethodId;
jmethodID zoomToSelectionMethodId;
jmethodID selectAllMethodId;
jmethodID searchMethodId;
jmethodID pointMethodId;
jmethodID selectMethodId;
jmethodID spinMethodId;
jmethodID copyMethodId;
jmethodID pasteMethodId;
jmethodID enableHandsMethodId;
jmethodID enableVoiceMethodId;
jmethodID colorMethodId;

SkeletalTracker *skeleton = NULL;

INuiSensor * mNuiSensor = NULL;
HANDLE m_hDepthStream = NULL;

#define KINECT_CONTROLLER_CLASS "org/odonoghuelab/molecularcontroltoolkit/internal/KinectConnector" 

//DEFINE_GUID(CLSID_ExpectedRecognizer, 0x495648e7, 0xf7ab, 0x4267, 0x8e, 0x0f, 0xca, 0xfb, 0x7a, 0x33, 0xc1, 0x60);

JNIEXPORT void JNICALL Java_org_odonoghuelab_molecularcontroltoolkit_internal_KinectConnector_deregisterKinect
  (JNIEnv * p_env, jobject p_obj) 
{
	if (skeleton != NULL)
	{
		delete skeleton;
		skeleton = NULL;
	}
	if (mNuiSensor)
    {
        mNuiSensor->Release();
        mNuiSensor = NULL;
    }
	
}

DWORD WINAPI createObject(LPVOID pParam)
{
	skeleton = new SkeletalTracker();

	skeleton->initialise();

    skeleton->m_hNextSkeletonEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	HRESULT hr = mNuiSensor->NuiSkeletonTrackingEnable( skeleton->m_hNextSkeletonEvent, NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT | NUI_SKELETON_TRACKING_FLAG_ENABLE_IN_NEAR_RANGE);
    if( FAILED( hr ) )
    {
	    OutputDebugString( L"Skeleton tracking ERROR\r\n" );
        return hr;
    }
	  hr = mNuiSensor->NuiImageStreamOpen(
      NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX,
//	  NUI_IMAGE_RESOLUTION_320x240,
	  NUI_IMAGE_RESOLUTION_640x480,
      NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE,
      NUI_IMAGE_STREAM_FRAME_LIMIT_MAXIMUM,
      NULL,
      &m_hDepthStream);


    // Start the Nui processing thread
    skeleton->m_hEvNuiProcessStop = CreateEvent( NULL, FALSE, FALSE, NULL );
    skeleton->m_hThNuiProcess = CreateThread( NULL, 0, skeleton->Nui_ProcessThread, skeleton, 0, NULL );



//	fprintf(stderr, "created kinect object ");
	return 0;
}

JNIEXPORT void JNICALL Java_org_odonoghuelab_molecularcontroltoolkit_internal_KinectConnector_registerKinect
  (JNIEnv * p_env, jobject p_obj) 
{
	INuiSensor * pNuiSensor;

    int iSensorCount = 0;
    HRESULT hr = NuiGetSensorCount(&iSensorCount);
    if (FAILED(hr))
    {
		fprintf(stderr, "Failed getting sensor count %i", hr);
		return;
    }

    // Look at each Kinect sensor
    for (int i = 0; i < iSensorCount; ++i)
    {
        // Create the sensor so we can check status, if we can't create it, move on to the next
        hr = NuiCreateSensorByIndex(i, &pNuiSensor);
        if (FAILED(hr))
        {
            continue;
        }

        // Get the status of the sensor, and if connected, then we can initialize it
        hr = pNuiSensor->NuiStatus();
        if (S_OK == hr)
        {
            mNuiSensor = pNuiSensor;
            break;
        }

        // This sensor wasn't OK, so release it since we're not using it
        pNuiSensor->Release();
    }



	DWORD nuiFlags = NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_SKELETON | NUI_INITIALIZE_FLAG_USES_AUDIO ;
    mNuiSensor->NuiInitialize( nuiFlags );
//    NuiSetDeviceStatusCallback( &SkeletalTracker::Nui_StatusProcThunk, this );
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    if (SUCCEEDED(hr))
    {
			//HANDLE orocess = CreateThread( NULL, 0, createObject, NULL, 0, NULL );
			createObject(NULL);
			fprintf(stderr, "got JVM2");
			p_env->GetJavaVM(&jvm);
			jclass cls;
			fprintf(stderr, "got JVM3");

			cls = p_env->FindClass(KINECT_CONTROLLER_CLASS);
			if(cls == NULL){
				OutputDebugString( L"cannot find class KinectConnector\n") ;
				fprintf(stderr, "cannot find class KinectConnector\n") ;
		//		exit(-1);
			}
		//		printf("found class\n");
		
			
			enableHandsMethodId = p_env->GetStaticMethodID(cls, "enableHands", "(Z)V" );
			if(enableHandsMethodId ==0){
				OutputDebugString( L"cannot find method enableHandsMethodId\n");
				fprintf(stderr,"cannot find method enableHandsMethodId\n");
		//		exit(-1);
			}
			enableVoiceMethodId = p_env->GetStaticMethodID(cls, "enableVoice", "(Z)V" );
			if(enableVoiceMethodId ==0){
				OutputDebugString( L"cannot find method enableVoiceMethodId\n");
				fprintf(stderr,"cannot find method enableVoiceMethodId\n");
		//		exit(-1);
			}
			zoomMethodId = p_env->GetStaticMethodID(cls, "triggerZoom", "(I)V" );
			if(zoomMethodId ==0){
				OutputDebugString( L"cannot find method fun\n");
				fprintf(stderr,"cannot find method triggerZoom\n");
		//		exit(-1);
			}
			panMethodId = p_env->GetStaticMethodID(cls, "triggerPan", "(II)V" );
			if(panMethodId ==0){
				OutputDebugString( L"cannot find method triggerPan\n");
				fprintf(stderr,"cannot find method triggerPan\n");
		//		exit(-1);
			}
			rotateMethodId = p_env->GetStaticMethodID(cls, "triggerRotate", "(III)V" );
			if(rotateMethodId ==0){
				OutputDebugString( L"cannot find method triggerRotate\n");
				fprintf(stderr, "cannot find method triggerRotate\n");
		//		exit(-1);
			}
			speechMethodId = p_env->GetStaticMethodID(cls, "triggerSpeech", "(I)V" );
			if(speechMethodId ==0){
				OutputDebugString( L"cannot find method triggerSpeech\n");
				fprintf(stderr, "cannot find method triggerSpeech\n");
		//		exit(-1);
			}
			resetMethodId = p_env->GetStaticMethodID(cls, "reset", "()V" );
			if(resetMethodId ==0){
				OutputDebugString( L"cannot find method resetMethod\n");
				fprintf(stderr, "cannot find method resetMethod\n");
		//		exit(-1);
			}
			zoomToSelectionMethodId = p_env->GetStaticMethodID(cls, "zoom", "()V" );
			if(zoomToSelectionMethodId ==0){
				OutputDebugString( L"cannot find method zoomToSelectionMethodId\n");
				fprintf(stderr, "cannot find method zoomToSelectionMethodId\n");
		//		exit(-1);
			}
			selectAllMethodId = p_env->GetStaticMethodID(cls, "selectAll", "()V" );
			if(selectAllMethodId ==0){
				OutputDebugString( L"cannot find method selectAllMethodId\n");
				fprintf(stderr, "cannot find method selectAllMethodId\n");
		//		exit(-1);
			}
			selectMethodId = p_env->GetStaticMethodID(cls, "select", "(Ljava/lang/String;)V" );
			if(selectMethodId ==0){
				OutputDebugString( L"cannot find method selectMethodId\n");
				fprintf(stderr, "cannot find method selectMethodId\n");
		//		exit(-1);
			}
			searchMethodId = p_env->GetStaticMethodID(cls, "search", "(C)V" );
			if(searchMethodId ==0){
				OutputDebugString( L"cannot find method searchMethodId\n");
				fprintf(stderr, "cannot find method searchMethodId\n");
		//		exit(-1);
			}
			pointMethodId = p_env->GetStaticMethodID(cls, "point", "(FF)V" );
			if(pointMethodId ==0){
				OutputDebugString( L"cannot find method pointMethodId\n");
				fprintf(stderr, "cannot find method pointMethodId\n");
		//		exit(-1);
			}
			spinMethodId = p_env->GetStaticMethodID(cls, "spin", "(Z)V" );
			if(spinMethodId ==0){
				OutputDebugString( L"cannot find method spinMethodId\n");
				fprintf(stderr, "cannot find method spinMethodId\n");
		//		exit(-1);
			}
			copyMethodId = p_env->GetStaticMethodID(cls, "copy", "()V" );
			if(copyMethodId ==0){
				OutputDebugString( L"cannot find method copyMethodId\n");
				fprintf(stderr, "cannot find method copyMethodId\n");
		//		exit(-1);
			}
			pasteMethodId = p_env->GetStaticMethodID(cls, "paste", "()V" );
			if(pasteMethodId ==0){
				OutputDebugString( L"cannot find method pasteMethodId\n");
				fprintf(stderr, "cannot find method pasteMethodId\n");
		//		exit(-1);
			}
			colorMethodId = p_env->GetStaticMethodID(cls, "color", "(Ljava/lang/String;)V" );
			if(colorMethodId ==0){
				OutputDebugString( L"cannot find method colorMethodId\n");
				fprintf(stderr, "cannot find method colorMethodId\n");
		//		exit(-1);
			}

			
			printf("Finished Initialising Kinect Interface");

        CoUninitialize();
    }



}

void enableHands(bool enabled)
{
	JNIEnv * env;
//	printf("about to call back to java\n");
	jvm->AttachCurrentThread((void **)&env, NULL);
	jclass cls;

	cls = env->FindClass(KINECT_CONTROLLER_CLASS);
	if(cls == NULL){
		OutputDebugString( L"cannot find class KinectConnector\n") ;
		printf( "cannot find class KinectConnector\n") ;
		exit(-1);
	}
	env->CallVoidMethod(cls,enableHandsMethodId, enabled);

	jvm->DetachCurrentThread();
}

void zoomNow(int zoom)
{
	JNIEnv * env;
//	printf("about to call back to java\n");
	jvm->AttachCurrentThread((void **)&env, NULL);
	jclass cls;

	cls = env->FindClass(KINECT_CONTROLLER_CLASS);
	if(cls == NULL){
		OutputDebugString( L"cannot find class KinectConnector\n") ;
		printf( "cannot find class KinectConnector\n") ;
		exit(-1);
	}
	env->CallVoidMethod(cls,zoomMethodId, zoom);

	jvm->DetachCurrentThread();

}

void rotateNow(int rotateX, int rotateY, int rotateZ)
{
	JNIEnv * env;
	jvm->AttachCurrentThread((void **)&env, NULL);
	jclass cls;
//	printf("rotating now %i \n \ r", rotate);

	cls = env->FindClass(KINECT_CONTROLLER_CLASS);
	if(cls == NULL){
		OutputDebugString( L"cannot find class KinectConnector\n") ;
		printf( "cannot find class KinectConnector\n") ;
		exit(-1);
	}
	env->CallVoidMethod(cls,rotateMethodId, rotateX, rotateY, rotateZ );


}


void panNow(int x, int y)
{
	JNIEnv * env;
//	printf("about to call back to java\n");
	jvm->AttachCurrentThread((void **)&env, NULL);
	jclass cls;

		cls = env->FindClass(KINECT_CONTROLLER_CLASS);
	if(cls == NULL){
		OutputDebugString( L"cannot find class KinectConnector\n") ;
		printf( "cannot find class KinectConnector\n") ;
		exit(-1);
	}
	env->CallVoidMethod(cls,panMethodId, x, y);
}

void point(float x, float y)
{
	JNIEnv * env;
//	printf("about to call back to java\n");
	jvm->AttachCurrentThread((void **)&env, NULL);
	jclass cls;

	cls = env->FindClass(KINECT_CONTROLLER_CLASS);
	if(cls == NULL){
		OutputDebugString( L"cannot find class KinectConnector\n") ;
		printf( "cannot find class KinectConnector\n") ;
		exit(-1);
	}
	env->CallVoidMethod(cls,pointMethodId, x, y);

	jvm->DetachCurrentThread();

}

void processSpeechSearch(LPCWSTR searchTerm) 
{
	JNIEnv * env;
//	printf("about to call back to java\n");
	jvm->AttachCurrentThread((void **)&env, NULL);
	jclass cls;

		cls = env->FindClass(KINECT_CONTROLLER_CLASS);
	if(cls == NULL){
		OutputDebugString( L"cannot find class KinectConnector\n") ;
		printf( "cannot find class KinectConnector\n") ;
		exit(-1);
	}
	env->CallVoidMethod(cls,searchMethodId, *searchTerm);
}

void processSpeech(AquariaAction action) 
{
		JNIEnv * env;
	jvm->AttachCurrentThread((void **)&env, NULL);
	jclass cls;
//	printf("rotating now %i \n \ r", rotate);

	cls = env->FindClass(KINECT_CONTROLLER_CLASS);
	if(cls == NULL){
		OutputDebugString( L"cannot find class KinectConnector\n") ;
		printf( "cannot find class KinectConnector\n") ;
		exit(-1);
	}

	switch (action) {
	case ActionReset:
		env->CallVoidMethod(cls,resetMethodId);
		break;

	case ActionZoom:
		env->CallVoidMethod(cls,zoomToSelectionMethodId);
		break;
	
	case ActionSelectAll:
		env->CallVoidMethod(cls,selectMethodId, env->NewStringUTF("All"));
		break;

	case ActionSelectUp:
		env->CallVoidMethod(cls,selectMethodId, env->NewStringUTF("Up"));
		break;

	case ActionSelectLigand:
		env->CallVoidMethod(cls,selectMethodId, env->NewStringUTF("Ligand"));
		break;

	case ActionSelectAllLigands:
		env->CallVoidMethod(cls,selectMethodId, env->NewStringUTF("AllLigands"));
		break;

	case ActionSelectNextLigand:
		env->CallVoidMethod(cls,selectMethodId, env->NewStringUTF("NextLigand"));
		break;

	case ActionSelectProximity:
		env->CallVoidMethod(cls,selectMethodId, env->NewStringUTF("Proximity"));
		break;

	case ActionSelectResidue:
		env->CallVoidMethod(cls,selectMethodId, env->NewStringUTF("Residue"));
		break;

	case ActionSelectChain:
		env->CallVoidMethod(cls,selectMethodId, env->NewStringUTF("Chain"));
		break;

	case ActionSelectSubChain:
		env->CallVoidMethod(cls,selectMethodId, env->NewStringUTF("SubChain"));
		break;

	case ActionDeselect:
		env->CallVoidMethod(cls,selectMethodId, env->NewStringUTF("None"));
		break;

	case ActionSpin:
		env->CallVoidMethod(cls,spinMethodId, false);
		break;

	case ActionStopSpin:
		env->CallVoidMethod(cls,spinMethodId, false);
		break;

	case ActionSpinCrazy:
		env->CallVoidMethod(cls,spinMethodId, true);
		break;

	case ActionCopy:
		env->CallVoidMethod(cls,copyMethodId);
		break;

	case ActionPaste:
		env->CallVoidMethod(cls,pasteMethodId);
		break;

	case ActionHandsOn:
		env->CallVoidMethod(cls,enableHandsMethodId, true);
		break;

	case ActionHandsOff:
		env->CallVoidMethod(cls,enableHandsMethodId, false);
		break;

	case ActionWakeUp:
		env->CallVoidMethod(cls,enableVoiceMethodId, true);
		break;

	case ActionGoToSleep:
		env->CallVoidMethod(cls,enableVoiceMethodId, false);
		break;

	case ActionColorSequenceSimilarity:
		env->CallVoidMethod(cls,colorMethodId, env->NewStringUTF("SequenceSimilarity"));
		break;

	case ActionColorDisplacement:
		env->CallVoidMethod(cls,colorMethodId, env->NewStringUTF("Displacement"));
		break;

	case ActionColorSecondaryStructure:
		env->CallVoidMethod(cls,colorMethodId, env->NewStringUTF("SecondaryStructure"));
		break;

	case ActionColorStructure:
		env->CallVoidMethod(cls,colorMethodId, env->NewStringUTF("Structure"));
		break;

	default:
		env->CallVoidMethod(cls,speechMethodId, action);

	}
	
}

