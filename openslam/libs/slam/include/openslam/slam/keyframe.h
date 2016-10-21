#ifndef OPENSLAM_SLAM_KEYFRAME_H_
#define OPENSLAM_SLAM_KEYFRAME_H_

#include <openslam/slam/frame.h>
#include <mutex>

namespace openslam
{
	namespace slam
	{
		class MapPoint;
	
		class SLAM_IMPEXP KeyFrame : public Frame
		{
		public:
			KeyFrame(const Frame &frame);
			~KeyFrame();

			/** \brief ������ȣ�ת����ǰ֡������ϵ�£���������������Ӧ��map point��z����ֵ����Ϊ���ֵ
			*/
			float computeSceneMedianDepth();

			/** \brief ���ڼ���ùؼ�֡������������Ӧ��map point���۲쵽�������ؼ�֡����
			* �����ؼ�֡������\param min_obs ��ͬmap point
			*/
			int trackedMapPoints(const int &min_obs);

			/** \brief ���ùؼ�֡�Ƿ���õı�ʶ
			*/
			void setBadFlag();

			/** \brief �õ��ؼ�֡�Ƿ����
			*/
			bool isBad();

			/** \brief �ؼ�֡���������Ӧ���е�map point
			*/
			std::vector<MapPoint*> getMapPointMatches();
		public:
			static long unsigned int keyframe_counter_;//!< �����ؼ�֡�ļ���������������֡��Ψһid
			long unsigned int keyframe_id_;//!< �ؼ�֡��Ψһid
			cv::Mat gba_Tcw_;
			long unsigned int gba_for_keyframe_num_;
			long unsigned int local_ba_for_keyframe_id_;
			long unsigned int fixed_ba_for_keyframe_id_;
		protected:
			std::mutex mutex_pose_;
			std::mutex mutex_features_;
			std::mutex mutex_connections_;
			bool is_bad_;

		};
	}
}

#endif // OPENSLAM_SLAM_KEYFRAME_H_
