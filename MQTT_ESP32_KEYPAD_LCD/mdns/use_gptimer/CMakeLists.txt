set(app_src use_gptimer.c)
set(pri_req)

idf_component_register(SRCS ${app_src}
                           INCLUDE_DIRS "."
                           REQUIRES ${pri_req} driver)