workspace "3DRenderer"
    architecture "x64"
    startproject "3DRenderer"

    configurations { "Debug", "Release"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

libDir = {}
libDir["SDL3"] = "%{prj.name}/vendor/SDL3"



project "3DRenderer"
    location "3DRenderer"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    --pchheader "pch.h"
	--pchsource "Aurora/src/pch.cpp"

    files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp",
	}

    defines
	{
		
	}

    includedirs 
	{
		"%{libDir.SDL3}/include"
	}

	libdirs
	{
		"%{libDir.SDL3}/lib/x64"
	}

    links 
	{
		"SDL3.lib"
	}

	postbuildcommands {
		'xcopy /Q /Y "%{prj.location}vendor\\SDL3\\lib\\x64\\SDL3.dll" "%{cfg.targetdir}\\."'
    }

    filter "system:windows"
		systemversion "latest"
		buildoptions "/utf-8"

		defines 
		{
		}
		
	filter "configurations:Debug"
		--defines {"_DEBUG", "X_ENABLE_ASSERTS"}
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "AR_RELEASE"
		runtime "Release"
		optimize "on"