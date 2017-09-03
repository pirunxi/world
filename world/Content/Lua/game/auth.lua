--
-- Created by HuangQiang on 2017/6/10.
--
local network = require "common.network"

local auth = {}

local serverid
local accout = "huangqiang"
local userid

local function onmsg_SBindServer(msg)
	serverid = msg.serverid
	network.CreateAndSend("msg.gs.auth.CAuth", {account=accout})
end

local function onmsg_SAuth(msg)
	userid = msg.userid

	network.CreateAndSend("msg.gs.login.CGetRoleList", {})
end

function auth.Init()
	network.Handler("msg.client2gate.SBindServer", onmsg_SBindServer)
	network.Handler("msg.gs.auth.SAuth", onmsg_SAuth)
end


return auth