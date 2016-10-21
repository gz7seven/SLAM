#ifndef OPENSLAM_SLAM_LOOP_CLOSING_H_
#define OPENSLAM_SLAM_LOOP_CLOSING_H_


#include <queue>
#include <thread>
#include <openslam/slam/map.h>
#include <openslam/slam/keyframe.h>

namespace openslam
{
	namespace slam
	{
		class SLAM_IMPEXP LoopClosing
		{
		public:
			/**�����ͼ�����Ż�*/
			LoopClosing(Map* map);

			virtual ~LoopClosing();

			/**�����߳�*/
			void startThread();

			/**�ر��߳�*/
			void stopThread();

			/** ����ؼ�֡
			*/
			void insertKeyFrame(KeyFrame * keyframe);
		protected:
			void run();

		protected:
			bool loop_closing_stop_;//!<�̹߳رս�����ʶ
			std::thread* thread_;//!<�Լ��߳�
			std::mutex mutex_new_keyframes_;//!<��ӹؼ�֡��
			bool is_runing_;//!<�����߳��Ƿ����
			Map* map_;
			std::queue<KeyFrame *> queue_new_keyframes_;//!<�洢�ؼ�֡�б���Ҫ���оֲ���ͼ�Ż�
		};
	}
}



#endif // OPENSLAM_SLAM_LOOP_CLOSING_H_
