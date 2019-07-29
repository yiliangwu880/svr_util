--
-- premake5 file to build RecastDemo
-- http://premake.github.io/
--
require "lib"

local action = _ACTION or ""
local outdir = action


WorkSpaceInit ("svr_util")



Project "src"
	includedirs { 
		"../include/",
		"../include/time/",
	}

	SrcPath { 
		"../src/**",  --**递归所有子目录，指定目录可用 "cc/*.cpp" 或者  "cc/**.cpp"
		"../include/**",
	}
	files {"../*.sh",}
	files {"../*.txt",}
	
Project "test"
	includedirs { 
		"../include/",
		"base/",
	}

	SrcPath { 
		"../test/**",  
	}
	
Project "samples"
	includedirs { 
		"../include/",
		"base/",
	}

	SrcPath { 
		"../samples/**",  
	}
	



    
    
    