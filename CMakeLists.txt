cmake_minimum_required(VERSION 3.16)

project(GtkPopups VERSION 0.0.1  LANGUAGES C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
#pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)
set(GRESOURCE_C     popup_samples.gresource.c)
set(GRESOURCE_XML   popup_samples.gresource.xml)

set(PROJECT_TARGET      popup_samples)
set(PROJECT_SOURCE_DIR  ./src)
set(PROJECT_BINARY_DIR  ./target)

set(CSS_TARGET popup_samples_style.css)

set(PROJECT_RESOURCE_DIR    ${CMAKE_SOURCE_DIR}/resources)
set(STYLE_DIR               ${PROJECT_RESOURCE_DIR}/style)
set(SCSS_SOURCE             ${STYLE_DIR}/scss/popup_samples_style.scss)
set(CSS_OUTFILE             ${STYLE_DIR}/${CSS_TARGET})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_compile_options(${CMAKE_C_FLAGS} -Wall -Wextra -rdynamic)
add_link_options(-rdynamic)

#add_custom_command(OUTPUT ${CSS_OUTFILE} COMMAND sassc -t expanded ${SCSS_SOURCE} ${CSS_OUTFILE})

# This will force a rebuild of the css by adding a dummy target that doesn't exists
# Again, that's to the oracles at SO:
# https://stackoverflow.com/questions/13920072/how-to-always-run-command-when-building-regardless-of-any-dependency
if(EXISTS ${CMAKE_BINARY_DIR}/__dummy_force_rebuild__.unk)
    message(FATAL_ERROR "File \"${CMAKE_BINARY_DIR}/__dummy_force_rebuild__.unk\" found. \
    And is only there to force css rebuild, delete it!")
endif ()
add_custom_target(custom_target_that_always_runs ALL
                  DEPENDS ${CMAKE_BINARY_DIR}/__dummy_force_rebuild__.unk)
add_custom_command(OUTPUT
                   ${CMAKE_BINARY_DIR}/__dummy_force_rebuild__.unk
                   ${CSS_OUTFILE}
                   COMMAND sassc -t expanded ${SCSS_SOURCE} ${CSS_OUTFILE})

# This will build our resource bundle
# From: https://stackoverflow.com/questions/63697778/how-to-use-glib-compile-resources-with-cmake
add_custom_command(
      OUTPUT ${GRESOURCE_C}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMAND ${GLIB_COMPILE_RESOURCES}
      ARGS
      --generate-source
      --target=${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
      ${PROJECT_RESOURCE_DIR}/${GRESOURCE_XML}
      VERBATIM
      MAIN_DEPENDENCY ${PROJECT_RESOURCE_DIR}/${GRESOURCE_XML}
      DEPENDS
      ${PROJECT_RESOURCE_DIR}/popup_samples.glade
      ${CSS_OUTFILE}
)
add_custom_target(popup_samples-resource DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C})

set(MY_SOURCES
    src/main.c
    src/app_globals.h
    src/utils/parse_args.c
    src/utils/parse_args.h
    src/utils/logging.c
    src/utils/logging.h
    src/utils/sys_interface.c
    src/utils/sys_interface.h
    src/interfaces/app_interface.c
    src/interfaces/app_interface.h
    src/views/ui_builder.c
    src/views/ui_builder.h
    src/views/css_styler.c
    src/views/css_styler.h
    src/views/root_window.c
    src/views/root_window.h
    src/views/composites/two_button_popup_overlay.c
    src/views/composites/two_button_popup_overlay.h
    src/views/composites/one_button_acknowledge.c
    src/views/composites/one_button_acknowledge.h
    src/views/composites/validated_entry.c
    src/views/composites/validated_entry.h
)

include_directories(${PROJECT_SOURCE_DIR})
configure_file("${PROJECT_SOURCE_DIR}/version.h.in" "${PROJECT_SOURCE_DIR}/version.h")

add_executable(${PROJECT_TARGET} ${MY_SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C})

target_include_directories(${PROJECT_TARGET} PUBLIC "${PROJECT_BINARY_DIR}")
target_link_libraries(${PROJECT_TARGET} ${GTK3_LIBRARIES})

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C} PROPERTIES GENERATED TRUE )
add_dependencies(${PROJECT_TARGET} popup_samples-resource)