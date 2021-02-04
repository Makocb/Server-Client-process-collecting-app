#pragma once

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <iostream>
#include <string>

class Processes
{
private:
	std::string GetProcessNameAndID(DWORD processID);
public:
	Processes();
	std::string Get_processes();
};

