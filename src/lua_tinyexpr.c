#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdlib.h>
#include <string.h>
#include "tinyexpr.h"

typedef struct {
  te_expr *expr; // Expression tree
  te_variable *vars; // Expression variables
  double *storage; // Expression storage
  int count; // Expression variable count
} LuaExpr;

static int l_te_interp(lua_State *L) {
  const char *expr = luaL_checkstring(L, 1);
  
  int err = 0;
  double result = te_interp(expr, &err); 

  if (err != 0) {
    lua_pushnil(L);
    lua_pushinteger(L, err);
    return 2;
  }

  lua_pushnumber(L, result);
  return 1;
}

static int l_te_compile(lua_State *L) {
  const char *expr = luaL_checkstring(L, 1);
  int count = (int)luaL_optinteger(L, 3, 0); 

  if (count > 0) {
    luaL_checktype(L, 2, LUA_TTABLE);
  }
  
  te_variable *vars = NULL;
  double *storage = NULL;

  // Convert the Lua table of variables and their values to a C array
  if (count > 0) {
    vars = malloc(sizeof(te_variable) * count);
    storage = malloc(sizeof(double) * count); 

    int i = 0;

    lua_pushnil(L);

    // Iterate over the Lua table and place variables with their values and data
    while (lua_next(L, 2) && i < count) {
      const char *name = lua_tostring(L, -2);
      double value = luaL_checknumber(L, -1);

      storage[i] = value;
      vars[i].name = strdup(name);
      vars[i].address = &storage[i];
      vars[i].type = TE_VARIABLE;
      vars[i].context = NULL;

      lua_pop(L, 1);
      i++;
    }
  }

  int err = 0;
  te_expr *n = te_compile(expr, vars, count, &err);

  if (!n) {
    // When there is an error, free all allocated resources
    if (vars) {
      for (int i = 0; i < count; i++) {
        free((void*)vars[i].name);
      }
      free(vars);
      free(storage);
    }

    lua_pushnil(L);
    lua_pushinteger(L, err);
    return 2;
  }

  LuaExpr *ud = lua_newuserdata(L, sizeof(LuaExpr));
  ud->expr = n;
  ud->vars = vars;
  ud->storage = storage;
  ud->count = count;

  luaL_getmetatable(L, "TE_EXPR");
  lua_setmetatable(L, -2);

  return 1;
}

static int l_te_eval(lua_State *L) {
  LuaExpr *tree = luaL_checkudata(L, 1, "TE_EXPR");
  lua_pushnumber(L, te_eval(tree->expr)); 
  return 1;
}

static int l_te_gc(lua_State *L) {
  LuaExpr *ud = luaL_checkudata(L, 1, "TE_EXPR");

  if (ud->expr) {
    te_free(ud->expr);
  }
  
  if (ud->vars) {
    for (int i = 0; i < ud->count; i++)
      free((void*)ud->vars[i].name);
    free(ud->vars);
  }

  if (ud->storage) {
    free(ud->storage);
  }
  
  return 0;
}

static const luaL_Reg te_lib[] = {
  {"interp", l_te_interp},
  {"compile", l_te_compile},
  {"eval", l_te_eval},
  {NULL, NULL}
};

int luaopen_te(lua_State *L) {
  luaL_newlib(L, te_lib);

  luaL_newmetatable(L, "TE_EXPR");
  lua_pushcfunction(L, l_te_gc);
  lua_setfield(L, -2, "__gc");
  lua_pop(L, 1);

  return 1;
}
