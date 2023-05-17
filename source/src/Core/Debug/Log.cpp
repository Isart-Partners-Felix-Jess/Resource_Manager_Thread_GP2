#include <Log.hpp>
//#include <stdarg.h>
#include <iostream>
#include <chrono>

//Singleton
Log* Log::instance = nullptr;

void FormatString(char* buffer, size_t bufferSize, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	std::vsnprintf(buffer, bufferSize, format, args);
	va_end(args);
}
Log* Log::GetInstance()
{
	if (instance == nullptr) {
		instance = new Log();
	}
	return instance;
}

void Log::DeleteInstance()
{
	//Useless to check: delete nullptr is safe
	delete instance;
}

Log::Log()
{

}

Log::~Log()
{
	time_t now = time(0);
	char buffer[26];
	ctime_s(buffer, sizeof(buffer), &now);
	GetInstance()->Print("End of log entry at %s", buffer);
if (m_Output.is_open())
	m_Output.close();
}
void Log::OpenFile(std::filesystem::path const& filename, bool _erase)
{
	GetInstance()->InstanceOpenFile(filename,_erase);
}
void Log::InstanceOpenFile(std::filesystem::path const& filename, bool _erase)
{
	if (_erase)
		m_Output.open(filename, std::ios::out | std::ios::trunc);
	else
		m_Output.open(filename, std::ios::out | std::ios::app);
	if (m_Output.is_open())
	{
		time_t now = time(0);
		char buffer[26];
		ctime_s(buffer, sizeof(buffer), &now);
		Print("Log entry :%s", buffer);
	}
	else
		std::cout << "Failed to open " << filename << std::endl;
}
void Log::Print(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	GetInstance()->InstancePrint(format, args);
	va_end(args);
}
void Log::InstancePrint(const char* format, va_list args)
{
	//vsnprintf instead of manually checking the format[i]
	const int bufferSize = 1024;
	char buffer[bufferSize];
	vsnprintf(buffer, bufferSize, format, args);
	std::cout << std::string(buffer) << "\n";
	m_Output << std::string(buffer) << "\n";
	m_Output.flush();
}
void Log::ResetColor()
{
	SetConsoleTextAttribute(handle, 15); // texte in white (default)
}
void Log::WarningColor()
{
	ChangeColor(Color::YELLOWONBLACKBG);
}
void Log::ErrorColor()
{
	ChangeColor(Color::WHITEONREDBG);
}
void Log::ChangeColor(unsigned char _handleWindowsId)
{
	SetConsoleTextAttribute(handle, _handleWindowsId);
}
void Log::ChangeColor(Color _handleWindowsId)
{
	SetConsoleTextAttribute(handle, (WORD)_handleWindowsId);
}
