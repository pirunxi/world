--
-- Created by HuangQiang on 2017/6/10.
--
local type = type
local tostring = tostring
local pairs = pairs
local ipairs = ipairs
local concat = table.concat
local insert = table.insert
local print = print
local sbyte = string.byte
local find = string.find
local sub = string.sub

local function to_readable(s)
	if #s > 32 then
		return "#" .. #s
	else
		local a = {sbyte(s, 1, #s)}
		local all_ascii = true
		for i, b in ipairs(a) do
			if b >= 128 then
				all_ascii = false
				break
			end
		end
		return all_ascii and ("'" .. s .. "'") or ("#" .. #s .. "#" .. concat(a, "."))
	end
end

local function atom_tostring(x)
	if type(x) == "string" then
		return to_readable(x)
	else
		return tostring(x)
	end
end

local function table_tostring(t)
	local code = {"{"}

	for k, v in pairs(t) do
		if type(v) ~= "table" then
			insert(code, tostring(k) .. "=" .. atom_tostring(v) .. ",")
		else
			insert(code, tostring(k) .. "=" .. table_tostring(v) .. ",")
		end
	end
	insert(code, "}")
	return concat(code)
end

local function printt(t)
	print(table_tostring(t))
end


local function lastIndexOf(haystack, needle)
	local i, j
	local k = 0
	repeat
		i = j
		j, k = find(haystack, needle, k + 1, true)
	until j == nil

	return i
end

local function getSimpleName(fullname)
	local pos = lastIndexOf(fullname, ".")
	return pos and sub(fullname, pos + 1, -1) or fullname
end

local function getNamespace(fullname)
	local pos = lastIndexOf(fullname, ".")
	return pos and sub(fullname, 1, pos - 1)
end

return {
	table_tostring = table_tostring,
	printt = printt,
	getSimpleName = getSimpleName,
	getNamespace = getNamespace,
}