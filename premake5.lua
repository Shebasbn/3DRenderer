workspace "3DRenderer"
    architecture "x64"
    startproject "3DRenderer"

    configurations { "Debug", "Release"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "3DRenderer"
    location "3DRenderer"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
	pchsource "Aurora/src/pch.cpp"

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
		"%{prj.name}/src"
	}

    links 
	{
	}

    filter "system:windows"
		systemversion "latest"
		buildoptions "/utf-8"

		defines 
		{
		}
		
	filter "configurations:Debug"
		defines {"AR_DEBUG", "AR_ENABLE_ASSERTS"}
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "AR_RELEASE"
		runtime "Release"
		optimize "on"