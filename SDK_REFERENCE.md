# Deadlock SDK Reference

## Tested on build 4,192

### DLL Patterns

**client.dll:**
```
LookupBone: E8 ? ? ? ? 83 F8 ? 74 ? 4C 8D 4D
GetBonePosition: 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 4D 8B F1 49 8B E8 8B F2
CGameEntitySystem/dwEntityList: 44 89 BF ? ? ? ? 4C 89 3D ? ? ? ?
GetLocalPlayerController: 48 8B 05 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 8B 90
```

**engine2.dll:**
```
ViewMatrix: 4D 8D BC 24 ? ? ? ? 49 03 EE
```

### Offsets
- CGameEntitySystem::HighestEntityIndex: 0x1510

### HeroIDs (PlayerDataGlobal_t::m_nHeroID)
```cpp
enum HeroIDs {
    Infernus = 1,
    Seven, 
    Vindicta,
    LadyGeist,
    Abrams = 6,
    Wraith,
    McGinnis,
    Paradox = 10,
    Dynamo,
    Kelvin,
    Haze,
    Holliday,
    Bebop,
    GreyTalon = 17,
    MoAndKrill,
    Shiv,
    Ivy,
    Warden = 25,
    Yamato = 27,
    Lash = 31,
    Viscous = 35,
    Wrecker = 48,
    Pocket = 50,
    Mirage = 52
};
```

### Usage
```cpp
// Get bone position
int iHead = pPlayerPawn->LookupBone("head");
if (iHead != -1)
    pPlayerPawn->GetBonePosition(iHead, vHeadPos, Vector());

// Iterate entities
for (int i = 0; i <= (*g_pEntitySystem)->m_iHighestEntityIndex; ++i) {
    C_BaseEntity* entity = (*g_pEntitySystem)->GetBaseEntity(i);
    if (!entity) continue;
    if (strcmp(entity->m_pIdentity->GetAnyName(), "citadel_player_controller")) continue;
    if (entity == g_pLocalController) continue;
    // render...
}
```

### Notes
- Most [removed] functions are the same as CS2/Dota
- Dev builds have debug strings that will be removed on release
- Source: https://pastebin.com/raw/P1G80aab (client.dll)
- Source: https://pastebin.com/raw/kBL1jV0B (server.dll)
- Source: https://pastebin.com/raw/XSTJEJip (engine2.dll)
- SDKs: C:\Users\EggZys\Desktop\source2sdk, C:\Users\EggZys\Desktop\source2gen
