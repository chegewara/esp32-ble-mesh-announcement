file(GLOB_RECURSE SRC_FILES COMPONENT_SRCS
  LIST_DIRECTORIES false
  "core/*.cpp"
  "models/*.cpp"
  "models/*.c"
)


idf_component_register(SRCS ${SRC_FILES} "mesh_callbacks.cpp"
                    INCLUDE_DIRS include 
                                 include/models/foundation
                                 include/models/generic
                                 include/models/light
                                 include/models/scene
                                 include/models/sensor
                                 include/models/vendor
                    PRIV_REQUIRES log bt nvs_flash
        )

        
target_compile_options(${COMPONENT_LIB} PUBLIC -Wno-missing-field-initializers -Wno-deprecated-declarations -Wno-unused-variable)
