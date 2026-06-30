# TinyExpr for Lua (lua-tinyexpr)

A fast, sandboxed mathematical expression evaluator for Lua based on [TinyExpr](https://codeplea.com/tinyexpr) written in C.

This module allows safe evaluation of mathematical expressions without using `load()` or Lua bytecode execution.

It supports both:
- one-shot evaluation (`interp`)
- compiled expressions with persistent state (`compile` + `eval`)

The latter should be used when you need to evaluate an expression many times. The former should be used for one-time calculations.

---

## Features

- Safe, sandboxed expression evaluation (no Lua execution)
- Based on TinyExpr C parser
- Fast compiled expression evaluation
- Support for variables from Lua tables
- Persistent expression tree objects (userdata)
- Works with Lua 5.1+ and LuaJIT

---

## Installation

### LuaRocks

```bash
luarocks install lua-tinyexpr
````
