#pragma once

#include <string>

class CHttpClient
{
public:
	static auto DownloadString( const char* szUrl , std::string& out ) -> bool;
	static auto DownloadToFile( const char* szUrl , const char* szPath ) -> bool;
};
