local require = require
require "global"

local m = {}
function m.Init()

	for _, module in ipairs(LoadOnce("modules")) do
		--print("== load ", module, "begin")
		local m = require(module)
		--print("== load ", module, "end")
		m.Init()
	end
end

return m
