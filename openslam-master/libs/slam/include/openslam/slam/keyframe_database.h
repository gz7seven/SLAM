#ifndef OPENSLAM_SLAM_KEYFRAME_DATABASE_H_
#define OPENSLAM_SLAM_KEYFRAME_DATABASE_H_
#include <openslam/slam/link_pragmas.h>
#include <mutex>
#include <openslam/slam/keyframe.h>
#include <openslam/slam/orb_vocabulary.h>

namespace openslam
{
	namespace slam
	{
		/** \brief �ؼ�֡�����ݿ�
		*/
		class SLAM_IMPEXP KeyFrameDatabase
		{
		public:

			KeyFrameDatabase(const ORBVocabulary &voc);

			void add(KeyFrame * keyframe);

			void erase(KeyFrame * keyframe);

			void clear();

		protected:

			const ORBVocabulary* orb_voc_;//!< �����Ĵʻ��

			std::vector<std::list<KeyFrame *> > inverted_file_;//!<�ʴ��������

			std::mutex mutex_keyframe_;//!< �Թؼ�֡���µ�ʱ��Ļ�����
		};
	}
}

#endif // OPENSLAM_SLAM_KEYFRAME_DATABASE_H_
