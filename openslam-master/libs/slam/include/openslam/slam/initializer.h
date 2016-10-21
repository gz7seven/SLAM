/*************************************************************************
* �ļ����� initializer.h
*
* ���ߣ� ���
* �ʼ��� fengbing123@gmail.com
* ʱ�䣺 2016/4/16
*
* ˵���� ��Ҫ�������е�Ŀ��ʼ��
*************************************************************************/
#ifndef OPENSLAM_SLAM_INITIALIZER_H_
#define OPENSLAM_SLAM_INITIALIZER_H_

#include <openslam/slam/frame.h>

namespace openslam
{
	namespace slam
	{
		typedef std::pair<int, int> Match;
		/** \brief ��Ҫ���ڵ�Ŀslam�еĳ�ʼ��
		*/
		class SLAM_IMPEXP Initializer
		{
		public:

			/** \brief ����sigma��iterations�������жϼ����ʼλ���ǲ���
			*  ��Ӧ�����ǻ��������Լ�����RANSAC��ʱ�������������
			*/
			Initializer(float sigma = 1.0f, int iterations = 200);
			~Initializer();

			bool addFirstFrame(FramePtr ref_frame);
			bool addSecondFrame(FramePtr cur_frame);

			void reset();
		private:

			void findHomography(std::vector<bool> &matches_is_inliers, float &score, cv::Mat &H_cur_from_ref);
			void findFundamental(std::vector<bool> &is_inliers, float &score, cv::Mat &F21);

			/**
			* \brief 2D��Ӧ������ƣ���Ϊ��һ��֮��ĵ�
			*/
			cv::Mat calcHFromMatches(const std::vector<cv::Point2f> &points_ref, const std::vector<cv::Point2f> &points_cur);
			/** \brief ��������Ĺ���
			*/
			cv::Mat calcFFromMatches(const std::vector<cv::Point2f> &points_ref, const std::vector<cv::Point2f> &points_cur);

			float checkHomography(const cv::Mat &H_cur_from_ref, const cv::Mat &H_ref_from_cur, std::vector<bool> &matches_is_inliers, float sigma);
			float checkFundamental(const cv::Mat &F21, std::vector<bool> &matches_is_inliers, float sigma);

			bool reconstructF(std::vector<bool> &matches_is_inliers, cv::Mat &F21, cv::Mat &K,
				cv::Mat &R_cur_ref, cv::Mat &t_cur_ref, std::vector<cv::Point3f> &vec_point3d, std::vector<bool> &is_triangulated, float min_parallax, int min_triangulated);

			bool reconstructH(std::vector<bool> &matches_is_inliers, cv::Mat &H_cur_from_ref, cv::Mat &K,
				cv::Mat &R_cur_ref, cv::Mat &t_cur_ref, std::vector<cv::Point3f> &vec_point3d, std::vector<bool> &is_triangulated, float min_parallax, int min_triangulated);
			/** ���Ƕ�λ�������Ӧ�����㣬����ӦͶӰ��������������Ӧ����ά��
			*/
			void triangulate(const cv::KeyPoint &kp1, const cv::KeyPoint &kp2, const cv::Mat &P1, const cv::Mat &P2, cv::Mat &x3D);

			void normalize(const std::vector<Feature *> &features, std::vector<cv::Point2f> &normalized_points, cv::Mat &T);

			int checkRt(const cv::Mat &R, const cv::Mat &t, const std::vector<Feature *> &vec_features1, const std::vector<Feature *> &vec_features2,
				const std::vector<Match> &matches_ref_cur, std::vector<bool> &is_inliers,
				const cv::Mat &K, std::vector<cv::Point3f> &vec_point3d, float th2, std::vector<bool> &vec_is_good, float &parallax);

			void decomposeE(const cv::Mat &E, cv::Mat &R1, cv::Mat &R2, cv::Mat &t);

		public:
			FramePtr ref_frame_;//!<��ʼ�ο�֡
			std::vector<int> init_matchex_;//!<��ʼ��ƥ��
			std::vector<cv::Point3f> init_3d_points_;//!<��ʼ���õ���3d��
			std::vector<Feature *> ref_features_;//!<�ο�֡�е�����
			std::vector<Feature *> cur_features_;//!<��ǰ֡�е�����

		protected:
			float sigma_;//!< ��׼��
			int max_iter_num_;//!<RANSAC ����������
			
			std::vector<Match> matches_ref_cur_;//!<��ǰƥ�䣬��ʾ�ο�֡�Ե�ǰ֡
			std::vector<bool> is_matched_ref_;//!<���ڱ�ʾ�ο�֡�Ƿ���ƥ��
		
			std::vector<cv::Point2f> prev_matched_;//!<Ԥ��ƥ��㣬Ҳ���ǲο�֡�������λ��

			std::vector<std::vector<size_t> > ransac_sets_;//!<����8���������ݼ�
		};
	}
}

#endif // OPENSLAM_SLAM_INITIALIZER_H_