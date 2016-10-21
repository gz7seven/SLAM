#include <openslam/slam/tracking_base.h>
#include <openslam/slam/orb_matcher.h>
#include <openslam/slam/feature.h>
#include <openslam/utils/notify.h>

namespace openslam
{
	namespace slam
	{
		TrackingBase::TrackingBase(ORBVocabulary *voc) :
			orb_vocabulary_(voc),
			tracking_state_(TRACKING_NO_IMAGES_YET),
			is_only_tracking_(false),
			is_vo_(false),
			is_rgb_order_(false),
			map_(nullptr)
		{
			//������ͼ
			map_ = new Map();
		}
		TrackingBase::~TrackingBase()
		{
			delete map_;
		}

		// ���и��ٴ���ȷ���Ƿ���ٳɹ������߶�ʧ
		void TrackingBase::track()
		{
			bool is_ok = true;
			//�����жϸ����Ƿ�ʧ
			if (tracking_state_ == TRACKING_LOST)
			{
				is_ok = relocalization();
			}

			if (!is_only_tracking_)//��Ӿֲ��Ż�
			{

				if (is_ok)
					is_ok = trackLocalMap();
			}
			else// ֻ�Ǹ���
			{

				if (!is_vo_)
				{
					if (!motion_model_.empty())
					{
						is_ok = trackWithMotionModel();
					}
					else
					{
						is_ok = trackReferenceKeyFrame();
					}

					if (is_ok)
						is_ok = trackLocalMap();
				}
				else
				{

				}
			}
			cur_frame_->ref_keyframe_ = ref_keyframe_;

			//���ٽ���֮��ȷ������״̬
			if (is_ok)
				tracking_state_ = TRACKING_OK;
			else
				tracking_state_ = TRACKING_LOST;

			// ���������ȷ������һ������Ҫȷ����ǰ֡�Ƿ�Ϊ�ؼ�֡
			if (is_ok)
			{
				if (!last_frame_->Tcw_.empty())
				{
					cv::Mat last_Twc = cv::Mat::eye(4, 4, CV_32F);
					last_frame_->getPose().copyTo(last_Twc);
					motion_model_ = cur_frame_->Tcw_*last_Twc;
				}
				else
				{
					motion_model_ = cv::Mat();
				}

				// ɾ��û�ж�Ӧ���mappoint
				for (int i = 0; i < cur_frame_->getKeypointsNum(); i++)
				{
					MapPoint* map_point = cur_frame_->features_[i]->map_point_;
					if (map_point)
					{
						if (map_point->observationsNum() < 1)
						{
							cur_frame_->features_[i]->map_point_->is_outlier_ = true;
							cur_frame_->features_[i]->map_point_ = static_cast<MapPoint*>(NULL);
						}
					}
				}
			}

		}

		void TrackingBase::updateLastFrame()
		{
			cv::Mat Tcw = camera_trajectory_.list_frame_tcw.back();
			last_frame_->setCameraExternal(Tcw);
		}

		bool TrackingBase::trackWithMotionModel()
		{
			OPENSLAM_INFO << "trackWithMotionModel begins" << std::endl;
			updateLastFrame();//�õ���һ֡�����ֵ����ֹ��ʧ
			cur_frame_->setCameraExternal(motion_model_ * last_frame_->Tcw_);

			ORBmatcher matcher(options_.track_motion_model_matcher_nnratio, true);
			int nmatches = matcher.searchByProjection(cur_frame_, last_frame_, options_.projection_threshold);

			//���ƥ�����������������������
			if (nmatches < 20)
			{
				nmatches = matcher.searchByProjection(cur_frame_, last_frame_, 2 * options_.projection_threshold);
			}
			// �������û���㹻��ƥ�䣬�򷵻�
			if (nmatches < 20)
				return false;
			return true;
		}

		bool TrackingBase::trackReferenceKeyFrame()
		{
			OPENSLAM_INFO << "trackReferenceKeyFrame begin " << std::endl;
			// ��������ת��BoW�ĵ��ʱ�ʾ
			cur_frame_->computeBoW();
			ORBmatcher matcher(options_.track_ref_keyframe_matcher_nnratio, true);

			int nmatches = matcher.searchByBoW(ref_keyframe_, cur_frame_);

			if (nmatches < 15)
				return false;
			//������ʼֵ�������Ż�
			cur_frame_->setCameraExternal(last_frame_->Tcw_);

			//Optimizer::poseOptimization(cur_frame_);

			// �Ż����֮��ɾ�����
			int matches_in_map_num = 0;
			for (int i = 0; i < cur_frame_->getKeypointsNum(); i++)
			{
				MapPoint *map_point = cur_frame_->features_[i]->map_point_;
				if (map_point)
				{
					if (map_point->is_outlier_)
					{
						map_point = static_cast<MapPoint*>(nullptr);
						map_point->is_outlier_ = false;
						map_point->track_in_view_ = false;
						map_point->last_frame_seen_id_ = cur_frame_->id_;
						nmatches--;
					}
					else if (map_point->observationsNum()>0)
						matches_in_map_num++;
				}
			}

			return matches_in_map_num >= 10;
		}

		bool TrackingBase::trackLocalMap()
		{
			OPENSLAM_INFO << "trackLocalMap begin " << std::endl;
			map_->setReferenceMapPoints(vec_local_map_points_);

			updateLocalKeyFrames();
			updateLocalPoints();
			return true;
		}

		void TrackingBase::updateLocalPoints()
		{

		}

		void TrackingBase::updateLocalKeyFrames()
		{

		}

		bool TrackingBase::relocalization()
		{
			OPENSLAM_INFO << "relocalization begin" << std::endl;
			return true;
		}
	}
}