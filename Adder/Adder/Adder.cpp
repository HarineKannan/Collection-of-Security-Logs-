#define _WIN32_DCOM
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include <vector> 
#include "Adder.h"
#include <windows.h> 
#include<jni.h>

using namespace std;

#pragma comment(lib, "wbemuuid.lib")

JNIEXPORT jint JNICALL Java_Adder_add(JNIEnv* env, jobject obj) {
    
    HRESULT hres;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
        cout << "Failed to initialize COM library" << endl;
        return 1;
    }

    hres = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL);

    if (FAILED(hres))
    {
        cout << "Failed to initialize security." << endl;
        CoUninitialize();
        return 1;
    }
    IWbemLocator* pLoc = nullptr;

    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (LPVOID*)&pLoc);

    if (FAILED(hres))
    {
        cout << "Failed to create IWbemLocator object" << endl;
        CoUninitialize();
        return 1;
    }

    IWbemServices* pSvc = nullptr;

    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        nullptr,
        nullptr,
        0,
        0,
        0,
        0,
        &pSvc);

    if (FAILED(hres))
    {
        cout << "Could not connect" << endl;
        pLoc->Release();
        CoUninitialize();
        return 1;
    }


    hres = CoSetProxyBlanket(
        pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE);

    if (FAILED(hres))
    {
        cout << "Could not set proxy blanket" << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    IEnumWbemClassObject* pEnumerator = nullptr;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_NTLogEvent WHERE Logfile = 'Security'"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres))
    {
        cout << "Query for security events failed." << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    if (!pEnumerator)
    {
        cout << "Enumerator is null." << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    IWbemClassObject* pclsObj = nullptr;
    ULONG uReturn = 0;
    int count = 0;
    while (pEnumerator && count < 10)
    {
        hres = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

        if (0 == uReturn)
        {
            break;
        }

        VARIANT vtProp;

        hres = pclsObj->Get(L"TimeGenerated", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hres))
        {
            std::wcout << L"TimeGenerated: " << vtProp.bstrVal << std::endl;
            VariantClear(&vtProp);
        }

        hres = pclsObj->Get(L"EventCode", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hres))
        {
            std::wcout << L"EventCode: " << vtProp.uintVal << std::endl;
            VariantClear(&vtProp);
        }

        hres = pclsObj->Get(L"EventType", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hres))
        {
            std::wcout << L"EventType: " << vtProp.uintVal << std::endl;
            VariantClear(&vtProp);
        }

        hres = pclsObj->Get(L"SourceName", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hres))
        {
            std::wcout << L"SourceName: " << vtProp.bstrVal << std::endl;
            VariantClear(&vtProp);
        }

        hres = pclsObj->Get(L"Message", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hres))
        {
            std::wcout << L"Message: " << vtProp.bstrVal << std::endl;
            VariantClear(&vtProp);
        }

        pclsObj->Release();
        pclsObj = nullptr;
        count++;
    }

    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();
    return 0;

    //HRESULT hres;

    //hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    //if (FAILED(hres))
    //{
    //    cout << "Failed to initialize COM library" << endl;
    //    return NULL;
    //}

    //hres = CoInitializeSecurity(
    //    NULL,
    //    -1,
    //    NULL,
    //    NULL,
    //    RPC_C_AUTHN_LEVEL_DEFAULT,
    //    RPC_C_IMP_LEVEL_IMPERSONATE,
    //    NULL,
    //    EOAC_NONE,
    //    NULL);

    //if (FAILED(hres))
    //{
    //    cout << "Failed to initialize security." << endl;
    //    CoUninitialize();
    //    return NULL;
    //}

    //IWbemLocator* pLoc = nullptr;

    //hres = CoCreateInstance(
    //    CLSID_WbemLocator,
    //    0,
    //    CLSCTX_INPROC_SERVER,
    //    IID_IWbemLocator,
    //    (LPVOID*)&pLoc);

    //if (FAILED(hres))
    //{
    //    cout << "Failed to create IWbemLocator object" << endl;
    //    CoUninitialize();
    //    return NULL;
    //}

    //IWbemServices* pSvc = nullptr;

    //hres = pLoc->ConnectServer(
    //    _bstr_t(L"ROOT\\CIMV2"),
    //    nullptr,
    //    nullptr,
    //    nullptr, // Pass nullptr instead of 0
    //    0,
    //    nullptr,
    //    nullptr,
    //    &pSvc);



    //if (FAILED(hres))
    //{
    //    cout << "Could not connect" << endl;
    //    pLoc->Release();
    //    CoUninitialize();
    //    return NULL;
    //}


    //hres = CoSetProxyBlanket(
    //    pSvc,
    //    RPC_C_AUTHN_WINNT,
    //    RPC_C_AUTHZ_NONE,
    //    NULL,
    //    RPC_C_AUTHN_LEVEL_CALL,
    //    RPC_C_IMP_LEVEL_IMPERSONATE,
    //    NULL,
    //    EOAC_NONE);

    //if (FAILED(hres))
    //{
    //    cout << "Could not set proxy blanket" << endl;
    //    pSvc->Release();
    //    pLoc->Release();
    //    CoUninitialize();
    //    return NULL;
    //}

    //IEnumWbemClassObject* pEnumerator = nullptr;
    //hres = pSvc->ExecQuery(
    //    bstr_t("WQL"),
    //    bstr_t("SELECT * FROM Win32_NTLogEvent WHERE Logfile = 'Security'"),
    //    WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
    //    NULL,
    //    &pEnumerator);

    //if (FAILED(hres))
    //{
    //    cout << "Query for security events failed." << endl;
    //    pSvc->Release();
    //    pLoc->Release();
    //    CoUninitialize();
    //    return NULL;
    //}

    //if (!pEnumerator)
    //{
    //    cout << "Enumerator is null." << endl;
    //    pSvc->Release();
    //    pLoc->Release();
    //    CoUninitialize();
    //    return NULL;
    //}

    //IWbemClassObject* pclsObj = nullptr;
    //ULONG uReturn = 0;
    //int count = 0;

    //// Create a vector to store jobject instances
    //vector<jobject> logEntries;

    //// Get jclass of the Java class LogEntry
    //jclass logEntryClass = env->FindClass("LogEntry");
    //jmethodID constructor = env->GetMethodID(logEntryClass, "<init>", "(Ljava/lang/String;II)V");

    //while (pEnumerator && count < 10)
    //{
    //    hres = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

    //    if (0 == uReturn)
    //    {
    //        break;
    //    }
    //    int eventType = 0;
    //    int eventCode = 0;

    //    wstring timeGenerated;
    //    VARIANT vtProp;
    //    hres = pclsObj->Get(L"TimeGenerated", 0, &vtProp, 0, 0);
    //    if (SUCCEEDED(hres))
    //    {
    //        timeGenerated = vtProp.bstrVal;
    //        VariantClear(&vtProp);
    //    }

    //    hres = pclsObj->Get(L"EventCode", 0, &vtProp, 0, 0);
    //    if (SUCCEEDED(hres))
    //    {
    //        eventCode = vtProp.uintVal;
    //        VariantClear(&vtProp);
    //    }

    //    hres = pclsObj->Get(L"EventType", 0, &vtProp, 0, 0);
    //    if (SUCCEEDED(hres))
    //    {
    //         eventType = vtProp.uintVal;
    //        VariantClear(&vtProp);
    //    }

    //    // Create a jobject using the constructor method
    //    jobject logEntry = env->NewObject(logEntryClass, constructor, timeGenerated, eventType, eventCode);

    //    // Add the created jobject to the vector
    //    logEntries.push_back(logEntry);

    //    pclsObj->Release();
    //    pclsObj = nullptr;
    //    count++;
    //}

    //// Convert the vector to a jobjectArray
    //jobjectArray resultArray = env->NewObjectArray(logEntries.size(), logEntryClass, NULL);
    //for (size_t i = 0; i < logEntries.size(); ++i) {
    //    env->SetObjectArrayElement(resultArray, i, logEntries[i]);
    //}

    //pSvc->Release();
    //pLoc->Release();
    //pEnumerator->Release();
    //CoUninitialize();

    //return resultArray;
}
