cmake_minimum_required(VERSION 3.5)

project(Calculator VERSION 2.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS view/forms)

set(QT_LIBS_REQUIRED Widgets PrintSupport)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_LIBS_REQUIRED})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_LIBS_REQUIRED})

include_directories(model view view/thirdparty controller common)

file(GLOB_RECURSE PROJECT_SOURCES *.cc *.h *.ui)
set(EXCLUDED_DIRS tests)
foreach(dir ${EXCLUDED_DIRS})
    list(FILTER PROJECT_SOURCES EXCLUDE REGEX "${dir}.*")
endforeach()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${PROJECT_SOURCES})
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
    else()
        add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
    endif()
endif()

foreach(lib ${QT_LIBS_REQUIRED})
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::${lib})
endforeach()

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
