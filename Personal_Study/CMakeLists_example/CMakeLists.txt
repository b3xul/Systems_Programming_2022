cmake_minimum_required(VERSION 3.19)
project(ProjectName)

set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

# check for boost (head only). If you need not head-only parts, you need to add COMPONENTS and then the list (like COMPONENTS filesystem program_options)
find_package(Boost REQUIRED )
if ( NOT Boost_FOUND )
    message(FATAL_ERROR "Boost libraries are require, but not found!")
else()
    include_directories(${Boost_INCLUDE_DIR})
endif()

#FetchContent expects the repo to be cmake compilable. If you are including a make or a head only library, you can configure and build by specifying how with CONFIGURE_COMMAND and BUILD_COMMAND
FetchContent_Declare(libraryName
        GIT_REPOSITORY https://github.com/<repo>
        GIT_TAG <specifig tag>
        CONFIGURE_COMMAND ""
        BUILD_COMMAND "")

FetchContent_GetProperties(libraryName)

# download and build only once
if(NOT libraryName_POPULATED)
    FetchContent_Populate(libraryName)
    add_library(libraryName INTERFACE) # INTERFACE is for head-only
endif()

add_executable(executable <list of files>)
target_include_directories(ExamDates PRIVATE ${libraryName_SOURCE_DIR}) # to include includes of fetched/local libraries
target_link_libraries(ExamDates Boost::filesystem Boost::program_options) #if you need to link something, such as from boost