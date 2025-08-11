cmake_minimum_required(VERSION 3.16)

if(DEFINED ENV{VCPKG_ROOT})
	set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
	set(VCPKG_TRIPLET "x64-windows-static" CACHE STRING "")
endif()

project(Calculator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(POLICY CMP0071)
	cmake_policy(SET CMP0071 NEW)
endif()

#======================================================================
#===================== WXWIDGETS SETUP ================================
#======================================================================

option(wxWidgets_USE_SHARED OFF)
option(wxWidgets_USE_UNICODE ON)
option(wxWidgets_USE_DEBUG OFF)

set(wxWidgets_ROOT_DIR ${CMAKE_SOURCE_DIR}/third_party)
set(wxWidgets_LIB_DIR ${CMAKE_SOURCE_DIR}/third_party/lib)

set(wxWidgets_POSSIBLE_PATHS
 ${wxWidgets_ROOT_DIR}
 $ENV{WXWIN}
 $ENV{wxWidgets_ROOT_DIR}
 "C:/wxWidgets"
 "C:/wx"
 "C:/Program Files/wxWidgets"
 "C:/Program Files (x86)/wxWidgets"
)

find_package(wxWidgets REQUIRED COMPONENTS core base)

if(wxWidgets_FOUND)
	include(${wxWidgets_USE_FILE})
	
	set(wxWidgets_VERSION_MAJOR 3)
	set(wxWidgets_VERSION_MINOR 2)
	set(wxWidgets_VERSION_PATCH 4)
	set(wxWidgets_VERSION "${wxWidgets_VERSION_MAJOR}.${wxWidgets_VERSION_MINOR}.${wxWidgets_VERSION_PATCH}")

	set(wxWidgets_DEFINITIONS
		-D__WXMSW__
		-DwxUSE_UNICODE=1
		-DUNICODE
		-D_UNICODE
		-DWXUSINGDLL=$<IF:$<BOOL:${wxWidgets_USE_SHARED}>,1,0>
	)
	
	if(NOT wxWidgets_USE_SHARED)
		list(APPEND wxWidgets_DEFINITIONS -DwxUSE_DLL=0)
	endif()
	
endif()

set(SOURCES
    src/main.cpp
    src/core/app.cpp
    src/ui/main_window.cpp
    src/ui/button_panel.cpp
    src/utils/helpers.cpp
)

set(HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include/core
	${CMAKE_CURRENT_SOURCE_DIR}/include/ui
	${CMAKE_CURRENT_SOURCE_DIR}/include/utils
)

set(HEADERS_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/include/core/app.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/ui/button_panel.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/ui/main_window.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/utils/helpers.h
)

source_group("Headers files" FILES ${HEADERS_FILES})


#======================================================================
#===================== CREATING AN EXECUTABLE FILE ====================
#======================================================================
if(WIN32)
	add_executable(${PROJECT_NAME} WIN32 ${HEADERS_FILES} ${SOURCES})
else()
	add_executable(${PROJECT_NAME} ${HEADERS_FILES} ${SOURCES})
endif()
	
#======================================================================
#===================== CONFIGURING COMPILATION ========================
#======================================================================

target_include_directories(${PROJECT_NAME} PRIVATE

	${HEADERS}
	${wxWidgets_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})

target_compile_definitions(${PROJECT_NAME} PRIVATE ${wxWidgets_DEFINITIONS})


if(WIN32)
	set_target_properties(${PROJECT_NAME} PROPERTIES 
		WIN32_EXECUTABLE ON
	)
			
	target_compile_definitions(${PROJECT_NAME} PRIVATE
		_WIN32_WINNT=0x0A00
		_WINVER=0x0A00
		WIN32_LEAN_AND_MEAN
		NOMINMAX
		_CRT_SECURE_NO_WARNINGS
		_WINSOCK_DEPRECATED_NO_WARNINGS
	)

	if(NOT wxWidgets_USE_SHARED)
		if(MSVC)
			set_target_properties(${PROJECT_NAME} PROPERTIES
				MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
			)
		endif()
	endif()
endif()

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE
		/W4
		/utf-8
		/wd4996
		/wd4251
		/wd4275
	)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE
		-Wall
		-Wextra
		-Wpedantic
		-Wno-unused-parameter
		-Wshadow
	)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(${PROJECT_NAME} PRIVATE
		DEBUG_BUILD
		_DEBUG
	)
else()
	target_compile_definitions(${PROJECT_NAME} PRIVATE
		RELEASE_BUILD
		NDEBUG
	)
endif()

#======================================================================
#===================== ADDITIONAL OPTIONS =============================
#======================================================================
option(BUILD_TEST "Build tests" OFF)

if(BUILD_TEST)
	enable_testing()
	if(EXISTS "${CMAKE_SOURCE_DIR}/tests/CMakeLists.txt")
		add_subdirectory(tests)
	endif()
endif()	

#======================================================================
#===================== INFORMATIONAL MESSAGES =========================
#======================================================================

message(STATUS "=== PROJECT CONFIGURATION ===")
message(STATUS "Project: 			${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Platform: 			${CMAKE_SYSTEM_NAME}")
message(STATUS "Build type: 		${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: 			${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Standard:		${CMAKE_CXX_STANDARD}")
message(STATUS "Build tests: 		${BUILD_TEST}")
message(STATUS "")
message(STATUS "=== WXWIDGETS CONFIGURATION ===")
if(wxWidgets_FOUND)
	message(STATUS "wxWidgets version: ${wxWidgets_VERSION}")
	message(STATUS "wxWidgets root: ${wxWidgets_ROOT_DIR}")
	if(wxWidgets_LIB_DIR)
		message(STATUS "wxWidgets lib dir: ${wxWidgets_LIB_DIR}")
	endif()
	message(STATUS "wxWidgets shared: ${wxWidgets_USE_SHARED}")
	message(STATUS "wxWidgets Unicode: ${wxWidgets_USE_UNICODE}")
	message(STATUS "wxWidgets debug: ${wxWidgets_USE_DEBUG}")
	message(STATUS "wxWidgets libs: ${wxWidgets_LIBRARIES}")
else()
		message(WARNING "wxWidgets not found! Build will fail.")
endif()