--
-- Created by HuangQiang on 2017/6/11.
--
local concat = table.concat
local loadstring = loadstring
local print = print
local error = error
local insert = table.insert
local ipairs = ipairs
local format = string.format

local utils = require "utils"
local datamarshal = require "common.datamarshal"

local MEMORY_OPTIMISTIC = false

local config = {}

local basicTypes = {bool = 1, int = 1, long = 1, float = 1, double = 1, string = 1, set = 2, list = 2, map = 3 }
local function UnmarshalMethodNameWithoutBsName(typeName)
	local params = basicTypes[typeName]
	if params == 1 then
		return 'm' .. typeName
	else
		return 'm["' .. typeName .. '"]'
	end
end

local function UnmarshalMethodName(typeName, ktypeName, vtypeName)
	local params = basicTypes[typeName]
	if params == 1 then
		return 'm' .. typeName .. "(ds)"
	elseif params == 2 then
		return 'm' .. typeName .. "(ds," .. UnmarshalMethodNameWithoutBsName(vtypeName) .. ')'
	elseif params == 3 then
		return 'm' .. typeName .. "(ds," .. UnmarshalMethodNameWithoutBsName(ktypeName) .. "," .. UnmarshalMethodNameWithoutBsName(vtypeName) ..')'
	else
		return 'm["' .. typeName .. '"](ds)'
	end
end

local function GetSelfAndParentFields(bean, beans, fields)
	if bean.parent then
		GetSelfAndParentFields(beans[bean.parent], beans, fields)
	end
	for _, v in ipairs(bean.fields) do
		insert(fields, v)
	end
	return fields
end

local function GenMarshalCode(cs, bean, beans)
	local name = bean.name


	if bean.children then
		insert(cs, 'm["' .. name .. '"] = function (ds)')
		local namespace = utils.getNamespace(name)
		insert(cs, "local name = mstring(ds)")
		if namespace then
			insert(cs, 'return m["' .. namespace .. '." .. name](ds)')
		else
			insert(cs, 'return m[name](ds)')
		end
		insert(cs, "end")
	else
		local fields = GetSelfAndParentFields(bean, beans, {})
		if MEMORY_OPTIMISTIC then
			insert(cs, "do")
			insert(cs, "local name2index = {")
			for index, f in ipairs(fields) do
				insert(cs, f.name .. "=" .. index .. ",")
			end
			insert(cs, "}")
			insert(cs, [[local meta = {
	__index = function (t, key)
		local index = name2index[key]
		if index then
			return rawget(t, index)
		elseif key == "class" then
			return "]] .. name .. [["
		else
			error("unknown field:" .. key)
		end
	end	}]])
			insert(cs, 'm["' .. name .. '"] = function (ds)')
			insert(cs, 'local x={')
			for _, f in ipairs(fields) do
				insert(cs, UnmarshalMethodName(f.type, f.key, f.value))
				insert(cs, ",")
			end
			insert(cs, "}")
			insert(cs, "setmetatable(x, meta)")
			insert(cs, "return x end end")
		else
			insert(cs, 'm["' .. name .. '"] = function (ds)')
			insert(cs, 'return {')
			insert(cs, format('class="%s",', name))
			for _, f in ipairs(fields) do
				insert(cs, f.name .. "=" .. UnmarshalMethodName(f.type, f.key, f.value) .. ",")
			end
			insert(cs, "} end")
		end
	end
end

local function RegisterBeans()
	local cs = {[[
local setmetatable = setmetatable
local m = require "common.datamarshal"

local mbool = m.bool
local mint = m.int
local mlong = m.long
local mfloat = m.float
local mdouble = m.double
local mstring = m.string
local mlist = m.list
local mset = m.set
local mmap = m.map
	]]}

	local beans = LoadOnce("cfg.beans")
	for _, bean in ipairs(beans) do
		beans[bean.name] = bean
	end
	for _, bean in ipairs(beans) do
		GenMarshalCode(cs, bean, beans)
	end

	insert(cs, "return m")
	local codeStr = concat(cs, "\n")
	--print(codeStr)
	local ret, err = loadstring(codeStr)
	if err then
		error(err)
	else
		ret()
	end
end

local configDir
local tables = {}
local function LoadConfigs()
	local ctables = LoadOnce("cfg.tables")
	local ds = cfg.DataStream("")
	local lines = io.lines
	for _, t in ipairs(ctables) do
		local datas
		local name = t.name:lower()
		local type = t.type
		print("== load config begin. ", name)
		local mode = t.mode
		local file = configDir .. t.file
		local vfun = datamarshal[type]
		if mode == "map" then
			datas = {}
			local key = t.key
			for line in lines(file) do
				ds:Clear()
				ds:From(line)
				local value = vfun(ds)
				datas[value[key]] = value
			end
		elseif mode == "list" then
			datas = {}
			for line in lines(file) do
				ds:Clear()
				ds:From(line)
				insert(datas, vfun(ds))
			end
		elseif mode == "one" then
			datas = nil
			for line in lines(file) do
				if datas then error("config mode=one, should have only one data") end
				ds:Clear()
				ds:From(line)
				datas = vfun(ds)
			end
		else
			error("unknown mode " .. mode)
		end
		tables[name] = datas
		utils.printt(datas)
		print("== load config   end. ", name)
	end
end

function config.Get(tableName, id)
	local table = tables[tableName]
	return id and table[id] or table
end

function config.GetAll(tableName)
	return tables[tableName]
end

function config.Init()
	configDir = UnityEngine.Application.dataPath .. "/../scripts/data/"
	-- 有意的,全局枚举表
	enums = require "cfg.enums"

	RegisterBeans()
	LoadConfigs()
end

return config
