#ifndef OPENSLAM_SLAM_ORB_MATCHER_H_
#define OPENSLAM_SLAM_ORB_MATCHER_H_
#include <openslam/slam/frame.h>
#include <opencv2/opencv.hpp>

namespace openslam
{
	namespace slam
	{
		class MapPoint;
		class SLAM_IMPEXP ORBmatcher
		{
		public:

			ORBmatcher(float nnratio = 0.6, bool check_orientation = true);

			~ORBmatcher();
			/** ������������֮��ĺ�������
			* �������ο� http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
			*/
			static int descriptorDistance(const cv::Mat &a, const cv::Mat &b);

			/** ��֮֡��Ѱ��ƥ�䣬���ڵ�Ŀ�ĳ�ʼ��
			*/
			int searchForInitialization(FramePtr ref_frame, FramePtr cur_frame,
				std::vector<cv::Point2f> &prev_matched, std::vector<int> &matches_ref_cur, int window_size = 10);

			/** ����һ֡�л�õ�mappoint��ͶӰ����ǰ֡�У�����ƥ��
			*/
			int searchByProjection(FramePtr cur_frame, const FramePtr last_frame, const float th);

			/**
			*/
			int searchByBoW(KeyFrame *keyframe, FramePtr frame);
		protected:
			/** \brief ����ֱ��ͼ�и߶�ǰ3�����������
			*/
			void computeThreeMaxima(std::vector<int>* histo, const int L, int &ind1, int &ind2, int &ind3);

		public:
			static const int TH_LOW;
			static const int TH_HIGH;
			static const int HISTO_LENGTH;//!<������������Ƿ�һ�£�ֱ��ͼ���������Ĭ��30

		protected:
			float nn_ratio_;
			bool is_check_orientation_;

		};
	}
}

#endif // OPENSLAM_SLAM_ORB_MATCHER_H_
