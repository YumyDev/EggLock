#include "HttpClient.hpp"

#include <Windows.h>
#include <winhttp.h>
#include <fstream>
#pragma comment(lib, "winhttp.lib")

auto CHttpClient::DownloadString( const char* szUrl , std::string& out ) -> bool
{
	wchar_t szHost[256] = { 0 };
	wchar_t szPath[1024] = { 0 };
	wchar_t szFullUrl[2048] = { 0 };

	MultiByteToWideChar( CP_UTF8 , 0 , szUrl , -1 , szFullUrl , 2048 );

	URL_COMPONENTS UrlComp = { 0 };
	UrlComp.dwStructSize = sizeof( UrlComp );
	UrlComp.lpszHostName = szHost;
	UrlComp.dwHostNameLength = 256;
	UrlComp.lpszUrlPath = szPath;
	UrlComp.dwUrlPathLength = 1024;

	if ( !WinHttpCrackUrl( szFullUrl , 0 , 0 , &UrlComp ) )
		return false;

	HINTERNET hSession = WinHttpOpen( L"EggLock/1.0" ,
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY ,
		WINHTTP_NO_PROXY_NAME ,
		WINHTTP_NO_PROXY_BYPASS , 0 );
	if ( !hSession ) return false;

	HINTERNET hConnect = WinHttpConnect( hSession , szHost ,
		UrlComp.nPort , 0 );
	if ( !hConnect ) { WinHttpCloseHandle( hSession ); return false; }

	DWORD dwFlags = ( UrlComp.nScheme == INTERNET_SCHEME_HTTPS ) ?
		WINHTTP_FLAG_REFRESH : 0;

	HINTERNET hRequest = WinHttpOpenRequest( hConnect , L"GET" ,
		szPath , NULL , WINHTTP_NO_REFERER ,
		WINHTTP_DEFAULT_ACCEPT_TYPES , dwFlags );
	if ( !hRequest ) { WinHttpCloseHandle( hConnect ); WinHttpCloseHandle( hSession ); return false; }

	if ( UrlComp.nScheme == INTERNET_SCHEME_HTTPS )
	{
		DWORD dwSecFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
			SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
			SECURITY_FLAG_IGNORE_CERT_CN_INVALID |
			SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE;
		WinHttpSetOption( hRequest , WINHTTP_OPTION_SECURITY_FLAGS ,
			&dwSecFlags , sizeof( dwSecFlags ) );
	}

	if ( !WinHttpSendRequest( hRequest , WINHTTP_NO_ADDITIONAL_HEADERS , 0 ,
		WINHTTP_NO_REQUEST_DATA , 0 , 0 , 0 ) )
	{
		WinHttpCloseHandle( hRequest );
		WinHttpCloseHandle( hConnect );
		WinHttpCloseHandle( hSession );
		return false;
	}

	if ( !WinHttpReceiveResponse( hRequest , NULL ) )
	{
		WinHttpCloseHandle( hRequest );
		WinHttpCloseHandle( hConnect );
		WinHttpCloseHandle( hSession );
		return false;
	}

	out.clear();
	char szBuffer[4096] = { 0 };
	DWORD dwBytesRead = 0;

	while ( WinHttpReadData( hRequest , szBuffer , sizeof( szBuffer ) - 1 , &dwBytesRead ) && dwBytesRead > 0 )
	{
		szBuffer[dwBytesRead] = '\0';
		out += szBuffer;
		dwBytesRead = 0;
	}

	WinHttpCloseHandle( hRequest );
	WinHttpCloseHandle( hConnect );
	WinHttpCloseHandle( hSession );

	return !out.empty();
}

auto CHttpClient::DownloadToFile( const char* szUrl , const char* szPath ) -> bool
{
	std::string Data;
	if ( !DownloadString( szUrl , Data ) )
		return false;

	std::ofstream ofs( szPath , std::ios::binary );
	if ( !ofs.is_open() )
		return false;

	ofs.write( Data.data() , Data.size() );
	return ofs.good();
}
