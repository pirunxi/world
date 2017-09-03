--
-- Created by HuangQiang on 2017/6/11.
--
local insert = table.insert

local ds = cfg.DataStream
local m = {}

m.bool = ds.GetBool
m.int = ds.GetInt
m.long = ds.GetLong
m.float = ds.GetFloat
m.double = ds.GetDouble
m.string = ds.GetString

local usize = m.int
function m.list(this, vfun)
	local x = {}
	for i = 1, usize(this) do
		local v = vfun(this)
		insert(x, v)
	end
	return x
end

function m.set(this, vfun)
	local x = {}
	for i = 1, usize(this) do
		x[vfun(this)] = true
	end
	return x
end

function m.map(this, kfun, vfun)
	local x = {}
	for i = 1, usize(this) do
		-- 如果简单地将以下三行合并为  x[kfun(this)] = vfun(this) 恐怕会有执行顺序的问题
		-- 这样分开比较清楚
		local k = kfun(this)
		local v = vfun(this)
		x[k] = v
	end
	return x
end

return m