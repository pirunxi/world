return {
{
name = 'msg.gs.login.RoleInfo',  
fields={
{name = 'roleid', type='long'},
{name = 'name', type='string'},
{name = 'gender', type='int'},
{name = 'profession', type='int'},
}
},
{
name = 'msg.gs.mall.Equip', typeid=1, 
parent = 'msg.gs.mall.Item',
fields={
{name = 'name', type='string'},
}
},
{
name = 'msg.gs.mall.Title', typeid=2, 
parent = 'msg.gs.mall.Item',
fields={
{name = 'num', type='int'},
}
},
{
name = 'msg.gs.mall.Item',  children={'msg.gs.mall.Equip','msg.gs.mall.Title'},
fields={
{name = 'id', type='int'},
}
},
}