#include "Hook_OnRemoveEntity.hpp"

#include <EggLockClient/CEggLockClient.hpp>

auto Hook_OnRemoveEntity( CGameEntitySystem* pCGameEntitySystem , CEntityInstance* pInst , CHandle handle ) -> void
{
	GetEggLockClient()->OnRemoveEntity( pInst , handle );

	return OnRemoveEntity_o( pCGameEntitySystem , pInst , handle );
}
