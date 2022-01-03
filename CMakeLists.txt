cmake_minimum_required(VERSION 3.0)

project(WifiClient)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Qt5 COMPONENTS Qml Quick LinguistTools REQUIRED)

macro(add_qt_resources source destination)
    file(GLOB_RECURSE QML_FILES ${source}/qml/*.qml)
    if (QML_FILES)
        file(COPY ${source}/qml DESTINATION ${destination})
    endif ()

    file(GLOB_RECURSE FONT_FILES ${source}/fonts/*.otf)
    if (FONT_FILES)
        file(COPY ${source}/fonts DESTINATION ${destination})
    endif ()

    file(GLOB_RECURSE IMAGE_FILES ${source}/images/*)
    if (IMAGE_FILES)
        file(COPY ${source}/images DESTINATION ${destination})
    endif ()

    file(GLOB_RECURSE TS_FILES ${source}/languages/*.ts)
    if (TS_FILES)
        set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION ${destination}/languages)
        qt5_create_translation(QM_FILES ${TS_FILES})
    else()
        set(QM_FILES "")
    endif ()

    string(REGEX REPLACE "(\\.|\\+|\\*)" "\\\\\\1" SOURCE_DIR_ESC ${source})
    string(REGEX REPLACE "(\\.|\\+|\\*)" "\\\\\\1" BINARY_DIR_ESC ${destination})
    set(RES_FILE_LIST "")
	FOREACH(RES_FILE ${FONT_FILES} ${IMAGE_FILES} ${QM_FILES} ${QML_FILES} ${CONFIG_FILE})
        string(REGEX REPLACE "^(${BINARY_DIR_ESC}|${SOURCE_DIR_ESC})/" "" RES_FILE_REL ${RES_FILE})
        set(RES_FILE_LIST "${RES_FILE_LIST}\n\t\t<file>${RES_FILE_REL}</file>")
    ENDFOREACH(RES_FILE)
    configure_file(application.qrc ${destination})
    configure_file(config.json ${destination})
endmacro()

add_qt_resources(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${CMAKE_CURRENT_BINARY_DIR}/application.qrc
)
target_include_directories(${PROJECT_NAME} SYSTEM BEFORE PUBLIC src)

target_link_libraries(${PROJECT_NAME} Qt5::Qml Qt5::Quick)
