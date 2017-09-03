--
-- Created by HuangQiang on 2017/8/9.
--


local Component = {

}

local Sequence
local Paraller
local Fork
local Timeline
local LocalVariable
local GlobalVariable
local ComposeAction

local skill1 = Skill{
	id = 1,
	name = "skill1",

	variables = {"self", "position", "target"},

	subs = Paraller {
		Timeline{0,5, Bomb{ attackid = 1, hitpoint = 3} },
		Timeline{0.6, ShakeScreen{ xx = 1, yy = 2}},
		Breakable{ starttime=0.5, endtime=1.5, breaktype=3},
	}
}

local Behavior = {
	id = 1,
	params = {}, --  name ->
	children = {}
}

local skill2 = Skill{
	id = 2,
	name="skill2",
	params = {"self", "position", "target"},
	children = Seq {
		Timeline{0,5, Attack{ attackid = 1, hitpoint = 3, params={ "self", "position", "target" }} },
		Timeline{0.6, ShakeScreen{ xx = 1, yy = 2}},
	}
}

local EnterRoom = ComposeAction {
	id = 5,
	params = {"self", "door", "result"},
	children = Seq {
		Timeline{0.5, {class="FindKey", }},
		Timeline{1, SetVariable("result", OpenDoor{params={"door"}})},
		IF{params={"result"}, exec=Enter{params={"door"},}}
	}
}


local Skill1 = Skill {
	id = 6,
	name="青云门大招",
	actions = {
		{ class='Attack', time = 0.5,  },
		{ class='ShakeScreen', time=1.5,},
	}
}

local Skill2 = Skill {
	id=7,
	name="青云门大招",

	chidren = Seq {
		Timeline{0.5, { class='Attack'}},
		Timeline{1.5, { class='ShakeScreen'}},
	}
}



	PerformSkill({params={agent=GetAgent {params={"agentid"},}, skillid=ConstInt(5)}})

local Attack
local ShakeScreen
local CreateMonster
local PerformSkill
local Param
local Var
local IF2
local OR
local AND
local Timelines
local Seq

local function EctypeScript(self)
	local agentid = CreateMonster(232)
	local skillid = 1070
	PerformSkill(agentid, skillid)
end

local Skill
local function TestSkill()
	local self = Param("self")
	Timeline(0.5, Attack{ attackid = 7}(self))
	Timeline(1, ShakeScreen{x=1,y=2,z=3}(self))
end

local host = Param("host", "any") -- bool, int, long, float, double, string, any



local var1 = Var("result", "long")

Skill{
	id=7,
	name="青云门大招",

	children = Seq {
		Timeline(1, Attack{ attackid = 7}.params(var1).retval(var1)),
		Timelines{
			{0.5, Attack{}},
			{1, ShakeScreen{}},
			{1.2, Attack{}},
		},
		var1.set(10),
		IF2(AND(var1.plus(7).gt(5), var1.lt(10)),
			Attack{},
			Attack{}
		),
		Timeline(2, ShakeScreen{x=1,y=2,z=3}),
		Paraller{
			Attack{},
			Attack{},
		},
		Fork(Attack{attackid=5}),
	}
}

Skill {
	id = 8,
	name = "xxx",
	chidren = Timelines {
		{0, Attack{}},
		{1, ShakeScreen{}},
	}
}


Ectype {
	id = 5,
	xx = "afs",

	script = Seq {
		Dialog{},
		Show{id="xxx", var="saf"},
		CreatMonsters{ },
		WaitAllMonsterDead{},
		Hide{id="xxx"},
		CreateMonsterThenWait{},
	}
}

Gt("var", Add("var", 5))
--[[
class Seq
{
	Object host;
	int var1;

	public async void exec()
	{

	}
}
 ]]
