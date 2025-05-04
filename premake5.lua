workspace "GMTK25"
	startproject "sandbox"

	configurations
	{
		"Debug_x86_64",
		"Release_x86_64",
		"Dist_x86_64"
	}
	filter "configurations:*_x86_64"
		architecture "x86_64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/engine/vendor/spdlog/include"
IncludeDir["SDL3"] = "%{wks.location}/engine/vendor/SDL3-3.2.10/include"
IncludeDir["ImGui"] = "%{wks.location}/engine/vendor/imgui"
IncludeDir["stb_image"] = "%{wks.location}/engine/vendor/stb_image"
IncludeDir["glad"] = "%{wks.location}/engine/vendor/Glad/include"

group "Dependencies"
	include "engine/vendor/imgui"
	include "engine/vendor/glad"

group ""
	include "engine"
	include "sandbox"