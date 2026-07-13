#pragma once

#include <Common/Common.hpp>

#include <DeadLock/SDK/FunctionListSDK.hpp>

class C_BaseEntity;
class CCitadelPlayerController;

struct CEntityIdentity
{
	void* m_pEntity;
};

class CGameEntitySystem
{
public:
	auto GetBaseEntity( int index ) -> C_BaseEntity*
	{
		if ( (unsigned int)index > 0x7FFE )
			return nullptr;

		if ( (unsigned int)( index >> 9 ) > 0x3F )
			return nullptr;

		auto chunks = *(uintptr_t*)( (uintptr_t)this + 8i64 * ( index >> 9 ) + 16 );
		if ( !chunks )
			return nullptr;

		auto identity = 120i64 * ( index & 0x1FF ) + chunks;
		if ( !identity )
			return nullptr;

		return ( (CEntityIdentity*)identity )->m_pEntity;
	}

public:
	auto GetHighestEntityIndex() -> int
	{
		auto HighestIdx = -1;

		CGameEntitySystem_GetHighestEntityIndex( this , HighestIdx );

		return HighestIdx;
	}

public:
	/*
	00007FFAAA9B920 | E8 5EE2DDFF                    | call client.7FFAAA797470                         | GetLocalCitadelPlayerController
	00007FFAAA9B921 | 45:8B4F 78                     | mov r9d,dword ptr ds:[r15+0x78]                  |
	00007FFAAA9B921 | 4C:8D05 E304C000               | lea r8,qword ptr ds:[0x7FFAAB5B9700]             | 00007FFAAB5B9700:"preentitypacket:ack %d cmds"
	*/
	static auto GetLocalCitadelPlayerController() -> CCitadelPlayerController*
	{
		return CGameEntitySystem_GetLocalCitadelPlayerController( -1 );
	}

	// Offset for HighestEntityIndex in the entity system
	static constexpr int m_iHighestEntityIndex = 0x1510;
};

#define FOR_EACH_ENTITY( idx_name ) \
	for ( auto idx_name = 0; idx_name <= SDK::Interfaces::GameEntitySystem()->GetHighestEntityIndex(); idx_name++ )
