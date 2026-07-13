#include "Hook_MouseInputEnabled.hpp"

#include <EggLockClient/CEggLockGUI.hpp>

auto Hook_MouseInputEnabled( CCitadelInput* pCCitadelInput ) -> bool
{
	if ( GetEggLockGUI()->IsVisible() )
		return false;

	return MouseInputEnabled_o( pCCitadelInput );
}
