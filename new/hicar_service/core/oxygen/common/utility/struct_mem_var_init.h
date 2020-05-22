#pragma once 

#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"

#define struct_mem_var_init(init_struct, ...) \
    [&] () { init_struct $;  __VA_ARGS__; return $; }()

#define struct_mem_var_init_with_constructor_args(init_struct, arg, ...) \
    [&] () { init_struct $(arg);  __VA_ARGS__; return $; }()
