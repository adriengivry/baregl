project "6-tessellation"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	targetdir (outputdir .. "%{cfg.buildcfg}/%{prj.name}")
	objdir (objoutdir .. "%{cfg.buildcfg}/%{prj.name}")
	debugdir (outputdir .. "%{cfg.buildcfg}/%{prj.name}")
	
	files {
		"**.h",
		"**.cpp",
	}

	includedirs {
		depsdir .. "_glm/glm",
		depsdir .. "_glfw/glfw/include",
		"%{wks.location}../../include",
	}

	links {
		"glfw",
		"glm",
		"baregl"
    }

	filter { "configurations:Debug" }
		defines { "DEBUG" }
		symbols "On"

	filter { "configurations:Release" }
		defines { "NDEBUG" }
		optimize "On"
