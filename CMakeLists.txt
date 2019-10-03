# Scans the current directory and returns a list of subdirectories.
#Third parameter is 1 if you want relative paths returned.
# Usage: list_subdirectories(the_list_is_returned_here C:/cwd 1)
macro(list_subdirectories retval curdir return_relative)
  file(GLOB sub-dir RELATIVE ${curdir} *)
  set(list_of_dirs "")
  foreach(dir ${sub-dir})
    if(IS_DIRECTORY ${curdir}/${dir})
      if (${return_relative})
        set(list_of_dirs ${list_of_dirs} ${dir})
      else()
        set(list_of_dirs ${list_of_dirs} ${curdir}/${dir})
      endif()
    endif()
  endforeach()
  set(${retval} ${list_of_dirs})
endmacro()

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()


#指定cmake的最小版本
cmake_minimum_required(VERSION 3.15)
#指定项目名称
project(ATSEA)
#查看当前目录下的所有源文件
#aux_source_directory(. DIR_SRCS)

#add_subdirectory(Actor)
#list_subdirectories(GWW "." 1)

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

set(AllFiles "")
FOREACH(subdir ${SUBDIRS})
	file(GLOB test1 "${subdir}/*.*")
	#list(APPEND ${DIR_SRCS} ${test1})
	#set(DIR_SRCS "${DIR_SRCS} ${test1}")
	source_group(${subdir} FILES ${test1})
	
	set(AllFiles ${test1})
ENDFOREACH()
#source_group("Tests1" FILES "D:/atsea/Code/ATSEA.cpp")
#source_group("Tests1" FILES "D:/atsea/Code/丢弃备份/RainCameraCone.cpp")
#source_group("Implementation" FILES array.cpp vector.cpp)

file(GLOB_RECURSE DIR_SRCS "*.cpp" "*.h")
#指定生成目标
add_executable(ATSEA ${DIR_SRCS})
message(${DIR_SRCS})
# 添加链接库
#target_link_libraries(ATSEA Actor)