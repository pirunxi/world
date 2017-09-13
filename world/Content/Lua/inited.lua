--
-- Created by HuangQiang on 2017/6/11.
--


local inited = {}

function inited.Init()
	print("== inited finish")
	collectgarbage("collect")
	
	print("=== LuaManager ", LuaManager, trace)
	
	local t = Test.new()
	t.a = 12
	t.b = 18
	t:print()
	t.hello(87, 1218)
	
end

return inited