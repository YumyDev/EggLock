#include "Hook_FireEventClientSide.hpp"

#include <EggLockClient/CEggLockClient.hpp>

auto Hook_FireEventClientSide( IGameEventManager2* pGameEventManager2 , IGameEvent* pGameEvent ) -> bool
{
	GetEggLockClient()->OnFireEventClientSide( pGameEvent );

	return FireEventClientSide_o( pGameEventManager2 , pGameEvent );
}
