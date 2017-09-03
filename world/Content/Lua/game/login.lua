--
-- Created by HuangQiang on 2017/6/10.
--
local network = require "common.network"

local login = {}

local roles = {}

local curLoginRoleid
local function onmsg_SGetRoleList(msg)
	roles = msg.roles
	if #roles == 0 then
		network.CreateAndSend("msg.gs.login.CCreateRole", {name="hq@" .. Time.time, gender=1, profession=2})
	else
		network.CreateAndSend("msg.gs.login.CRoleLogin", {roleid = roles[1].roleid})
	end
end

local function onmsg_SCreateRole(msg)
	table.insert(roles, msg.roleinfo)
	network.CreateAndSend("msg.gs.login.CRoleLogin", {roleid = msg.roleinfo.roleid})
end

local function onmsg_SRoleLogin(msg)
	curLoginRoleid = msg.roleid
end

function login.Init()
	network.Handler("msg.gs.login.SGetRoleList", onmsg_SGetRoleList)
	network.Handler("msg.gs.login.SCreateRole", onmsg_SCreateRole)
	network.Handler("msg.gs.login.SRoleLogin", onmsg_SRoleLogin)
end
return login