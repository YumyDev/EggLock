#include "Hook_OnAddEntity.hpp"

#include <EggLockClient/CEggLockClient.hpp>

auto Hook_OnAddEntity( CGameEntitySystem* pCGameEntitySystem , CEntityInstance* pInst , CHandle handle ) -> void
{
	GetEggLockClient()->OnAddEntity( pInst , handle );

	return OnAddEntity_o( pCGameEntitySystem , pInst , handle );
}
