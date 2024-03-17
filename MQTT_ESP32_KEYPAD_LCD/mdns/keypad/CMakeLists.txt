if(IDF_VERSION_MAJOR AND IDF_VERSION_MAJOR GREATER 3)
    idf_component_register(SRCS "keypad.c"
        INCLUDE_DIRS "."
        REQUIRES ${req} driver)
else()
    set(COMPONENT_SRCDIRS .)
    set(COMPONENT_REQUIRES log)
    register_component()
endif()