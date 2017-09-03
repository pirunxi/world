return {
{
name = 'cfg.role.Login',
children={'cfg.role.Player','cfg.role.Monster'},
fields={
{name = 'name', type='string', },
{name = 'desc', type='string', },
}
},
{
name = 'cfg.role.Role',
fields={
{name = 'name', type='string', },
{name = 'desc', type='string', },
}
},
{
name = 'cfg.role.Player',
parent = 'cfg.role.Login',
fields={
{name = 'pro', type='int', },
}
},
{
name = 'cfg.role.Monster',
parent = 'cfg.role.Login',
fields={
{name = 'key', type='long', },
{name = 'xx', type='string', },
}
},
{
name = 'cfg.role.Profession',
fields={
{name = 'id', type='int', },
{name = 'name', type='string', },
{name = 't1', type='bool', },
{name = 't2', type='int', },
{name = 't3', type='long', },
{name = 't4', type='float', },
{name = 't5', type='double', },
{name = 't6', type='list', value='int', },
{name = 't7', type='set', value='int', },
{name = 't8', type='map', key='int', value='int', },
{name = 't9', type='cfg.role.Login', },
}
},
}