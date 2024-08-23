add_rules("mode.debug")

includes("xf_utils/xmake.lua")

-- xf_task所有的内容
function add_xf_hal() 
    add_xf_utils("./xf_utils")
    add_files("src/**.c")
    add_includedirs("src")
end 

-- xf_task移植的内容
function add_port() 
    add_files("port/*.c")
    add_includedirs("port")
end

-- 模板化添加示例工程
function add_target(name) 
    target(name)
        set_kind("binary")
        add_cflags("-Wall")
        add_cflags("-std=gnu99 -O0")
        add_files(string.format("example/%s/*.c", name))
        add_includedirs(string.format("example/%s", name))
        add_xf_hal()
        add_port()
end 

add_target("gpio")
add_target("tim")
add_target("pwm")
add_target("adc")
add_target("dac")
add_target("uart")
add_target("i2c")
add_target("spi")



