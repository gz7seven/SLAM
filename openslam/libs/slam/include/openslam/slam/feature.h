/*************************************************************************
* �ļ����� frame
*
* ���ߣ� ���
* �ʼ��� fengbing123@gmail.com
* ʱ�䣺 2016/4/9
*
* ˵���� ���������м򵥷�װ
*************************************************************************/
#ifndef OPENSLAM_SLAM_FEATURE_H_
#define OPENSLAM_SLAM_FEATURE_H_
#include <openslam/slam/frame.h>
#include <openslam/slam/map_point.h>

namespace openslam
{
	namespace slam
	{
		class SLAM_IMPEXP Feature
		{
		public:
			Feature(Frame* frame, const cv::KeyPoint& keypoint, cv::Mat &descriptor);
			~Feature();
			/** ���������Ӧ��map point
			*/
			void addMapPointRef(MapPoint *map_point);

			/** �����������Ӧ��֡
			*/
			void addFrameRef(Frame *frame);

		private:
			void undistortKeyPoint();
		public:
			Frame*         frame_;              //!< ָ��ָ����������⵽����Ӧ��֡
			cv::KeyPoint   keypoint_;           //!< ÿ��������Ӧ��������
			cv::KeyPoint   undistored_keypoint_;//!< ���л������֮���������
			cv::Mat        descriptor_;         //!< ÿ��������Ӧ����������
			MapPoint*      map_point_;          //!< ָ��ָ���������Ӧ��3D��
		
		};
	}
}

#endif // OPENSLAM_SLAM_FEATURE_H_
