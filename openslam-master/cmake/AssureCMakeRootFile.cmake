#  ��ҪΪ��ȷ����ǰCMakeLists.txt�Ѿ���ӵ���ROOT��CMakeLists.txt�У���ֹ��Դ�������Ӱ��
#  Usage:  INCLUDE(AssureCMakeRootFile)
#

IF(NOT OPENSLAM_SOURCE_DIR)
	MESSAGE(FATAL_ERROR "ERROR: Do not use this directory as 'source directory' in CMake, but the ROOT directory of the OPENSLAM source tree.")
ENDIF(NOT OPENSLAM_SOURCE_DIR)


