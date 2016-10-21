#ifndef OPENSLAM_SLAM_SYSTEM_MONO_H_
#define OPENSLAM_SLAM_SYSTEM_MONO_H_
#include <openslam/slam/map.h>
#include <openslam/slam/tracking_base.h>

namespace openslam
{
	namespace slam
	{
		/**���û��ӿڣ��û������ṩͼ��Ȼ��õ�ʵʱ���λ�ˣ��켣���ݣ���ͼ����*/
		class SLAM_IMPEXP System
		{
		public:
			
			System(const std::string &voc_file_name, const std::string &settings_file_name, const bool is_use_viewer = true);
			~System();

			/**����ͼ�񣬵õ����*/
			void addImage(const cv::Mat& img, double timestamp);

			
		protected:

			ORBVocabulary* orb_vocabulary_;//!< ����������ڵ���ʻ��
			TrackingBase* tracker_;//!<���ٵĻ��࣬���ݲ�ͬ�Ĵ����������ٲ�ͬ
			Map *map_;//!<��ͼ��Ϣ


		};
	}
}

#endif // OPENSLAM_SLAM_SYSTEM_MONO_H_
