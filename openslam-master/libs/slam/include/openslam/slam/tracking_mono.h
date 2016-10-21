#ifndef OPENSLAM_SLAM_TRACKING_MONO_H_
#define OPENSLAM_SLAM_TRACKING_MONO_H_
#include <openslam/slam/tracking_base.h>
#include <openslam/slam/initializer.h>
#include <openslam/slam/monocular_camera.h>
#include <openslam/slam/map.h>

namespace openslam
{
	namespace slam
	{
		class SLAM_IMPEXP TrackingMono : public TrackingBase
		{
		public:
			enum Stage {
				STAGE_PAUSED,//��ͣ
				STAGE_FIRST_FRAME,//��һ֡
				STAGE_SECOND_FRAME,//�ڶ�֡
				STAGE_DEFAULT_FRAME,//Ĭ��֡
				STAGE_RELOCALIZING//���¶�λ
			};

			/**���������ļ����г�ʼ��*/
			TrackingMono(const std::string &setting_path_name, ORBVocabulary * voc);
			/**����Ϊ�麯�������Խ�һ����չ*/
			virtual ~TrackingMono();

			virtual void initialize();
			/**����һ��ͼ��*/
			void addImage(const cv::Mat& img, double timestamp);

		protected:
			/**�����һ֡*/
			virtual bool processFirstFrame();

			/**���������������֡��ֱ����һ���ؼ�֡��ѡ��*/
			virtual bool processSecondFrame();

			/**�����꿪ʼ�������ؼ�֡��������֡*/
			virtual bool processFrame();

		protected:
			Stage stage_;                 //!< �㷨��ǰ�����Ľ׶�,��Ŀ�ڲ�״̬
			MonocularCamera *cam_;//!<��Ŀ���
			ORBextractor* init_orb_extractor_;//!<��ʼ����ʱ��������ȡ
			ORBextractor* extractor_;//!< orb������ȡ
			Initializer initializer_;//!<��Ҫ���ڵ�Ŀslam �ĳ�ʼ��
			
		};
	}
}

#endif // OPENSLAM_SLAM_TRACKING_MONO_H_
