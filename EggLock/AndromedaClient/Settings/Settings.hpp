#pragma once

#include <Common/Common.hpp>

namespace Settings
{
	namespace Visual
	{
		inline auto Active = true;

		inline auto BoxEsp = true;
		inline auto BoxStyle = 0;

		inline auto SkeletonEsp = true;
		inline auto HeadDot = true;
		inline auto HealthBar = true;
		inline auto NameEsp = false;
		inline auto DistanceEsp = true;
		inline auto Snaplines = false;

		inline auto NpcEsp = true;

		inline auto SoundStepEsp = true;
	}
	namespace AimBot
	{
		inline auto Active = false;
		inline auto Key = 0;
		inline auto Fov = 15.f;
		inline auto Smooth = 4.f;
		inline auto Bone = 0;
		inline auto TeamCheck = true;
	}
	namespace Misc
	{
		inline auto SkyColorChanger = true;
		inline auto NightMode = false;

		inline auto MenuAlpha = 200;
		inline auto MenuStyle = 0;
		inline auto MenuSounds = false;
		inline auto MenuKey = 0x2D;
	}
	namespace Colors
	{
		namespace Visual
		{
			inline float SoundStepEsp[3] = { 1.f , 1.f , 0.f };
		}
		namespace Misc
		{
			inline float SkyColor[3] = { 1.f , 1.f , 0.f };
		}
	}
	namespace Update
	{
		inline auto Enabled = true;
		inline const char* Url = "";
	}
}
