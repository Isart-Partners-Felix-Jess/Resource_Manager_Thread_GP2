#pragma once
#include <fstream>
#include <string>
#include <filesystem>
#include <Windows.h>

//Windows only

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define DEBUG_LOG(...)\
{ \
std::ostringstream debugLogStream; \
debugLogStream <<"DEBUG: "<< __FILENAME__ << "(" << __LINE__ << "): "; \
debugLogStream << __VA_ARGS__ << std::endl; \
Log::GetInstance()->Print(debugLogStream.str().c_str());\
OutputDebugStringA(debugLogStream.str().c_str()); \
}
//log.Print(message);\
//std::string fileNbrMsg = GetFileNameFromPath(__FILE__) + "(" + __LINE__ + "):" <<message;
//OutputDebugStringA(fileNbrMsg.c_str()); }\

class Log
{
private:
	std::ofstream m_Output;
	//Singleton part
	static Log* instance;
public:
	static Log* GetInstance();
	Log();
	Log(Log& other) = delete;
	void operator=(const Log&) = delete;
	~Log();
	//End of Singleton part

	void OpenFile(std::filesystem::path const& filename, bool _erase = false);
	void Print(const char* format, ...);
};