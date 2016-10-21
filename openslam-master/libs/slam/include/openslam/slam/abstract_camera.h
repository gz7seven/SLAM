#ifndef OPENSLAM_SLAM_ABSTRACT_CAMERA_H_
#define OPENSLAM_SLAM_ABSTRACT_CAMERA_H_
#include <openslam/slam/link_pragmas.h>
#include <opencv2/core/core.hpp>

namespace openslam
{
	namespace slam
	{
		class SLAM_IMPEXP AbstractCamera {

		public:
			// ���ǻ������k1,k2,p1,p2,k3
			AbstractCamera(double width, double height,
				double fx, double fy, double cx, double cy,
				double k1 = 0.0, double k2 = 0.0, double p1 = 0.0, double p2 = 0.0, double k3 = 0.0);

			virtual ~AbstractCamera() {}

			/// ����x����Ľ���ֵ
			virtual double getFocalLength() const
			{
				return fabs(fx_);
			}

			/**���RGB-D���ͼ�����������ʵ�ʾ���ı������ӣ�Ԥ���ӿ�*/
			virtual double getMapFactor() const = 0;

			/**���˫Ŀ���Ԥ���ӿ�*/
			virtual double getBaselineMultFx() const = 0;

			/// ��ý���֮���ͼ����Ҫ������ʾ
			void undistortImage(const cv::Mat& raw, cv::Mat& rectified);

			/// ��������ֱ��ʵĿ��
			inline int width() const { return width_; }
			/// ��������ֱ��ʵĸ߶�
			inline int height() const { return height_; }
			/// �ֱ�õ���������4������
			inline double fx() const { return fx_; }
			inline double fy() const { return fy_; }
			inline double cx() const { return cx_; }
			inline double cy() const { return cy_; }

			inline double invfx() const
			{
				if (fx_ == 0) return 0;
				return 1.0 / fx_;
			}

			inline double invfy() const
			{
				if (fy_ == 0) return 0;
				return 1.0 / fy_;
			}

			inline cv::Mat cvK() const { return cvK_; }

			inline cv::Mat distCoef() const{ return dist_coef_; }

		private:
			int width_; //!< ����ֱ��ʵĿ��
			int height_; //!< ����ֱ��ʵĸ߶�
			double fx_, fy_;  //!< �����������Ľ���ֵ
			double cx_, cy_;  //!< ��������ĵ�
			bool distortion_; //!< �ǵ�����С�����ģ�ͣ����Ǵ��л��䣿
			double d_[5];     //!< ����������ο� http://docs.opencv.org/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html
			cv::Mat cvK_, dist_coef_;//!< ͨ��OpenCV��ʾ�������������������������
			cv::Mat undist_map1_, undist_map2_;//!<������������������map���ṩ��remap����ʹ��
		};
	}
}

#endif // OPENSLAM_SLAM_ABSTRACT_CAMERA_H_
