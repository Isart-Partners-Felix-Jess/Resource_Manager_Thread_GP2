#pragma once

#include <fstream>
#include <filesystem>

#define NOMINMAX
#include <Windows.h>

// Windows only /!\

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define DEBUG_LOG(format,...)\
{\
std::ostringstream debugLogStream; \
debugLogStream << __FILENAME__ << "(" << __LINE__ << "): "; \
const int bufferSize = 1024;\
char buffer[bufferSize];\
FormatString(buffer, bufferSize, format, ##__VA_ARGS__);\
debugLogStream << buffer << std::endl; \
Log::Print(format, ##__VA_ARGS__);\
OutputDebugStringA(debugLogStream.str().c_str());\
}

#define DEBUG_WARNING(format,...)\
{\
Log::WarningColor();\
std::ostringstream WarningLogStream;\
WarningLogStream <<"Warning: " << format ;\
DEBUG_LOG(WarningLogStream.str().c_str(), ##__VA_ARGS__)\
Log::ResetColor();\
}

#define DEBUG_ERROR(format,...)\
{\
Log::ErrorColor();\
std::ostringstream ERRORLogStream;\
ERRORLogStream <<"ERROR: " << format ;\
DEBUG_LOG(ERRORLogStream.str().c_str(), ##__VA_ARGS__)\
Log::ResetColor();\
}

void FormatString(char* _buffer, size_t _bufferSize, const char* _format, ...);

class Log
{
private:
	std::ofstream m_output;
	HANDLE m_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	// Singleton /!\ BE CAREFUL: IT IS NOT THREAD SAFE
	static Log* m_instance;
	Log() {};

public:
	static Log* GetInstance();
	static void DeleteInstance();

	Log(Log& other) = delete;
	~Log();

	void operator=(const Log&) = delete;

	// Shortcut to get the instance
	static void OpenFile(std::filesystem::path const& _filename, bool _erase = false);
	// Shortcut to get the instance
	static void Print(const char* _format, ...);
	static void WarningColor();
	static void ErrorColor();
	static void ResetColor();
	static void SuccessColor();

private:
	void InstanceOpenFile(std::filesystem::path const& _filename, bool _erase = false);
	void InstancePrint(const char* _format, va_list _args);
	void ChangeColor(unsigned char _handleWindowsId) const;
	enum class Color : unsigned char
	{
		DEFAULT = 15,
		YELLOWONBLACKBG = 14,
		GREENONBLACKBG = 10,
		// RED  BG   WHITE OFFSET
		WHITEONREDBG = (12 * 16 + 15)
	};
	void ChangeColor(Color _handleWindowsId) const;
};