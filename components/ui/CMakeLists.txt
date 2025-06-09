file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/ui/*.c)

idf_component_register(
    SRCS
    ${SOURCES}
    INCLUDE_DIRS
    "./"
    REQUIRES lvgl
)