--[[--
  Use this file to specify **System** preferences.
  Review [examples](+D:\Programs\ZeroBraneStudio\cfg\user-sample.lua) or check [online documentation](http://studio.zerobrane.com/documentation.html) for details.
--]]--

console = true

editor.autotabs = true
editor.usetabs  = true
editor.tabwidth = 4
editor.usewrap = false


--path.love2d='D:/Programs/LOVE/love'
--path.tiledgame2 = '../TimeGame 2.0.exe'

--set ZBS=D:\Saxion\Project3rdPerson\Zerobrane_Studio
--set LUA_PATH=./?.lua;%ZBS%/lualibs/?.lua;%ZBS%/lualibs/?/?.lua
--set LUA_CPATH=%ZBS%/bin/?.dll;%ZBS%/bin/clibs/?.dll;%ZBS%/bin/clibs/?/?.dll
--mge -- my exe

package.path = package.path..";D:\Saxion\Project3rdPerson\Zerobrane_Studio/lualibs/?.lua;D:\Saxion\Project3rdPerson\Zerobrane_Studio/lualibs/?/?.lua"


package.cpath = package.cpath..";D:\Saxion\Project3rdPerson\Zerobrane_Studio/bin/clibs/?.dll;D:\Saxion\Project3rdPerson\Zerobrane_Studio/bin/clibs/?/?.dll"