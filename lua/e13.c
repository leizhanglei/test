#include <stdio.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

lua_State *L;

int luaadd(int x, int y)
{
	int sum;
	//function name
	lua_getglobal(L, "add");

	//first argument
	lua_pushnumber(L, x);

	//second argument
	lua_pushnumber(L, y);

	//call function
	lua_call(L, 2, 1);

	//get result
	sum = (int)lua_tonumber(L, -1);

	lua_pop(L, 1);
	
	return sum;
}

int main(int argc, char* argv[])
{
	int sum;
	//load lua base libraries
	L = luaL_newstate();
	
	luaL_openlibs(L);
	//load the script
	luaL_dofile(L, "e12.lua");

	//call the fuction
	sum = luaadd(10, 15);

	printf("the number is %d\n", sum);
	lua_close(L);
}


