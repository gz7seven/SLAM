/*************************************************************************
* �ļ����� tracking
*
* ���ߣ� ���
* �ʼ��� fengbing123@gmail.com
* ʱ�䣺 2016/4/9
*
* ˵���� �ο�rpg_svo,orb_slam2
*************************************************************************/
#ifndef OPENSLAM_SLAM_TRACKING_H_
#define OPENSLAM_SLAM_TRACKING_H_
#include <openslam/slam/frame.h>
#include <openslam/slam/map.h>

namespace openslam
{
	namespace slam
	{
		/** �����࣬��Ҫ���ڽ���֡��������֮��Ӧ�������̬�����������ʱ����һ���ؼ�֡������һ����
		*   ��map point�������������ʧ��ִ���ض�λ��
		*/
		class SLAM_IMPEXP TrackingBase
		{
		public:
			/** ����״̬
			*/
			enum TrackingState
			{
				TRACKING_SYSTEM_NOT_READY = -1,//״̬��Ҫ���ڵ���ʻ��Ĺ���
				TRACKING_NO_IMAGES_YET = 0,
				TRACKING_NOT_INITIALIZED = 1,
				TRACKING_OK = 2,
				TRACKING_LOST = 3
			};

			/**�������ٵ����ò�������ͬ�Ĵ���������ֵ��һ��*/
			struct Options
			{
				float track_motion_model_matcher_nnratio;//!<�ڸ����˶�ģ�͸��ٵ�ʱ��ƥ����С���������С����ֱ�ӵı�ֵ��ֵ
				float track_ref_keyframe_matcher_nnratio;//!<�Բο��ؼ�֡���ٵ�ʱ��ƥ����С���������С����ֱ�ӵı�ֵ��ֵ
				float projection_threshold;//!<ͶӰƥ�������Ĵ�������ֵ����Ŀ��˫Ŀ��һ��

				Options() :projection_threshold(15.f),
					track_motion_model_matcher_nnratio(0.9),
					track_ref_keyframe_matcher_nnratio(0.7)
				{}
			}options_;

			struct CameraTrajectory
			{
				std::list<cv::Mat> list_frame_tcw;
				std::list<cv::Mat> list_relative_frame_poses;
				std::list<KeyFrame *> list_ref_keyframes;
				std::list<double> list_frame_times;
				std::list<bool> list_is_lost;
			}camera_trajectory_;

			TrackingBase(ORBVocabulary *voc);
			virtual ~TrackingBase();
			/**��ͬ�Ĵ�������ʼ����һ��*/
			virtual void initialize() = 0;

			/**����һ��ͼ�񣬸���Ŀ�Ľӿ�*/
			virtual void addImage(const cv::Mat& img, double timestamp) = 0;

			/**�����߳�ִ�е�������*/
			void track();

			/**���ݼ�����˶�ģ�ͽ��и���*/
			bool trackWithMotionModel();

			/**���ݲο��ؼ�֡���и���*/
			bool trackReferenceKeyFrame();

			bool trackLocalMap();

			bool relocalization();
		protected:
			/**������һ֡��λ�ˣ���ֹ��ʧ*/
			void updateLastFrame();

			void updateLocalMap();
			void updateLocalPoints();
			void updateLocalKeyFrames();
		protected:
			TrackingState tracking_state_;//!<��ʾ���õĸ���״̬
			Map  *map_;
			ORBVocabulary *orb_vocabulary_;
			bool is_only_tracking_;//!< �����true,�����ִֻ�ж�λ
			bool is_vo_;//!< �����Ƿ����Ӿ���̼�		
			KeyFrame* init_keyframe_;//!< ��ʼ�ؼ�֡
			FramePtr  cur_frame_;      //!< ��ǰ֡
			bool is_rgb_order_;//!<ͼ��˳��trueͼ����RGB��˳��false��BGR��˳��
			cv::Mat motion_model_;//!< �˶�ģ��

			KeyFrame* ref_keyframe_;//!<�ֲ���ͼ��ʹ�òο�֡
			KeyFrame* last_keyframe_;//!< ��һ���ؼ�֡
			FramePtr last_frame_;//!<��һ֡

			std::vector<MapPoint*> vec_local_map_points_;//!<���ٶ�Ӧ��mappoint����Ҫ���ڿ��ӻ���ʾ
		};
	}
}

#endif // OPENSLAM_SLAM_TRACKING_H_