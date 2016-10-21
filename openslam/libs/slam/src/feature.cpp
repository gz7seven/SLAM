/*************************************************************************
* �ļ����� frame
*
* ���ߣ� ���
* �ʼ��� fengbing123@gmail.com
* ʱ�䣺 2016/4/9
*
* ˵���� ���������м򵥷�װ
*************************************************************************/
#include <openslam/slam/feature.h>

namespace openslam
{
	namespace slam
	{
		Feature::Feature(Frame* frame, const cv::KeyPoint& keypoint, cv::Mat& descriptor) :
			frame_(frame),
			keypoint_(keypoint),
			descriptor_(descriptor),
			map_point_(nullptr)
		{
			undistored_keypoint_ = keypoint_;
			if (frame_!=nullptr)
			{
				undistortKeyPoint();
			}
		}

		Feature::~Feature()
		{}

		void Feature::undistortKeyPoint()
		{
			cv::Mat cvK = frame_->cam_->cvK();
			cv::Mat dist_coef = frame_->cam_->distCoef();

			if (dist_coef.at<float>(0) == 0.0) return;
			cv::Mat mat = (cv::Mat_<float>(1, 2)<< keypoint_.pt.x, keypoint_.pt.y);
			// ͨ��reshape�������ɵ��
			mat = mat.reshape(2);
			cv::undistortPoints(mat, mat, cvK, dist_coef, cv::Mat(), cvK);
			mat = mat.reshape(1);

			undistored_keypoint_.pt.x = mat.at<float>(0, 0);
			undistored_keypoint_.pt.y = mat.at<float>(0, 1);
		}

		void Feature::addMapPointRef(MapPoint *map_point)
		{
			map_point_ = map_point;
		}

		void Feature::addFrameRef(Frame *frame)
		{
			frame_ = frame;
		}
	}
}