--
-- Created by IntelliJ IDEA.
-- User: HuangQiang
-- Date: 2017/6/7
-- Time: 22:16

local insert = table.insert
local concat = table.concat
local format = string.format
local pairs = pairs
local ipairs = ipairs
local setmetatable = setmetatable
local tostring = tostring

local utils = require "utils"
local marshal = require "common.marshal"



local tempbs = Aio.BinaryStream(1024)

local mm = marshal.marshal
local mu = marshal.unmarshal
local network = {}
local MarshalSize = mm.size
local UnmarshalSize = mu.size
local typeCategory = {bool = 1, byte = 1, int = 1, long = 1, float = 1, double = 1, string = 1, binary = 1, set = 2, list = 2, map = 3 }

local msgmetas = {}

local msghandlers = {}

local function MarshalMethodNameWithoutVarName(typeName)
	local params = typeCategory[typeName]
	if params == 1 then
		return 'm' .. typeName
	else
		return 'm["' .. typeName .. '"]'
	end
end

local function MarshalMethodName(varName, typeName, ktypeName, vtypeName)
	local params = typeCategory[typeName]
	if params == 1 then
		return 'm' .. typeName .. "(bs," .. varName .. ")"
	elseif params == 2 then
		return 'm' .. typeName .. "(bs," .. varName .. "," .. MarshalMethodNameWithoutVarName(vtypeName) .. ')'
	elseif params == 3 then
		return 'm' .. typeName .. "(bs," .. varName .. "," .. MarshalMethodNameWithoutVarName(ktypeName) .. "," .. MarshalMethodNameWithoutVarName(vtypeName) ..')'
	else
		return 'm["' .. typeName .. '"](bs,' .. varName .. ")"
	end
end

local function UnmarshalMethodNameWithoutBsName(typeName)
	local params = typeCategory[typeName]
	if params == 1 then
		return 'u' .. typeName
	else
		return 'u["' .. typeName .. '"]'
	end
end

local function UnmarshalMethodName(typeName, ktypeName, vtypeName)
	local params = typeCategory[typeName]
	if params == 1 then
		return 'u' .. typeName .. "(bs)"
	elseif params == 2 then
		return 'u' .. typeName .. "(bs," .. UnmarshalMethodNameWithoutBsName(vtypeName) .. ')'
	elseif params == 3 then
		return 'u' .. typeName .. "(bs," .. UnmarshalMethodNameWithoutBsName(ktypeName) .. "," .. UnmarshalMethodNameWithoutBsName(vtypeName) ..')'
	else
		return 'u["' .. typeName .. '"](bs)'
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
		insert(cs, "do")
		insert(cs, "local name2id2name = {")
		for _, cname in ipairs(bean.children) do
			--print("== bean", bean.name, " child name", cname)
			local cid = beans[cname].typeid
			insert(cs, format("[\'%s\'] = %s, [%s] = \'%s\',", cname, cid, cid, cname))
		end
		insert(cs, "}")
		insert(cs, format([[m["%s"] = function (bs, x) ]], name))
		insert(cs, 'mint(bs, name2id2name[x.class])')
		insert(cs, 'm[x.class](bs, x)')
		insert(cs, "end")
		insert(cs, format([[u["%s"] = function (bs)]], name))
		insert(cs, "local id = uint(bs) local name = name2id2name[id]")
		insert(cs, 'local x = u[name](bs) x.class = name')
		insert(cs, "return x end")
		insert(cs, "end")
	else
		-- marshal
		insert(cs, 'm["' .. name .. '"] = function (bs, x)')
		local fields = GetSelfAndParentFields(bean, beans, {})
		for _, f in ipairs(fields) do
			insert(cs, MarshalMethodName("x." .. f.name, f.type, f.key, f.value))
		end
		insert(cs, "end")

		-- unmarshal
		insert(cs, 'u["' .. name .. '"] = function (bs) local x = {}')
		for _, f in ipairs(fields) do
			insert(cs, "x." .. f.name .. " = " .. UnmarshalMethodName(f.type, f.key, f.value))
		end
		insert(cs, "return x end")
	end
end

local function RegisterBeans()
    local cs = {
        [[
    local marshal = require "common.marshal"

    local m = marshal.marshal
    local u = marshal.unmarshal

    local mbool = m.bool
    local ubool = u.bool

    local mbyte = m.byte
    local ubyte = u.byte

    local mint = m.int
    local uint = u.int

    local mlong = m.long
    local ulong = u.long

    local mfloat = m.float
    local ufloat = u.float

    local mdouble = m.double
    local udouble = u.double

    local msize = m.size
    local usize = u.size

    local mstring = m.string
    local ustring = u.string

    local mbinary = m.binary
    local ubinary = u.binary

    local mlist = m.list
    local ulist = u.list

    local mset = m.set
    local uset = u.set

    local mmap = m.map
    local umap = u.map

        ]]
    }
	local beans = LoadOnce("msg.beans")
	for _, bean in ipairs(beans) do
		beans[bean.name] = bean
	end
    for _, bean in ipairs(beans) do
		GenMarshalCode(cs, bean, beans)
    end

	local msgs = LoadOnce("msg.msgs")
	for _, msg in ipairs(msgs) do
		GenMarshalCode(cs, msg, msgs)
	end

	insert(cs, "return marshal")
    local codeStr = concat(cs, "\n")
    --print(codeStr)
	local ret, err = loadstring(codeStr)
	if err then
		error(err)
	else
		ret()
	end

	for _, msg in ipairs(msgs) do
		local meta = {}
		meta.__index = meta
		local name = msg.name
		meta.class = name
		local typeid = msg.typeid
		meta._typeid = typeid
		local marshalMethod = mm[name]
		meta._encode = function (this, bs)
			MarshalSize(bs, typeid)
			marshalMethod(bs, this)
		end
		meta.__tostring = function(this)
			return name .. "#" .. utils.table_tostring(this)
		end
		msgmetas[name] = meta
		msgmetas[typeid] = meta
		mm[typeid] = mm[name]
		mu[typeid] = mu[name]
		--print("add msg name:", name, "type", typeid)
	end
end


-- return BinarySteram

-- return msg
function network.Decode(bs)
    local typeid = UnmarshalSize(bs)
	local meta = msgmetas[typeid]
	if not meta then
		error("unknown typeid:" .. typeid)
	end
    local x = mu[typeid](bs)
	setmetatable(x, meta)
    return x
end

local _luamanager_send = LuaManager.SendMsg
function network.Send(msg)
	print("== send ", tostring(msg))
	local bs = tempbs
	bs:Clear()
	msg:_encode(bs)
	_luamanager_send(bs)
end

local _decode = network.Decode
function network.Recv(bs)
	local msg = _decode(bs)
	print("== recv", tostring(msg))
	local handler = msghandlers[msg.class]
	if handler then
		handler(msg)
	else
		error(msg.class .. " don't have a handler")
	end
end


-- 有意这么做的
-- OnConnect OnClose OnClose
-- 导出给外部接口
-- 并不优雅,但也不丑陋

RecvMsg = network.Recv

function OnConnect()
	print("== network connect")
	network.CreateAndSend("msg.client2gate.CBindServer", {serverid=13})
end

function OnClose()
	print("== network close")
end

function network.CreateMsg(fullname, x)
	local meta = msgmetas[fullname]
	if not meta then
		error("unknown msg type:" .. fullname)
	end
	x = x or {}
	setmetatable(x, meta)
	return x
end

local _send = network.Send
local _create = network.CreateMsg
function network.CreateAndSend(fullname, data)
	_send(_create(fullname, data))
end

function network.Handler(msgName, handler)
	if msghandlers[msgName] then
		error("msg:", msgName, "has have handle, can't add handler again")
	end
	msghandlers[msgName] = handler
end

local _luamanager_connect = LuaManager.Connect
function network.Init()
	RegisterBeans()
	--[[
	--local msg = _create("msg.gs.auth.SAuth", {userid=12323, account="huangqiang"})
	local msg = _create("msg.gs.mall.SBuy", {id=1, item={class = 'Equip', id=1218, name='it a item'}})
	utils.printt(msg)
	local bs = tempbs
	msg:encode(tempbs)
	local x = network.Decode(bs)
	utils.printt(x)
	--]]
	_luamanager_connect("localhost", 1218)
end

return network
