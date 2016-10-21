# 导入Eigen库
# ===================================================
SET(EIGEN_EMBEDDED_INCLUDE_DIR "${OPENSLAM_SOURCE_DIR}/3rdparty/eigen3/" CACHE PATH "Eigen path for embedded use")

MARK_AS_ADVANCED(EIGEN_EMBEDDED_INCLUDE_DIR)

# 默认情况下使用系统提供的版本，如果pkg-config说明系统已经存在
SET(DEFAULT_EIGEN_USE_EMBEDDED_VERSION ON)
IF(PKG_CONFIG_FOUND)
	PKG_CHECK_MODULES(PKG_EIGEN3 ${_QUIET} eigen3)	# 通过pkg-config找到eigen:
	IF(PKG_EIGEN3_FOUND)
		# 使用系统的版本:
		SET(DEFAULT_EIGEN_USE_EMBEDDED_VERSION OFF)
	ENDIF(PKG_EIGEN3_FOUND)
ENDIF(PKG_CONFIG_FOUND)

SET(EIGEN_USE_EMBEDDED_VERSION ${DEFAULT_EIGEN_USE_EMBEDDED_VERSION} CACHE BOOL "Use embedded Eigen3 version or system version")
IF (EIGEN_USE_EMBEDDED_VERSION)
	# 获得内部版本的include目录:
	SET(OPENSLAM_EIGEN_INCLUDE_DIR "${EIGEN_EMBEDDED_INCLUDE_DIR}")
ELSE(EIGEN_USE_EMBEDDED_VERSION)
	# 获得系统环境中Eigen库的目录:
	IF(NOT PKG_CONFIG_FOUND)
		MESSAGE(SEND_ERROR "pkg-config is required for this operation!")
	ELSE(NOT PKG_CONFIG_FOUND)
		# 通过pkg-config找到eigen:
		PKG_CHECK_MODULES(PKG_EIGEN3 ${_QUIET} eigen3)
		IF(PKG_EIGEN3_FOUND)
			SET(OPENSLAM_EIGEN_INCLUDE_DIR "${PKG_EIGEN3_INCLUDE_DIRS}")
		ELSE(PKG_EIGEN3_FOUND)
			MESSAGE(SEND_ERROR "pkg-config was unable to find eigen3: install libeigen3-dev or enable EIGEN_USE_EMBEDDED_VERSION")
		ENDIF(PKG_EIGEN3_FOUND)
	ENDIF(NOT PKG_CONFIG_FOUND)
ENDIF(EIGEN_USE_EMBEDDED_VERSION)

#添加include目录到路径
INCLUDE_DIRECTORIES("${OPENSLAM_EIGEN_INCLUDE_DIR}")
IF(EXISTS "${OPENSLAM_EIGEN_INCLUDE_DIR}/unsupported/")
	INCLUDE_DIRECTORIES("${OPENSLAM_EIGEN_INCLUDE_DIR}/unsupported/")
ENDIF(EXISTS "${OPENSLAM_EIGEN_INCLUDE_DIR}/unsupported/")

# 创建变量给最后的总结输出
SET(CMAKE_OPENSLAM_HAS_EIGEN 1)        # Eigen库是必须的

# 对使用内部版本还是系统版本做一个标识
IF(EIGEN_USE_EMBEDDED_VERSION)
	SET(EIGEN_USE_EMBEDDED_VERSION_BOOL 1)
	SET(CMAKE_OPENSLAM_HAS_EIGEN_SYSTEM 0)
ELSE(EIGEN_USE_EMBEDDED_VERSION)
	SET(EIGEN_USE_EMBEDDED_VERSION_BOOL 0)
	SET(CMAKE_OPENSLAM_HAS_EIGEN_SYSTEM 1)
ENDIF(EIGEN_USE_EMBEDDED_VERSION)

# 检测Eigen的版本，为了最后总结的时候显示
SET(EIGEN_VER_H "${OPENSLAM_EIGEN_INCLUDE_DIR}/Eigen/src/Core/util/Macros.h")
IF (EXISTS ${EIGEN_VER_H})
	file(READ "${EIGEN_VER_H}" STR_EIGEN_VERSION)

	# 通过对 Macros.h 文件进行解析 匹配 "#define EIGEN_WORLD_VERSION  XX"

	STRING(REGEX MATCH "EIGEN_WORLD_VERSION[ ]+[0-9]+" CMAKE_EIGEN_VERSION_NUMBER_MAJOR "${STR_EIGEN_VERSION}")
	STRING(REGEX MATCH "[0-9]+" CMAKE_EIGEN_VERSION_NUMBER_MAJOR "${CMAKE_EIGEN_VERSION_NUMBER_MAJOR}")

	STRING(REGEX MATCH "EIGEN_MAJOR_VERSION[ ]+[0-9]+" CMAKE_EIGEN_VERSION_NUMBER_MINOR "${STR_EIGEN_VERSION}")
	STRING(REGEX MATCH "[0-9]+" CMAKE_EIGEN_VERSION_NUMBER_MINOR "${CMAKE_EIGEN_VERSION_NUMBER_MINOR}")

	STRING(REGEX MATCH "EIGEN_MINOR_VERSION[ ]+[0-9]+" CMAKE_EIGEN_VERSION_NUMBER_PATCH "${STR_EIGEN_VERSION}")
	STRING(REGEX MATCH "[0-9]+" CMAKE_EIGEN_VERSION_NUMBER_PATCH "${CMAKE_EIGEN_VERSION_NUMBER_PATCH}")

	SET(OPENSLAM_EIGEN_VERSION "${CMAKE_EIGEN_VERSION_NUMBER_MAJOR}.${CMAKE_EIGEN_VERSION_NUMBER_MINOR}.${CMAKE_EIGEN_VERSION_NUMBER_PATCH}")

	IF($ENV{VERBOSE})
		MESSAGE(STATUS "Eigen version detected: ${OPENSLAM_EIGEN_VERSION}")
	ENDIF($ENV{VERBOSE})
ENDIF (EXISTS ${EIGEN_VER_H})

