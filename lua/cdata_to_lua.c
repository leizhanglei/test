#include <stdio.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

lua_State *L;


typedef struct _user_data_
{
	int		age;
	char*	name;
	void (*_data_to_lua_)(lua_State* L, struct _user_data_* udata);
	void (*_lua_to_data_)(lua_State* L, struct _user_data_* udata);
	void (*_show_data_)(struct _user_data_* udata);
} user_data;

void show_data(struct _user_data_* udata)
{
	printf("age : %d\n", udata->age);
	printf("name : %s\n", udata->name);
}

void data_to_lua(lua_State* L, struct _user_data_* udata)
{
	lua_newtable(L);
	lua_pushstring(L, "age");
	lua_pushnumber(L, udata->age);
	lua_settable(L, -3);

	lua_pushstring(L, "name");
	lua_pushstring(L, udata->name);
	lua_settable(L, -3);
}

void lua_to_data(lua_State* L, struct _user_data_* udata)
{
	lua_pushnil(L);
	char* key;
	char* svalue;
	int   ivalue;

	lua_next(L, -2);
	ivalue = lua_tonumber(L, -1);
	key = lua_tostring(L, -2);
	udata->age = ivalue;

	lua_pop(L, 1);

	lua_next(L, -2);
	svalue = lua_tostring(L, -1);
	key = lua_tostring(L, -2);
	udata->name = svalue;
}

int lua_modify_data(user_data* udata)
{
	//function name
	lua_getglobal(L, "modify_data");

	udata->_data_to_lua_(L, udata);
	//call function
	lua_call(L, 1, 1);

	printf("after modify \n");
	user_data datatmp;
	udata->_lua_to_data_(L, &datatmp);
	udata->_show_data_(&datatmp);
}

int main(int argc, char* argv[])
{
	int sum;
	//load lua base libraries
	L = luaL_newstate();
	
	luaL_openlibs(L);
	//load the script
	luaL_dofile(L, "cdata_to_lua.lua");

		
	user_data udata;
	udata.age = 30;
	udata.name = strdup("zhanglei");
	udata._data_to_lua_ = data_to_lua;
	udata._lua_to_data_ = lua_to_data;
	udata._show_data_ = show_data;
	//call the fuction
	lua_modify_data(&udata);

	free(udata.name);

	lua_close(L);
}

