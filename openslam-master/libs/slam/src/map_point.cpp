#include <openslam/slam/map_point.h>
#include <openslam/slam/feature.h>
#include <openslam/slam/orb_matcher.h>
#include <openslam/utils/notify.h>

namespace openslam
{
	namespace slam
	{
		long unsigned int MapPoint::map_point_counter_ = 0;
		std::mutex MapPoint::global_mutex_;

		MapPoint::MapPoint(const cv::Mat &pos,Feature * cur_obs) :
			id_(map_point_counter_++),
			world_position_(pos), 
			cur_obs_(cur_obs),
			obs_num_(0),
			is_bad_(false),
			is_outlier_(false),
			track_in_view_(true),
			last_frame_seen_id_(0),
			gba_for_keyframe_num_(0),
			local_ba_for_keyframe_id_(0)
		{
			normal_vector_ = cv::Mat::zeros(3, 1, CV_32F);
		}

		cv::Mat MapPoint::getWorldPosition()
		{
			std::unique_lock<mutex> lock(mutex_position_);
			return world_position_.clone();
		}

		void MapPoint::setWorldPosition(const cv::Mat &pos)
		{
			std::unique_lock<mutex> lock2(global_mutex_);
			std::unique_lock<mutex> lock(mutex_position_);
			pos.copyTo(world_position_);
		}

		void MapPoint::addFeatureRef(Feature* ftr)
		{
			obs_.push_front(ftr);
			++obs_num_;
		}

		void MapPoint::computeDistinctiveDescriptors()
		{
			// ��ͬһ��mappoint���ж�Ӧ�����������������ӽ��д���Ѱ�����������
			std::vector<cv::Mat> vec_descriptors;
			std::list<Feature*>   obs;
			{
				std::unique_lock<std::mutex> lock1(mutex_features_);
				obs = obs_;
			}
			if (obs.empty()) return;
			vec_descriptors.reserve(obs.size());

			for (auto it = obs_.begin(), ite = obs_.end(); it != ite; ++it)
			{
				if (*it)
					vec_descriptors.push_back((*it)->descriptor_);
			}
			if (vec_descriptors.empty()) return;
			// ����������������֮��ľ���
			const size_t N = vec_descriptors.size();
#ifdef _WIN32
			float **distances;
			distances = new float*[N];
			for (int i = 0; i < N; i++)
			{
				distances[i] = new float[N];
			}
#else
			float distances[N][N];
#endif

			for (size_t i = 0; i < N; i++)
			{
				distances[i][i] = 0;
				for (size_t j = i + 1; j < N; j++)
				{
					int distij = ORBmatcher::descriptorDistance(vec_descriptors[i], vec_descriptors[j]);
					distances[i][j] = distij;
					distances[j][i] = distij;
				}
			}

			// ����Щ�����������������Ѱ����С����ֵ
			int best_median = INT_MAX;
			int best_index = 0;
			for (size_t i = 0; i < N; i++)
			{
				// �Ծ��������ҵ���ֵ
				std::vector<int> vec_dists(distances[i], distances[i] + N);
				sort(vec_dists.begin(), vec_dists.end());
				int median = vec_dists[0.5*(N - 1)];

				if (median < best_median)
				{
					best_median = median;
					best_index = i;
				}
			}
#ifdef _WIN32
			for (int i = 0; i < N; i++)
				delete[]distances[i];
			delete[]distances;
#endif 

			{
				std::unique_lock<mutex> lock(mutex_features_);
				descriptor_ = vec_descriptors[best_index].clone();
			}
		}

		cv::Mat MapPoint::getDescriptor()
		{
			std::unique_lock<mutex> lock(mutex_features_);
			return descriptor_.clone();
		}

		void MapPoint::updateNormalAndDepth()
		{
			std::list<Feature*>   obs;
			cv::Mat pos;// map point ����������ϵ�е�λ��
			Feature * cur_obs;
			{
				std::unique_lock<mutex> lock1(mutex_features_);
				std::unique_lock<mutex> lock2(mutex_position_);
				obs = obs_;
				cur_obs = cur_obs_;
				pos = world_position_.clone();
			}

			if (obs.empty())
			{
				OPENSLAM_INFO << "this map point has no feature! " << std::endl;
				return;
			}

			cv::Mat normal = cv::Mat::zeros(3, 1, CV_32F);
			int n = 0;
			for (auto it = obs_.begin(), ite = obs_.end(); it != ite; ++it)
			{
				cv::Mat Owi = (*it)->frame_->getCameraCenter();
				cv::Mat normali = world_position_ - Owi;
				normal = normal + normali / cv::norm(normali);//������map pointָ��ؼ�֡��Ӧ��������ĵ��������й�һ����������ֵ��������ʾ��ǰmap point�ķ�����
				n++;
			}
			Frame * cur_frame = cur_obs->frame_;
			cv::Mat PC = pos - cur_frame->getCameraCenter();
			const float dist = cv::norm(PC);// �õ�ǰ֡������Ŀǰmap point�����
			// ���map point ��Ӧ��ǰ֡�������������ڽ���������-> 
			const int level = cur_obs->undistored_keypoint_.octave;
			// ������ڲ����ĳ߶�����
			std::vector<float> scale_factors = cur_frame->getScaleFactors();
			const float level_scale_factor = scale_factors[level];
			// ��ý������Ĳ���
			const int levels_num = cur_frame->getLevelsNum();

			{
				std::unique_lock<mutex> lock3(mutex_position_);
				// ���������С����ļ���
				max_distance_ = dist*level_scale_factor;
				min_distance_ = max_distance_ / scale_factors[levels_num - 1];
				normal_vector_ = normal / n;
			}
		}

		void MapPoint::setBadFlag()
		{
			{
				std::unique_lock<mutex> lock1(mutex_features_);
				std::unique_lock<mutex> lock2(mutex_position_);
				is_bad_ = true;
			}
		}

		bool MapPoint::isBad()
		{
			std::unique_lock<mutex> lock(mutex_features_);
			std::unique_lock<mutex> lock2(mutex_position_);
			return is_bad_;
		}

		int MapPoint::observationsNum()
		{
			std::unique_lock<mutex> lock(mutex_features_);
			return obs_num_;
		}

	}
}