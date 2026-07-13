#include "Hook_IsRelativeMouseMode.hpp"

#include <EggLockClient/CEggLockGUI.hpp>

auto Hook_IsRelativeMouseMode( CInputSystem* pInputSystem , bool Active ) -> void
{
	GetEggLockGUI()->m_bMainActive = Active;

	if ( GetEggLockGUI()->IsVisible() )
		return IsRelativeMouseMode_o( pInputSystem , false );

	return IsRelativeMouseMode_o( pInputSystem , Active );
}
