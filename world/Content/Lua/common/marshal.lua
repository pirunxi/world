--
-- Created by HuangQiang on 2017/6/10.
--
local insert = table.insert

local bs = Aio.BinaryStream

local m = {}
local u = {}

m.bool = bs.MarshalBool
u.bool = bs.UnmarshalBool

m.byte = bs.MarshalByte
u.byte = bs.UnmarshalByte

m.int = bs.MarshalInt
u.int = bs.UnmarshalInt

m.long = bs.MarshalLong
u.long = bs.UnmarshalLong

m.float = bs.MarshalFloat
u.float = bs.UnmarshalFloat

m.double = bs.MarshalDouble
u.double = bs.UnmarshalDouble

m.size = bs.MarshalSize
u.size = bs.UnmarshalSize

m.string = bs.MarshalString
u.string = bs.UnmarshalString

m.binary = bs.MarshalBytes
u.binary = bs.UnmarshalBytes

local msize = m.size
m.list = function (bs, x, vfun)
    local n = #x
    msize(bs, n)
    for i = 1, n do
        vfun(bs, x[i])
    end
end

local usize = u.size
u.list = function(bs, vfun)
    local x = {}
    local n = usize(bs)
    for i = 1, n do
        insert(x,vfun(bs))
    end
    return x
end

m.set = m.list
u.set = u.list

local function map_size(m)
    local n = 0
    for _ in pairs(m) do
        n = n + 1
    end
	return n
end

m.map = function (bs, x, kfun, vfun)
    local n = map_size(x)
    msize(bs, n)
    for k, v in pairs(x) do
        kfun(bs, k)
        vfun(bs, v)
    end
end

u.map = function(bs, kfun, vfun)
    local x = {}
    local n = usize(bs)
    for i = 1, n do
	    local k = kfun(bs)
        local v = vfun(bs)
        x[k] = v
    end
    return x
end


return {
    marshal = m,
    unmarshal = u,
}
