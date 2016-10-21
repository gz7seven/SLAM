#ifndef OPENSLAM_SLAM_CONFIG_H_
#define OPENSLAM_SLAM_CONFIG_H_
#include <openslam/slam/link_pragmas.h>

namespace openslam
{
	namespace slam
	{
		class SLAM_IMPEXP Config
		{
		public:
			/** ���봫����
			*/
			enum Sensor
			{
				SENSOR_MONOCULAR = 0,
				SENSOR_STEREO = 1,
				SENSOR_RGBD = 2
			};

			static Config& getInstance();

			/**�õ�����������*/
			static Sensor& sensorType() { return getInstance().sensor_; }
		private:
			Config();
			Config(Config const&);
			void operator=(Config const&);
			Sensor sensor_;
		};
	}
}

#endif // OPENSLAM_SLAM_CONFIG_H_
