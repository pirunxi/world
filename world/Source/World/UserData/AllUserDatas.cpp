#include "AllUserDatas.h"
#include "Logger.h"
#include "Net/NetWork.h"
#include "LuaManager.h"

namespace UserData
{

	void RegisterBase(sol::state_view& lua)
	{

		lua["print"] = [](const sol::variadic_args& args)
		{
			std::stringstream ss;
			if (args.size() > 0)
			{
				ss << args[0].as<std::string>();
				for (auto it = ++args.begin(); it != args.end(); ++it)
				{
					ss << "\t" << it->as<std::string>();
				}
			}
			std::string text = ss.str();
			Logger(Log, TEXT("%s"), UTF8_TO_TCHAR(text.c_str()));
		};

		lua["trace"] = [](const sol::variadic_args& args)
		{
			std::stringstream ss;
			if (args.size() > 0)
			{
				ss << args[0].as<std::string>();
				for (auto it = ++args.begin(); it != args.end(); ++it)
				{
					ss << "\t" << it->as<std::string>();
				}
			}
			std::string text = ss.str();
			Logger(Log, TEXT("%s"), UTF8_TO_TCHAR(text.c_str()));
		};

	}

	class Test
	{
	public:
		int a;
		int b;

		static void hello(int x, int y)
		{
			Logger(Log, TEXT("X=%d, Y=%d"), x, y);
		}

		void print()
		{
			Logger(Log, TEXT("a=%d,b=%d"), a, b);
		}

	};
	void Register_NetWork(sol::state_view& lua)
	{
		lua.new_usertype<LuaManager>("LuaManager",
			"GetInstance", &LuaManager::GetInstance,
			"CreateSession", &LuaManager::CreateSession
			);
		lua.new_usertype<Test>("Test",
			"a", &Test::a,
			"b", &Test::b,
			"print", &Test::print,
			"hello", &Test::hello
			);
	}

	void RegisterAll(sol::state_view& lua)
	{
		RegisterBase(lua);
		Register_NetWork(lua);
	}
}