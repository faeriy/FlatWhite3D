workspace "FlatWhite3D"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "FlatWhite3D"
    location "FlatWhite3D"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        defines {
            "FW_PLATFORM_WINDOWS",
            "FW_BUILD_DLL"
        }
        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "FW_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "FW_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "FW_DIST"
        optimize "On"
    
        filter {"system:windows", "configurations:Release"}
            buildoptions "-utf8"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "FlatWhite3D/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        
        defines {
            "FW_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines "FW_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "FW_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "FW_DIST"
        optimize "On"
