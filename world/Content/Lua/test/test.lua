--
-- Created by HuangQiang on 2017/6/12.
--
--[[
local fieldName2Index = {
	id = 1,
	name = 2,
	dest = 3,
}

local meta = {

}

meta.__index = function (t, key)
	local index = fieldName2Index[key]
	if index then
		return rawget(t, index)
	elseif key == "class" then
		return "cfg.role.Item"
	else
		error("unknown type:" .. "cfg.role.Item's field:" .. key)
	end
end


local x = {15, "hahah", "my name is hq"}

setmetatable(x, meta)

print(x.id, x.name, x.dest, x.class)
--]]


local gc = collectgarbage
local insert = table.insert
local tostring = tostring
local setmetatable = setmetatable

local mem_before
local mem_after
local function stat_begin()
	gc()
	mem_before = gc("count")
end

local function stat_end()
	gc()
	mem_after = gc("count")
	print("== cost mem:", (mem_after - mem_before), "before", mem_before, "after", mem_after)
end

--[[
local datas1 = {}
local N = 102
stat_begin()
local index = 0
for i = 1, N do
	insert(datas1, {ectypeid=index, bornregion1=tostring(index+1), reviveregion=index+2, mainregionid=tostring(index+3), battlenum=index+4, bossrange=index+5, kill=index+6,
	towerp1 = index+7, towerp2=index+8, drawaw=index+9, winf=index+10, lostf=index+11, countdown=tostring(index+12), minonlinemembernum=tostring(index+13), class = "cfg.role.Item"})
	index = index + 14
end
stat_end()

local datas2 = {}
stat_begin()
local index = 0
for i = 1, N do
	local x = {index, tostring(index+1), index+2, tostring(index+3), index+4, index+5, index+6,index+7, index+8, index+9, index+10, index+11, tostring(index+12), tostring(index+13) }
	setmetatable(x, datas1)
	insert(datas2, x)
	index = index + 14
end
stat_end()


local datas1 = {}
local N = 10200
stat_begin()
local index = 0
for i = 1, N do
	local o = {}
	o.ectypeid = index
	o.bornregion = tostring(index + 1)
	o.reviveregion = index + 2
	insert(datas1, o)
	index = index + 14
end
stat_end()

local datas2 = {}
stat_begin()
local index = 0
for i = 1, N do
	local x = {nil, nil, nil }
	setmetatable(x, datas1)
	insert(datas2, x)
	index = index + 14
end
stat_end()

--]]


local m = {}
local function time(name, func)
	for i = 1, 5 do
		collectgarbage()
		local t1 = os.clock()
		func()
		local t2 = os.clock()
		print("==", name, " cost time:", t2 - t1)
	end
end

local function test10()
	local g  = 0
	local f = 0
	for i = 1, 10000000 do
		g = g + i
		f = f + g
		g = g + i
		f = f + g
		g = g + i
		f = f + g
		g = g + i
		f = f + g
	end
	return f
end

local function test11()
	local g  = 0
	local f = 0
	for i = 1, 10000000 do
		g = g + i
		f = f * g
		g = g + i
		f = f - g
		g = g + i
		f = f * g
		g = g + i
		f = f * g
	end
	return f
end

local function test12()
	local g  = 1.2
	local f = 2.1
	for i = 1, 10000000 do
		g = g + i
		f = f * g
		g = g + i
		f = f - g
		g = g + i
		f = f * g
		g = g + i
		f = f * g
	end
	return f
end

local function test13()
	local g  = 1.2
	local f = 2.1
	for i = 1, 10000000 do
		g = g / i
		f = f / g
		g = g / i
		f = f / g
		g = g / i
		f = f / g
		g = g / i
		f = f / g
	end
	return f
end

local function test2()
	local transform = mainObj.transform
	local Vector3 = Vector3
	for i = 1, 100000 do
		transform.position = Vector3(1,2,3)
		transform.position = Vector3(1,2,3)
		transform.position = Vector3(1,2,3)
		transform.position = Vector3(1,2,3)
		transform.position = Vector3(1,2,3)
		transform.position = Vector3(1,2,3)
		transform.position = Vector3(1,2,3)
		transform.position = Vector3(1,2,3)
		transform.position = Vector3(1,2,3)
		transform.position = Vector3(1,2,3)
	end
end

local function test3()
	local r
	for i = 1, 10000000 do
		r = Time.time
	end
	return r
end


local function test4()
	local GameObject = GameObject
	for i = 1, 10000 do
		GameObject()
	end
end

--[[
local function test3()
	local m = mainObj.transform.position
	for i = 1, 2000 do
		m:Normalize()
	end
end

local function test4()
	local transform = mainObj.transform
	local t = transform.position
	for i = 1, 2000 do
		transform.position = t
	end
end

local function test5()
	for i = 1, 2000 do
		Vector3(i, i , i)
	end
end

local function test6()
	for i = 1, 50000 do
		local g = GameObject("init")
		--GameObject.DestroyImmediate(g)
	end
end
--]]
function m.Init()
--[[
	print("== test")
	jit.off()
	mainObj = GameObject.Find("main")
--	time("test0", test0)
		time("test10", test10)
		time("test11", test11)
		time("test12", test12)
		time("test13", test13)
		time("test2", test2)
		time("test3", test3)
		time("test4", test4)

	time("test2", test2)
	time("test3", test3)
	time("test4", test4)
	time("test5", test5)
	time("test6", test6)
-]]

	local fs = {}
	for i = 1, 10 do
		local k = i * i
		table.insert(fs, function () print(k) end)
	end
	for _, f in ipairs(fs) do
		f()
	end
end

return m




