workspace "Crypto"
    configurations { "Debug", "Test", "Release" }
    targetdir "bin"

project "Crypto"
    kind "ConsoleApp"
    language "C"
    location "Crypto"

    files {
        "../src/**.h",
        "../src/**.c"
    }
    symbols "On"

    enablewarnings { "all", "extra" }
    fatalwarnings { "all" }
    filter "configurations:Debug"
        optimize "Debug"

    filter "configurations:Test"
        optimize "On"
    filter "configurations:Release"
        disablewarnings { "all" }
        optimize "On"
        symbols "Off"
