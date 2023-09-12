project "Test_SMJM"
      kind "ConsoleApp"

      targetdir "../dist/bin/%{cfg.buildcfg}"
      location "../build/"

   if os.istarget("windows") then
      links{ "version" }
   end
      includedirs { "../" }
      files { "../Orochi/Orochi.h", "../Orochi/Orochi.cpp" }
      files { "*.cpp", "*.h" }
      files { "../contrib/**.h", "../contrib/**.cpp" }
