#include <Log.hpp>

#include <iostream>
#include <chrono>

//Singleton
Log* Log::m_instance = nullptr;

void FormatString(char* _buffer, size_t _bufferSize, const char* _format, ...)
{
	va_list args;
	va_start(args, _format);
	std::vsnprintf(_buffer, _bufferSize, _format, args);
	va_end(args);
}

Log* Log::GetInstance()
{
	if (m_instance == nullptr)
		m_instance = new Log();
	return m_instance;
}

void Log::DeleteInstance()
{
	// Useless to check: delete nullptr is safe
	delete m_instance;
}

Log::~Log()
{
	time_t now = time(0);
	char buffer[26];
	ctime_s(buffer, sizeof(buffer), &now);
	GetInstance()->Print("End of log entry at %s", buffer);
	if (m_output.is_open())
		m_output.close();
}

void Log::OpenFile(std::filesystem::path const& _filename, bool _erase) {
	GetInstance()->InstanceOpenFile(_filename, _erase);
}

void Log::InstanceOpenFile(std::filesystem::path const& _filename, bool _erase)
{
	if (_erase)
		m_output.open(_filename, std::ios::out | std::ios::trunc);
	else
		m_output.open(_filename, std::ios::out | std::ios::app);
	if (m_output.is_open())
	{
		time_t now = time(0);
		char buffer[26];
		ctime_s(buffer, sizeof(buffer), &now);
		Print("Log entry :%s", buffer);
	}
	else
		std::cout << "Failed to open " << _filename << std::endl;
}

void Log::Print(const char* _format, ...)
{
	va_list args;
	va_start(args, _format);
	GetInstance()->InstancePrint(_format, args);
	va_end(args);
}

void Log::InstancePrint(const char* _format, va_list _args)
{
	//vsnprintf instead of manually checking the format[i]
	const int bufferSize = 1024;
	char buffer[bufferSize];
	vsnprintf(buffer, bufferSize, _format, _args);
	std::cout << std::string(buffer) << "\n";
	m_output << std::string(buffer) << "\n";
	m_output.flush();
}

void Log::ResetColor() { // Text in white (default)
	SetConsoleTextAttribute(GetInstance()->m_handle, 15);
}

void Log::SuccessColor() {
	GetInstance()->ChangeColor(Color::GREENONBLACKBG);
}

void Log::WarningColor() {
	GetInstance()->ChangeColor(Color::YELLOWONBLACKBG);
}

void Log::ErrorColor() {
	GetInstance()->ChangeColor(Color::WHITEONREDBG);
}

void Log::ChangeColor(unsigned char _handleWindowsId) const {
	SetConsoleTextAttribute(m_handle, _handleWindowsId);
}

void Log::ChangeColor(Color _handleWindowsId) const {
	SetConsoleTextAttribute(m_handle, (WORD)_handleWindowsId);
}