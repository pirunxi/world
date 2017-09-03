--
-- Created by HuangQiang on 2017/8/8.
--

local fs = {}
for i = 1, 10 do
	local k = i * i
	table.insert(fs, function () print(k) end)
	k = k * 10
	table.insert(fs, function () print(k) end)
end
for _, f in ipairs(fs) do
	f()
end
