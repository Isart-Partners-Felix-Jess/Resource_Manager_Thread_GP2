#pragma once
#include <fstream>
#include <filesystem>
#include <Windows.h>

//Windows only

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define DEBUG_LOG(format,...)\
{ \
std::ostringstream debugLogStream; \
debugLogStream /*<<"DEBUG: "*/<< __FILENAME__ << "(" << __LINE__ << "): "; \
const int bufferSize = 1024;\
char buffer[bufferSize];\
FormatString(buffer, bufferSize, format, ##__VA_ARGS__);\
debugLogStream << buffer << std::endl; \
Log::Print(format, ##__VA_ARGS__);\
OutputDebugStringA(debugLogStream.str().c_str()); \
}

void FormatString(char* buffer, size_t bufferSize, const char* format, ...);
static class Log
{
private:
	std::ofstream m_Output;
	//Singleton part /!\ CARE: IT IS NOT THREAD SAFE
	static Log* instance;
	Log();
public:
	static Log* GetInstance();
	static void DeleteInstance();
	Log(Log& other) = delete;
	void operator=(const Log&) = delete;
	~Log();
	//End of Singleton part
	
	//Shortcut to get the instance
	static void OpenFile(std::filesystem::path const& filename, bool _erase = false);
	//Shortcut to get the instance
	static void Print(const char* format, ...);
private :
	void InstanceOpenFile(std::filesystem::path const& filename, bool _erase = false);
	void InstancePrint(const char* format, va_list args);
};