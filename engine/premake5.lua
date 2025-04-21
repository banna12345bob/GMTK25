project "engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/compile/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/compile/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"%{IncludeDir.stb_image}/**.h",
		"%{IncludeDir.stb_image}/**.cpp",
	}

	includedirs
	{
		"src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.SDL3}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb_image}",
	}

	links 
	{
		"SDL3",
		"ImGui"
	}

	libdirs
	{
		"%{IncludeDir.SDL3}/../lib/x64/"
	}

	filter "system:windows"
		systemversion "latest"
		buildoptions { "/utf-8" }

	filter "system:linux"
		systemversion "latest"

	filter "system:macosx"
		systemversion "latest"
        xcodebuildsettings { ["ALWAYS_SEARCH_USER_PATHS"] = "YES" }

	filter "configurations:Debug*"
		defines "EG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release*"
		defines "EG_RELEASE"
		runtime "Debug"
		symbols "on"

	filter "configurations:Dist*"
		defines "EG_DIST"
		runtime "Release"
		symbols "off"
		optimize "on"
