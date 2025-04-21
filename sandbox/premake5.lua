project "sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/compile/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/compile/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{IncludeDir.spdlog}",
		"%{wks.location}/engine/src",
		"%{wks.location}/engine/vendor",
	}

	links
	{
		"engine"
	}
	
	postbuildcommands {
		"{COPY} %{IncludeDir.SDL3}/../lib/x64/SDL3.dll %{cfg.targetdir}"
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
		optimize "on"
		symbols "off"
