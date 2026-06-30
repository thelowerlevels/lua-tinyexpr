# tinyexpr for Lua

A fast, sandboxed mathematical expression evaluator for Lua based on [TinyExpr](https://codeplea.com/tinyexpr) written in C.

This module allows safe evaluation of mathematical expressions without using `load()` or Lua bytecode execution. These methods tend to be slower and more dangerous.

It supports both:
- one-shot evaluation (`interp`)
- compiled expressions with persistent state (`compile` + `eval`)

The latter should be used when you need to evaluate an expression many times. The former should be used for one-time calculations.

---

## Documentation

### Usage

Using `lua-tinyexpr` is very simple. There are only 3 functions available.

```lua
local te = require("te")

-- te.interp is used to quickly get a result by evaluating the compiled expression
print(te.interp("5+5")) -- 10.0

local expr = te.compile("10 / 5 * 8 - 4") -- This returns the compiled expression, not the result!
local eval = te.eval(expr) -- Evaluate compiled expression
print(eval) -- 12.0

-- You can also do more complicated math with variables and pre-defined functions
local expr = te.compile("x + 5 / y * z + 12 - 5", {x = 4, y = 15, z = 9}, 3)
local eval = te.eval(expr) -- 14.0

-- Is a pesky user trying to execute malicious code? Not allowed! 
load("local f=io.open('/etc/passwd', 'r'); return f:read('*a')")() -- If you pass in user input, you risk arbitrary code execution.
res, err = te.interp("local f=io.open('/etc/passwd', 'r'); return f:read('*a')") -- This won't work! 
if not res then
  print("An error occurred")
end
```

---

### Functions

`te.interp(expr)`

This function interprets an expression and evaluates it. It is equivalent to `te.compile()` and `te.eval()` for the expression. It returns the result of the expression.

`te.compile(expr, args, count)`

This function returns a compiled expression from the expression. It is compiled into a "syntax tree". It returns the *expression tree* itself.

`te.eval(expr)`

This function returns the result of the expression from the expression tree.

---

## Features

- Safe, sandboxed expression evaluation
    - `load()` is *unsafe*. It sets up a Lua environment to run the chunk. This is unsafe and without validation, it can lead to Lua code execution. [Trying to sandbox `load()`](https://stackoverflow.com/questions/1224708/how-can-i-create-a-secure-lua-sandbox) is certainly difficult and as we all know, **never ever blacklist**!
- Based on a tiny, tested and reliable expression parser
- Written in C to fully utilize the speed and reliability of the language
- Support for mathematical variables from Lua tables
- Persistent expression tree objects (userdata)
- Works with Lua 5.1+ and LuaJIT
    - If it doesn't work with LuaJIT, you may need to compile it from source and use the header files from LuaJIT.

---

## Installation

### LuaRocks

```bash
luarocks install lua-tinyexpr
````

### Compile from source

There is a Makefile and you can use it to compile the project.

You can clone the repository and run `make`. To remove the compiled file, you can run `make clean`.
