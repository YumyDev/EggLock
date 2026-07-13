#include "CFunctionList.hpp"

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include <HttpClient.hpp>
#include <EggLock/AndromedaClient/Settings/Settings.hpp>

static CFunctionList g_CFunctionList{};

static auto ParseType( const char* szType ) -> eBasePatternSearchType
{
	if ( _stricmp( szType , "PTR" ) == 0 ) return eBasePatternSearchType::SEARCH_TYPE_PTR;
	if ( _stricmp( szType , "CALL" ) == 0 ) return eBasePatternSearchType::SEARCH_TYPE_CALL;
	if ( _stricmp( szType , "PTR2" ) == 0 ) return eBasePatternSearchType::SEARCH_TYPE_PTR2;
	if ( _stricmp( szType , "PROC" ) == 0 ) return eBasePatternSearchType::SEARCH_TYPE_PROC;
	return eBasePatternSearchType::SEARCH_TYPE_NONE;
}

auto CFunctionList::LoadFromJson( const char* szJsonPath ) -> int
{
	std::ifstream Ifs( szJsonPath );
	if ( !Ifs.is_open() )
		return -1;

	rapidjson::IStreamWrapper Isw( Ifs );
	rapidjson::Document Doc;
	Doc.ParseStream( Isw );

	if ( Doc.HasParseError() )
		return -1;

	if ( !Doc.HasMember( "patterns" ) || !Doc["patterns"].IsObject() )
		return -1;

	int nUpdated = 0;

	const auto& Patterns = Doc["patterns"];

	auto TryUpdate = [&]( const char* szDll , const char* szName , CBasePattern& Pattern ) -> void
	{
		if ( Patterns.HasMember( szDll ) && Patterns[szDll].IsObject() )
		{
			const auto& DllObj = Patterns[szDll];
			if ( DllObj.HasMember( szName ) && DllObj[szName].IsObject() )
			{
				const auto& Entry = DllObj[szName];
				const char* szPattern = Entry.HasMember( "pattern" ) ? Entry["pattern"].GetString() : nullptr;
				uint32_t dwOffset = Entry.HasMember( "offset" ) ? Entry["offset"].GetUint() : 0;
				const char* szType = Entry.HasMember( "type" ) ? Entry["type"].GetString() : "NONE";
				const char* szNewDll = Entry.HasMember( "dll" ) ? Entry["dll"].GetString() : szDll;

				if ( szPattern && strlen( szPattern ) > 0 )
				{
					Pattern.UpdatePattern( szPattern , dwOffset , ParseType( szType ) );
					if ( _stricmp( szNewDll , szDll ) != 0 )
						Pattern.UpdateDll( szNewDll );
					nUpdated++;
				}
			}
		}
	};

	TryUpdate( "client.dll" , "CSkeletonInstance_CalcWorldSpaceBones" , CSkeletonInstance_CalcWorldSpaceBones );
	TryUpdate( "client.dll" , "ScreenTransform" , ScreenTransform );
	TryUpdate( "client.dll" , "CCitadelInput_GetViewAngles" , CCitadelInput_GetViewAngles );
	TryUpdate( "client.dll" , "CGameEntitySystem_GetBaseEntity" , CGameEntitySystem_GetBaseEntity );
	TryUpdate( "client.dll" , "CGameEntitySystem_GetHighestEntityIndex" , CGameEntitySystem_GetHighestEntityIndex );
	TryUpdate( "client.dll" , "CGameEntitySystem_GetLocalCitadelPlayerController" , CGameEntitySystem_GetLocalCitadelPlayerController );
	TryUpdate( "client.dll" , "IGameEvent_GetName" , IGameEvent_GetName );
	TryUpdate( "client.dll" , "GetCUserCmdTick" , GetCUserCmdTick );
	TryUpdate( "client.dll" , "GetCUserCmdArray" , GetCUserCmdArray );
	TryUpdate( "client.dll" , "GetCUserCmdBySequenceNumber" , GetCUserCmdBySequenceNumber );
	TryUpdate( "client.dll" , "C_EnvSky_Update" , C_EnvSky_Update );
	TryUpdate( "client.dll" , "C_BaseEntity_GetBoneIdByName" , C_BaseEntity_GetBoneIdByName );
	TryUpdate( "client.dll" , "C_BaseEntity_GetHitBoxSet" , C_BaseEntity_GetHitBoxSet );

	return nUpdated;
}

auto CFunctionList::TryUpdateFromRemote() -> bool
{
	if ( !Settings::Update::Enabled )
		return false;

	if ( !Settings::Update::Url || strlen( Settings::Update::Url ) == 0 )
		return false;

	std::string JsonData;
	if ( !CHttpClient::DownloadString( Settings::Update::Url , JsonData ) )
		return false;

	char szJsonPath[MAX_PATH] = { 0 };
	GetModuleFileNameA( NULL , szJsonPath , MAX_PATH );
	auto pos = std::string( szJsonPath ).find_last_of( '\\' );
	if ( pos == std::string::npos )
		return false;

	std::string sJsonPath( szJsonPath , pos + 1 );
	sJsonPath += "patterns.json";

	std::ofstream ofs( sJsonPath , std::ios::binary );
	if ( !ofs.is_open() )
		return false;

	ofs.write( JsonData.data() , JsonData.size() );
	ofs.close();

	int nUpdated = LoadFromJson( sJsonPath.c_str() );
	return nUpdated > 0;
}

auto CFunctionList::ScanPatterns() -> bool
{
	std::vector<CBasePattern*> vPatterns =
	{
		&CSkeletonInstance_CalcWorldSpaceBones,
		&ScreenTransform,
		&CCitadelInput_GetViewAngles,
		&CGameEntitySystem_GetBaseEntity,
		&CGameEntitySystem_GetHighestEntityIndex,
		&CGameEntitySystem_GetLocalCitadelPlayerController,
		&IGameEvent_GetName,
		&GetCUserCmdTick,
		&GetCUserCmdArray,
		&GetCUserCmdBySequenceNumber,
		&C_EnvSky_Update,
		&C_BaseEntity_GetBoneIdByName,
		&C_BaseEntity_GetHitBoxSet,
	};

	auto Searched = true;

	for ( auto& Pattern : vPatterns )
	{
		if ( !Pattern->Search() )
			Searched = false;
	}

	return Searched;
}

auto CFunctionList::OnInit() -> bool
{
	char szJsonPath[MAX_PATH] = { 0 };
	GetModuleFileNameA( NULL , szJsonPath , MAX_PATH );
	auto pos = std::string( szJsonPath ).find_last_of( '\\' );
	std::string sJsonPath;
	if ( pos != std::string::npos )
	{
		sJsonPath = std::string( szJsonPath , pos + 1 );
		sJsonPath += "patterns.json";
	}

	LoadFromJson( sJsonPath.c_str() );

	bool bOk = ScanPatterns();

	if ( !bOk && Settings::Update::Enabled && Settings::Update::Url && strlen( Settings::Update::Url ) > 0 )
	{
		if ( TryUpdateFromRemote() )
			bOk = ScanPatterns();
	}

	return bOk;
}

auto GetFunctionList() -> CFunctionList*
{
	return &g_CFunctionList;
}
