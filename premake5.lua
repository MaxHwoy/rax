workspace "rax"
	location ".\\build\\"
	startproject "rax-debug"
	vectorextensions "sse2"

	targetdir "%{wks.location}\\bin\\%{cfg.buildcfg}\\"
	objdir "%{wks.location}\\obj\\%{cfg.buildcfg}\\%{prj.name}\\"
	buildlog "%{wks.location}\\obj\\%{cfg.buildcfg}\\%{prj.name}.log"

	editandcontinue "off"
	staticruntime "on"

	systemversion "latest"
	characterset "mbcs"
	warnings "extra"

	includedirs {
		".\\src\\",
	}

	buildoptions {
		"/Zm200",
		"/utf-8",
		"/std:c++latest",
	}

	flags {
		"no64bitchecks",
		"shadowedvariables",
		"noincrementallink",
		"undefinedidentifiers",
		"multiprocessorcompile",
	}

	defines {
		"NOMINMAX",
		"_CRT_SECURE_NO_WARNINGS",
	}

	platforms {
		"x86",
		"x64",
	}

	configurations {
		"debug",
		"release",
	}

	configuration "debug"
		defines "DEBUG"
		optimize "debug"
		inlining "disabled"
		runtime "debug"
		symbols "full"

	configuration "release"
		defines "NDEBUG"
		optimize "full"
		inlining "auto"
		runtime "release"
		symbols "full"

	project "rax"
		language "c++"
		kind "staticlib"

		pchheader "stdafx.hpp"
		pchsource "src/rax/stdafx.cpp"

		files {
			"src/rax/**.*",
		}

		includedirs {
			"src/rax",
		}

	project "rax-tests"
		language "c++"
		kind "consoleapp"

		pchheader "stdafx.hpp"
		pchsource "src/rax-tests/stdafx.cpp"

		files {
			"src/rax-tests/**.*",
		}

		includedirs {
			"src/rax-tests",
		}

		links {
			"rax",
		}

		-- submodule.include({ "gtest" })

	project "rax-debug"
		language "c++"
		kind "consoleapp"

		pchheader "stdafx.hpp"
		pchsource "src/rax-debug/stdafx.cpp"

		files {
			"src/rax-debug/**.*",
		}

		includedirs {
			"src/rax-debug",
		}

		links {
			"rax",
		}
