/*************************************************************************
 * �ļ����� pinhole_camera
 *
 * ���ߣ� ���
 * �ʼ��� fengbing123@gmail.com
 * ʱ�䣺 2015/8/1
 *
 * ˵���� �ο�rpg_svo(https://github.com/uzh-rpg/rpg_svo)
 *************************************************************************/
#include <openslam/slam/monocular_camera.h>

namespace openslam
{
	namespace slam
	{
		// ���ǻ������k1,k2,p1,p2,k3
		MonocularCamera::MonocularCamera(double width, double height,
			double fx, double fy,
			double cx, double cy,
			double k1, double k2, double p1, double p2, double k3) :
			AbstractCamera(width, height, fx, fy, cx, cy, k1, k2, p1, p2, k3)
		{}

		MonocularCamera::~MonocularCamera(){}

	}
}
