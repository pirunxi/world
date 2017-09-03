return {
{
name = 'msg.gs.login.SRoleLogin', typeid=23990,
fields={
{name = 'roleid', type='long'},
}
},
{
name = 'msg.gs.common.SError3', typeid=41408,
fields={
{name = 'err', type='string'},
}
},
{
name = 'msg.gs.mall.SSell', typeid=50608,
fields={
{name = 'id', type='string'},
}
},
{
name = 'msg.gs.login.SCreateRole', typeid=25755,
fields={
{name = 'roleinfo', type='msg.gs.login.RoleInfo'},
}
},
{
name = 'msg.gs.auth.CAuth', typeid=16278,
fields={
{name = 'account', type='string'},
}
},
{
name = 'msg.gs.login.CRoleLogin', typeid=13750,
fields={
{name = 'roleid', type='long'},
}
},
{
name = 'msg.server2gate.Gate2XDelSession', typeid=4,
fields={
{name = 'sid', type='int'},
}
},
{
name = 'msg.server2gate.X2GateForwardToClientSession', typeid=6,
fields={
{name = 'sid', type='int'},
{name = 'msg', type='binary'},
}
},
{
name = 'msg.server2gate.X2XAnnounceServer', typeid=7,
fields={
{name = 'type', type='string'},
{name = 'serverid', type='int'},
{name = 'args', type='map', key='string', value='string'},
}
},
{
name = 'msg.gs.mall.CBuy', typeid=53080,
fields={
{name = 'id', type='int'},
}
},
{
name = 'msg.gs.login.CGetRoleList', typeid=44975,
fields={
}
},
{
name = 'msg.gs.common.SError', typeid=24579,
fields={
{name = 'err', type='int'},
}
},
{
name = 'msg.client2gate.CBindServer', typeid=1,
fields={
{name = 'serverid', type='int'},
}
},
{
name = 'msg.gs.mall.CSell', typeid=46848,
fields={
{name = 'id', type='string'},
}
},
{
name = 'msg.server2gate.Gate2XForwardFromClientSession', typeid=5,
fields={
{name = 'sid', type='int'},
{name = 'msg', type='binary'},
}
},
{
name = 'msg.gs.mall.SBuy', typeid= 20712,
fields={
{name = 'id', type='int'},
{name = 'item', type='msg.gs.mall.Item'},
}
},
{
name = 'msg.gs.login.CCreateRole', typeid=13899,
fields={
{name = 'name', type='string'},
{name = 'gender', type='int'},
{name = 'profession', type='int'},
}
},
{
name = 'msg.gs.login.SGetRoleList', typeid=25761,
fields={
{name = 'userid', type='long'},
{name = 'roles', type='list', value='msg.gs.login.RoleInfo'},
}
},
{
name = 'msg.client2gate.SBindServer', typeid=2,
fields={
{name = 'serverid', type='int'},
}
},
{
name = 'msg.gs.auth.SAuth', typeid=12518,
fields={
{name = 'account', type='string'},
{name = 'userid', type='long'},
}
},
{
name = 'msg.gs.common.SError2', typeid=41409,
fields={
{name = 'err', type='int'},
{name = 'param', type='int'},
}
},
}