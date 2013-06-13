//------------------------------------------------------------------------------
// <copyright file="SpeechBasics.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpeechBasics.h"
#define INITGUID
#include <guiddef.h>

// Static initializers
LPCWSTR CSpeechBasics::GrammarFileName = L"molecularcontrol-kinect.grxml";

/// <summary>
/// Constructor
/// </summary>
CSpeechBasics::CSpeechBasics() :
//    m_pNuiSensor(NULL),
    m_pKinectAudioStream(NULL),
    m_pSpeechStream(NULL),
    m_pSpeechRecognizer(NULL),
    m_pSpeechContext(NULL),
    m_pSpeechGrammar(NULL),
    m_hSpeechEvent(INVALID_HANDLE_VALUE)
{
}

/// <summary>
/// Destructor
/// </summary>
CSpeechBasics::~CSpeechBasics()
{
	if (NULL != m_pKinectAudioStream)
	{
		m_pKinectAudioStream->StopCapture();
	}

	if (NULL != m_pSpeechRecognizer)
	{
		m_pSpeechRecognizer->SetRecoState(SPRST_INACTIVE);
	}


//    if (m_pNuiSensor)
//    {
//        m_pNuiSensor->NuiShutdown();
//    }

//    SafeRelease(m_pNuiSensor);
    SafeRelease(m_pKinectAudioStream);
    SafeRelease(m_pSpeechStream);
    SafeRelease(m_pSpeechRecognizer);
    SafeRelease(m_pSpeechContext);
    SafeRelease(m_pSpeechGrammar);
}

/// <summary>
/// Creates the main window and begins processing
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
int CSpeechBasics::Run(HINSTANCE hInstance, int nCmdShow)
{
    MSG       msg = {0};

    const int eventCount = 1;
    HANDLE hEvents[eventCount];

    // Main message loop
    while (WM_QUIT != msg.message)
    {
        hEvents[0] = m_hSpeechEvent;

        // Check to see if we have either a message (by passing in QS_ALLINPUT)
        // Or a speech event (hEvents)
        DWORD dwEvent = MsgWaitForMultipleObjectsEx(eventCount, hEvents, INFINITE, QS_ALLINPUT, MWMO_INPUTAVAILABLE);

        // Check if this is an event we're waiting on and not a timeout or message
        if (WAIT_OBJECT_0 == dwEvent)
        {
            ProcessSpeech();
        }

        if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {

            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}



/// <summary>
/// Create the first connected Kinect found.
/// </summary>
/// <returns>S_OK on success, otherwise failure code.</returns>
HRESULT CSpeechBasics::CreateFirstConnected()
{
    HRESULT hr;

    hr = InitializeAudioStream();
    if (FAILED(hr))
    {
        fprintf(stderr, "Could not initialize audio stream.");
        return hr;
    }

    hr = CreateSpeechRecognizer();
    if (FAILED(hr))
    {
        fprintf(stderr, "Could not create speech recognizer. Please ensure that Microsoft Speech SDK and other sample requirements are installed.");
        return hr;
    }

    hr = LoadSpeechGrammar();
    if (FAILED(hr))
    {
        fprintf(stderr, "Could not load speech grammar. Please ensure that grammar configuration file was properly deployed.");
        return hr;
    }

    hr = StartSpeechRecognition();
    if (FAILED(hr))
    {
        fprintf(stderr, "Could not start recognizing speech.");
        return hr;
    }

    return hr;
}

/// <summary>
/// Initialize Kinect audio stream object.
/// </summary>
/// <returns>
/// <para>S_OK on success, otherwise failure code.</para>
/// </returns>
HRESULT CSpeechBasics::InitializeAudioStream()
{
    INuiAudioBeam*      pNuiAudioSource = NULL;
    IMediaObject*       pDMO = NULL;
    IPropertyStore*     pPropertyStore = NULL;
    IStream*            pStream = NULL;

    // Get the audio source
    HRESULT hr = NuiGetAudioSource(&pNuiAudioSource);
    if (SUCCEEDED(hr))
    {
        hr = pNuiAudioSource->QueryInterface(IID_IMediaObject, (void**)&pDMO);

        if (SUCCEEDED(hr))
        {
            hr = pNuiAudioSource->QueryInterface(IID_IPropertyStore, (void**)&pPropertyStore);
    
            // Set AEC-MicArray DMO system mode. This must be set for the DMO to work properly.
            // Possible values are:
            //   SINGLE_CHANNEL_AEC = 0
            //   OPTIBEAM_ARRAY_ONLY = 2
            //   OPTIBEAM_ARRAY_AND_AEC = 4
            //   SINGLE_CHANNEL_NSAGC = 5
            PROPVARIANT pvSysMode;
            PropVariantInit(&pvSysMode);
            pvSysMode.vt = VT_I4;
            pvSysMode.lVal = (LONG)(2); // Use OPTIBEAM_ARRAY_ONLY setting. Set OPTIBEAM_ARRAY_AND_AEC instead if you expect to have sound playing from speakers.
            pPropertyStore->SetValue(MFPKEY_WMAAECMA_SYSTEM_MODE, pvSysMode);
            PropVariantClear(&pvSysMode);

            // Set DMO output format
            WAVEFORMATEX wfxOut = {AudioFormat, AudioChannels, AudioSamplesPerSecond, AudioAverageBytesPerSecond, AudioBlockAlign, AudioBitsPerSample, 0};
            DMO_MEDIA_TYPE mt = {0};
            MoInitMediaType(&mt, sizeof(WAVEFORMATEX));
    
            mt.majortype = MEDIATYPE_Audio;
            mt.subtype = MEDIASUBTYPE_PCM;
            mt.lSampleSize = 0;
            mt.bFixedSizeSamples = TRUE;
            mt.bTemporalCompression = FALSE;
            mt.formattype = FORMAT_WaveFormatEx;	
            memcpy(mt.pbFormat, &wfxOut, sizeof(WAVEFORMATEX));
    
            hr = pDMO->SetOutputType(0, &mt, 0);

            if (SUCCEEDED(hr))
            {
                m_pKinectAudioStream = new KinectAudioStream(pDMO);

                hr = m_pKinectAudioStream->QueryInterface(IID_IStream, (void**)&pStream);

                if (SUCCEEDED(hr))
                {
                    hr = CoCreateInstance(CLSID_SpStream, NULL, CLSCTX_INPROC_SERVER, __uuidof(ISpStream), (void**)&m_pSpeechStream);

                    if (SUCCEEDED(hr))
                    {
                        hr = m_pSpeechStream->SetBaseStream(pStream, SPDFID_WaveFormatEx, &wfxOut);
                    }
                }
            }

            MoFreeMediaType(&mt);
        }
    }

    SafeRelease(pStream);
    SafeRelease(pPropertyStore);
    SafeRelease(pDMO);
    SafeRelease(pNuiAudioSource);

    return hr;
}

/// <summary>
/// Create speech recognizer that will read Kinect audio stream data.
/// </summary>
/// <returns>
/// <para>S_OK on success, otherwise failure code.</para>
/// </returns>
HRESULT CSpeechBasics::CreateSpeechRecognizer()
{
    ISpObjectToken *pEngineToken = NULL;
    
    HRESULT hr = CoCreateInstance(CLSID_SpInprocRecognizer, NULL, CLSCTX_INPROC_SERVER, __uuidof(ISpRecognizer), (void**)&m_pSpeechRecognizer);

    if (SUCCEEDED(hr))
    {
        m_pSpeechRecognizer->SetInput(m_pSpeechStream, FALSE);
        hr = SpFindBestToken(SPCAT_RECOGNIZERS,L"Language=409;Kinect=True",NULL,&pEngineToken);

        if (SUCCEEDED(hr))
        {
            m_pSpeechRecognizer->SetRecognizer(pEngineToken);
            hr = m_pSpeechRecognizer->CreateRecoContext(&m_pSpeechContext);
        }
    }

    SafeRelease(pEngineToken);

    return hr;
}

/// <summary>
/// Load speech recognition grammar into recognizer.
/// </summary>
/// <returns>
/// <para>S_OK on success, otherwise failure code.</para>
/// </returns>
HRESULT CSpeechBasics::LoadSpeechGrammar()
{
    HRESULT hr = m_pSpeechContext->CreateGrammar(1, &m_pSpeechGrammar);

    if (SUCCEEDED(hr))
    {
        // Populate recognition grammar from file
        hr = m_pSpeechGrammar->LoadCmdFromFile(GrammarFileName, SPLO_STATIC);
    }

    return hr;
}

/// <summary>
/// Start recognizing speech asynchronously.
/// </summary>
/// <returns>
/// <para>S_OK on success, otherwise failure code.</para>
/// </returns>
HRESULT CSpeechBasics::StartSpeechRecognition()
{
    HRESULT hr = m_pKinectAudioStream->StartCapture();

    if (SUCCEEDED(hr))
    {
        // Specify that all top level rules in grammar are now active
        m_pSpeechGrammar->SetRuleState(NULL, NULL, SPRS_ACTIVE);

        // Specify that engine should always be reading audio
        m_pSpeechRecognizer->SetRecoState(SPRST_ACTIVE_ALWAYS);

        // Specify that we're only interested in receiving recognition events
        m_pSpeechContext->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION));

        // Ensure that engine is recognizing speech and not in paused state
        hr = m_pSpeechContext->Resume(0);
        if (SUCCEEDED(hr))
        {
            m_hSpeechEvent = m_pSpeechContext->GetNotifyEventHandle();
        }
    }
        
    return hr;
}

/// <summary>
/// Process recently triggered speech recognition events.
/// </summary>
void CSpeechBasics::ProcessSpeech()
{
    const float ConfidenceThreshold = 0.3f;

    SPEVENT curEvent;
    ULONG fetched = 0;
    HRESULT hr = S_OK;

    m_pSpeechContext->GetEvents(1, &curEvent, &fetched);

    while (fetched > 0)
    {
        switch (curEvent.eEventId)
        {
            case SPEI_RECOGNITION:
                if (SPET_LPARAM_IS_OBJECT == curEvent.elParamType)
                {
                    // this is an ISpRecoResult
                    ISpRecoResult* result = reinterpret_cast<ISpRecoResult*>(curEvent.lParam);
                    SPPHRASE* pPhrase = NULL;
                    
                    hr = result->GetPhrase(&pPhrase);
                    if (SUCCEEDED(hr))
                    {
                        if ((pPhrase->pProperties != NULL) && (pPhrase->pProperties->pFirstChild != NULL))
                        {
                            const SPPHRASEPROPERTY* pSemanticTag = pPhrase->pProperties->pFirstChild;
                            if (pSemanticTag->SREngineConfidence > ConfidenceThreshold)
                            {
								wprintf( L"Found text for rule : %s, %f", pPhrase->Rule.pszName, pSemanticTag->SREngineConfidence) ;
								if (wcscmp(pPhrase->Rule.pszName,L"AminoAcids") == 0)
//								if (pPhrase->Rule.ulId  == 2)
								{
									processSpeechSearch(pSemanticTag->pszValue);
								}
								else 
								{
									AquariaAction action = MapSpeechTagToAction(pSemanticTag->pszValue);
									HRESULT ruleResult;
									switch (action) 
									{
									case ActionSearch:
										OutputDebugString( L"Searching action initiated\n") ;
//										m_pSpeechGrammar->SetRuleState(NULL, NULL, SPRS_INACTIVE);
										ruleResult = m_pSpeechGrammar->SetRuleState(L"AminoAcids", NULL, SPRS_ACTIVE);
//										fprintf(stderr, "Error for rule state: %d",ruleResult ) ;
//										m_pSpeechGrammar->SetRuleIdState(2, SPRS_ACTIVE);
										break;

									default:
										processSpeech(action);

									}
								}


//                                m_pTurtleController->DoAction(action);
                            }
                        }
                        ::CoTaskMemFree(pPhrase);
                    }
                }
                break;
        }

        m_pSpeechContext->GetEvents(1, &curEvent, &fetched);
    }

    return;
}

/// <summary>
/// Maps a specified speech semantic tag to the corresponding action to be performed on turtle.
/// </summary>
/// <returns>
/// Action that matches <paramref name="pszSpeechTag"/>, or TurtleActionNone if no matches were found.
/// </returns>
AquariaAction CSpeechBasics::MapSpeechTagToAction(LPCWSTR pszSpeechTag)
{
    struct SpeechTagToAction
    {
        LPCWSTR pszSpeechTag;
        AquariaAction action;
    };
    const SpeechTagToAction Map[] =
    {
         {L"RESET", ActionReset},
        {L"LOAD", ActionLoad},
        {L"SHOW", ActionShow},
        {L"ZOOM", ActionZoom},
        {L"SEARCH", ActionSearch},
        {L"SELECT_ALL", ActionSelectAll},
        {L"SELECT_UP", ActionSelectUp},
        {L"SELECT_LIGAND", ActionSelectLigand},
		{L"SELECT_ALL_LIGANDS", ActionSelectAllLigands},	
        {L"SELECT_NEXT_LIGAND", ActionSelectNextLigand},
        {L"SELECT_PROXIMITY", ActionSelectProximity},
        {L"SELECT_RESIDUE", ActionSelectResidue},
        {L"SELECT_CHAIN", ActionSelectChain},
        {L"SELECT_SUB_CHAIN", ActionSelectSubChain},
        {L"DESELECT", ActionDeselect},
        {L"SPIN", ActionSpin},
        {L"STOP_SPIN", ActionStopSpin},
        {L"SPIN_CRAZY", ActionSpinCrazy},
        {L"COPY", ActionCopy},
        {L"PASTE", ActionPaste},
        {L"HANDS_ON", ActionHandsOn},
        {L"HANDS_OFF", ActionHandsOff},
        {L"WAKE_UP", ActionWakeUp},
        {L"GO_TO_SLEEP", ActionGoToSleep},
		{L"COLOR_SEQUENCE_SIMILARITY", ActionColorSequenceSimilarity},
		{L"COLOR_DISPLACEMENT", ActionColorDisplacement},
		{L"COLOR_SECONDARY_STRUCTURE", ActionColorSecondaryStructure},
		{L"COLOR_STRUCTURE", ActionColorStructure}
    };

    AquariaAction action = ActionNone;

    for (int i = 0; i < _countof(Map); ++i)
    {
        if (0 == wcscmp(Map[i].pszSpeechTag, pszSpeechTag))
        {
            action = Map[i].action;
            break;
        }
    }
//	printf("Looking up: %s\n",action) ;

    return action;
}
