--
-- Created by HuangQiang on 2017/6/12.
--



function LoadOnce(mod)
	local r = require(mod)
	package.loaded[mod] = nil
	return r
end

