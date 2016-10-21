/*************************************************************************
 * �ļ����� pinhole_camera
 *
 * ���ߣ� ���
 * �ʼ��� fengbing123@gmail.com
 * ʱ�䣺 2015/8/1
 *
 * ˵���� �ο�rpg_svo(https://github.com/uzh-rpg/rpg_svo)
 *************************************************************************/
#ifndef OPENSLAM_SLAM_PINHOLE_CAMERA_H_
#define OPENSLAM_SLAM_PINHOLE_CAMERA_H_

#include <openslam/slam/abstract_camera.h>

namespace openslam
{
	namespace slam
	{
		class SLAM_IMPEXP MonocularCamera:public AbstractCamera {

		public:
			// ���ǻ������k1,k2,p1,p2,k3
			MonocularCamera(double width, double height,
				double fx, double fy, double cx, double cy,
				double k1 = 0.0, double k2 = 0.0, double p1 = 0.0, double p2 = 0.0, double k3 = 0.0);

			~MonocularCamera();

			double getMapFactor() const
			{
				return -1.;
			}

			double getBaselineMultFx() const
			{
				return -1.;
			}
		};
	}
}

#endif // OPENSLAM_SLAM_PINHOLE_CAMERA_H_
