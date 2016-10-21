#ifndef OPENSLAM_SLAM_MAP_POINT_H_
#define OPENSLAM_SLAM_MAP_POINT_H_

#include <openslam/slam/link_pragmas.h>
#include <list>
#include <memory>
#include <mutex>
#include <opencv2/core/core.hpp>

namespace openslam
{
	namespace slam
	{
		class Feature;

		class SLAM_IMPEXP MapPoint
		{
		public:
			MapPoint(const cv::Mat &pos, Feature * cur_obs);
			~MapPoint();

			/** \brief map point ����������ϵ�е�λ��
			*/
			cv::Mat getWorldPosition();

			/** \brief ����map point ����������ϵ�е�λ��
			*/
			void setWorldPosition(const cv::Mat &pos);

			/**��ӹ۲⵽�Ķ�Ӧ����*/
			void addFeatureRef(Feature* ftr);

			/** \brief �����ͬһ��mappoint���ж�Ӧ�����������������ӽ��д���Ѱ�����������
			*/
			void computeDistinctiveDescriptors();

			/** \brief �õ�map point ��Ӧ�����������
			*/
			cv::Mat getDescriptor();

			/** \brief ����map point�ķ���������Ӧ���ܵ������С���
			*/
			void updateNormalAndDepth();

			/**�������ø�mappoint�Ƿ�������*/
			void setBadFlag();

			/**���ڻ�ȡ��mapoint�Ƿ�������*/
			bool isBad();

			/**���ڻ�ù۲⵽��map point�Ĺؼ�֡�ĸ���*/
			int observationsNum();

			/**�õ���map point����Ӧ��������*/
			std::list<Feature*> getObservations() { return obs_; }
		public:
			long unsigned int id_;                                   //!< ��Ψһ��id
			static long unsigned int map_point_counter_;             //!< ������ļ�������������Ψһ��id
			cv::Mat world_position_;                                 //!<����������ϵ�е�λ��
			cv::Mat normal_vector_;                                  //!<ƽ���۲ⷽ��
			std::list<Feature*>   obs_;                              //!< ��Ӧ��������������
			Feature * cur_obs_;//!<��Ӧ���map point �ĵ�ǰ����
			
			std::mutex mutex_features_;//!< �Ե��Ӧ��������д���п���
			std::mutex mutex_position_;//!< �Ե��Ӧ��λ�ý��ж�д����
			cv::Mat descriptor_;//!<�õ���map point ��Ӧ����������ӣ��������ؼ�֡�Ըõ�����������С��
			float min_distance_;//!<�߶Ȳ��䣬��Ч�۲������Сֵ
			float max_distance_;//!<�߶Ȳ��䣬��Ч�۲�������ֵ

			bool is_outlier_;//!<�Ƿ�ȷ��Ϊ���
			bool track_in_view_;//!<�Ƿ��ܱ���ǰ֡�۲⵽
			long unsigned int last_frame_seen_id_;//!< ���һ�ι۲⵽�õ��֡��id
			static std::mutex global_mutex_;

			cv::Mat gba_pos_;
			long unsigned int gba_for_keyframe_num_;

			long unsigned int local_ba_for_keyframe_id_;
		protected:
			int obs_num_;                                            //!< �۲쵽��map point�Ĺؼ�֡�ĸ���
			bool is_bad_;//!<�������ø�map point�Ƿ���Ч
		};

	}
}

#endif // OPENSLAM_SLAM_MAP_POINT_H_
