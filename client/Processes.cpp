#include "Processes.h"
#include <sstream>

std::string Processes::GetProcessNameAndID(DWORD processID)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    // Get a handle to the process.
    
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, processID);

    // Get the process name.

    if (NULL != hProcess)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
            &cbNeeded))
        {
            GetModuleBaseName(hProcess, hMod, szProcessName,
                sizeof(szProcessName) / sizeof(TCHAR));
        }
    }

    // Print the process name and identifier.

    //_tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);

    CloseHandle(hProcess);

    return szProcessName;
}


std::string Processes::Get_processes() {

    std::string output = "";
    DWORD aProcesses[1024];
    DWORD cbNeeded = 0;
    DWORD  cProcesses = 0;
    unsigned int i = 0;

    // Calculate how many process identifiers were returned.

    EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);

    cProcesses = cbNeeded / sizeof(DWORD);



    for (i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            //TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
            //aProcessesChar[i] = GetProcessNameAndID(aProcesses[i]);
            std::stringstream str;
            str << aProcesses[i];
            if (GetProcessNameAndID(aProcesses[i])!= "<unknown>")
            output += '\n'+ GetProcessNameAndID(aProcesses[i]) + ' ' + str.str();
        }
    }

    return output;
}

Processes::Processes() {}