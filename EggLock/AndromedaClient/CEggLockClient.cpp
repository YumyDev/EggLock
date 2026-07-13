#include "CEggLockClient.hpp"
#include "CEggLockGUI.hpp"

#include "Fonts/CFontManager.hpp"

#include <DeadLock/SDK/SDK.hpp>
#include <DeadLock/SDK/Interface/IEngineToClient.hpp>

#include <GameClient/CEntityCache/CEntityCache.hpp>

#include <EggLockClient/Features/CVisual/CVisual.hpp>

#include <EggLockClient/GUI/CEggLockMenu.hpp>
#include <EggLockClient/Settings/CSettingsJson.hpp>
#include <EggLockClient/Render/CRenderStackSystem.hpp>

static CEggLockClient g_CEggLockClient{};

auto CEggLockClient::OnInit() -> void
{
	GetEggLockMenu()->InitColors();
	GetEggLockMenu()->SetConfigSelected( GetSettingsJson()->GetConfigLoadedIndex() );
}

auto CEggLockClient::OnFireEventClientSide( IGameEvent* pGameEvent ) -> void
{

}

auto CEggLockClient::OnAddEntity( CEntityInstance* pInst , CHandle handle ) -> void
{
	GetEntityCache()->OnAddEntity( pInst , handle );
}

auto CEggLockClient::OnRemoveEntity( CEntityInstance* pInst , CHandle handle ) -> void
{
	GetEntityCache()->OnRemoveEntity( pInst , handle );
}

auto CEggLockClient::OnStartSound( const Vector3& Pos , const int SourceEntityIndex , const char* szSoundName ) -> void
{
	GetVisual()->OnStartSound( Pos , SourceEntityIndex , szSoundName );
}

auto CEggLockClient::OnClientOutput() -> void
{
	if ( SDK::Interfaces::EngineToClient()->IsInGame() )
		GetVisual()->OnClientOutput();
}

auto CEggLockClient::OnRender() -> void
{
	if ( GetEggLockGUI()->IsVisible() )
		GetEggLockMenu()->OnRenderMenu();

	GetFontManager()->FirstInitFonts();
	GetFontManager()->m_VerdanaFont.DrawString( 1 , 1 , ImColor( 1.f , 1.f , 0.f ) , FW1_LEFT , XorStr( CHEAT_NAME ) );

	GetRenderStackSystem()->OnRenderStack();
}

auto CEggLockClient::OnCreateMove( CCitadelInput* pCitadelInput , CUserCmd* pUserCmd ) -> void
{

}

auto GetEggLockClient() -> CEggLockClient*
{
	return &g_CEggLockClient;
}
