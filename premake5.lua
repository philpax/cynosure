solution "cynosure"
	configurations { "Debug", "Release" }

	project "cynosure"
		kind "ConsoleApp"
		language "C++"
		targetdir "bin"
		debugdir "bin"

		includedirs { "src/" }
		files { "src/**.hpp", "src/**.cpp", "test/**.asm" }

		postbuildcommands 
		{ 
			[[{COPY} test/floppy_1_44.img bin/]],
			[[{COPY} test/hd1.fs bin/]],
			[[{COPY} test/config.json bin/]]
		}

		filter "configurations:Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

		filter "configurations:Release"
			defines { "NDEBUG" }
			optimize "On"

		filter 'files:**.asm'
			buildmessage 'Compiling %{file.relpath}'

			buildcommands {
				'nasm -O0 -f bin -o "bin/%{file.basename}.bin" "%{file.relpath}"'
			}

			buildoutputs { '%{cfg.objdir}/%{file.basename}.bin' }

		configuration { "gmake" }
			buildoptions { "-std=c++11" }