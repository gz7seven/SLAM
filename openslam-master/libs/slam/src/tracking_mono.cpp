#include <openslam/slam/tracking_mono.h>
#include <openslam/utils/notify.h>
#include <openslam/slam/feature.h>

namespace openslam
{
	namespace slam
	{
		TrackingMono::TrackingMono(const std::string &setting_path_name, ORBVocabulary *voc):
			TrackingBase(voc)
		{
			// �������ļ��е�������ڲ�
			cv::FileStorage file_settings(setting_path_name, cv::FileStorage::READ);
			// ��ʲô��С��ͼƬ�±궨�����Ҫ֪����
			float width = file_settings["Camera.width"];
			float height = file_settings["Camera.height"];
			float fx = file_settings["Camera.fx"];
			float fy = file_settings["Camera.fy"];
			float cx = file_settings["Camera.cx"];
			float cy = file_settings["Camera.cy"];
			
			// �õ�����������
			float k1 = file_settings["Camera.k1"];
			float k2 = file_settings["Camera.k2"];
			float p1 = file_settings["Camera.p1"];
			float p2 = file_settings["Camera.p2"];
			float k3 = file_settings["Camera.k3"];
			cam_ = new MonocularCamera(width, height, fx, fy, cx, cy, k1, k2, p1, p2, k3);

			int is_rgb = file_settings["Camera.RGB"];
			is_rgb_order_ = is_rgb;

			if (is_rgb_order_)
				OPENSLAM_INFO << "- color order: RGB (ignored if grayscale)" << std::endl;
			else
				OPENSLAM_INFO << "- color order: BGR (ignored if grayscale)" << std::endl;

			// ����ORB����ز���
			int feature_num = file_settings["ORBextractor.nFeatures"];
			float scale_factor = file_settings["ORBextractor.scaleFactor"];
			int levels_num = file_settings["ORBextractor.nLevels"];
			int default_fast_threshold = file_settings["ORBextractor.iniThFAST"];
			int min_fast_threshold = file_settings["ORBextractor.minThFAST"];

			extractor_ = new ORBextractor(feature_num, scale_factor, levels_num, default_fast_threshold, min_fast_threshold);

			init_orb_extractor_ = new ORBextractor(2 * feature_num, scale_factor, levels_num, default_fast_threshold, min_fast_threshold);

			OPENSLAM_INFO << std::endl << "ORB Extractor Parameters: " << std::endl;
			OPENSLAM_INFO << "- Number of Features: " << feature_num << std::endl;
			OPENSLAM_INFO << "- Scale Levels: " << levels_num << std::endl;
			OPENSLAM_INFO << "- Scale Factor: " << scale_factor << std::endl;
			OPENSLAM_INFO << "- Initial Fast Threshold: " << default_fast_threshold << std::endl;
			OPENSLAM_INFO << "- Minimum Fast Threshold: " << min_fast_threshold << std::endl;

			stage_ = STAGE_FIRST_FRAME;//��ʼ����ʱ�����ô���֡Ϊ��һ֡
			
		}

		TrackingMono::~TrackingMono()
		{
			delete cam_;
		}

		void TrackingMono::initialize()
		{

		}

		void TrackingMono::addImage(const cv::Mat& img, double timestamp)
		{
			// ��ʼ����֡
			cur_frame_ = std::make_shared<Frame>(cam_, img.clone(), timestamp, extractor_,orb_vocabulary_, is_rgb_order_);
			bool res = false;
			if (stage_ == STAGE_DEFAULT_FRAME)
				res = processFrame();
			else if (stage_ == STAGE_SECOND_FRAME)
				res = processSecondFrame();
			else if (stage_ == STAGE_FIRST_FRAME)
				res = processFirstFrame();

			//�������������ǰ֡������һ֡
			last_frame_ = cur_frame_;

			//��ӹ켣����
			if (!cur_frame_->Tcw_.empty())
			{
				// ��ǰ֡
				cv::Mat Tcr = cur_frame_->Tcw_*cur_frame_->ref_keyframe_->getPose();
				camera_trajectory_.list_frame_tcw.push_back(cur_frame_->Tcw_);
				camera_trajectory_.list_relative_frame_poses.push_back(Tcr);
				camera_trajectory_.list_ref_keyframes.push_back(cur_frame_->ref_keyframe_);
				camera_trajectory_.list_frame_times.push_back(cur_frame_->timestamp_);
				camera_trajectory_.list_is_lost.push_back(tracking_state_ == TRACKING_LOST);
			}
			else
			{
				// ���֡���ݶ�ʧ�������һ֡������
				camera_trajectory_.list_frame_tcw.push_back(camera_trajectory_.list_frame_tcw.back());
				camera_trajectory_.list_relative_frame_poses.push_back(camera_trajectory_.list_relative_frame_poses.back());
				camera_trajectory_.list_ref_keyframes.push_back(camera_trajectory_.list_ref_keyframes.back());
				camera_trajectory_.list_frame_times.push_back(camera_trajectory_.list_frame_times.back());
				camera_trajectory_.list_is_lost.push_back(tracking_state_ == TRACKING_LOST);
			}
			//��������ָ��ʹ����
			cur_frame_.reset();
		}

		bool TrackingMono::processFirstFrame()
		{
			bool is_success = true;
			is_success = initializer_.addFirstFrame(cur_frame_);
			if (is_success)//�����ʼ���ɹ����򽫵�ǰ֡תΪ�ؼ�֡��������BoW
			{
				KeyFrame *initkeyframe = new KeyFrame(*cur_frame_);
				init_keyframe_ = initkeyframe;
				init_keyframe_->computeBoW();
				//����ؼ�֡����ͼ��,�ͷŷŵ���ͼ��
				map_->addKeyframe(init_keyframe_);

				//������ǰ֡�Ĳο��ؼ�֡
				cur_frame_->ref_keyframe_ = initkeyframe;
				// ֡�����꣬������һ֡
				last_keyframe_ = initkeyframe;
				// ����ǰ֡�����ο�֡�����ֲ��Ż�ʹ��
				ref_keyframe_ = initkeyframe;

				stage_ = STAGE_SECOND_FRAME;
			}
			return is_success;
		}

		bool TrackingMono::processSecondFrame()
		{
			bool is_success = true;
			is_success = initializer_.addSecondFrame(cur_frame_);
			if (is_success)//�����ʼ���ɹ����򽫵�ǰ֡תΪ�ؼ�֡��������BoW
			{
				KeyFrame *cur_keyframe = new KeyFrame(*cur_frame_);
				cur_keyframe->computeBoW();
				//����ؼ�֡����ͼ��
				map_->addKeyframe(cur_keyframe);

				//����ά����䵽map point
				//����map point���䵽�ؼ�֡��
				std::vector<int> init_matchex = initializer_.init_matchex_;
				for (size_t i = 0; i < init_matchex.size(); i++)
				{
					if (init_matchex[i] < 0)
						continue;

					cv::Mat world_pos(initializer_.init_3d_points_[i]);//���Ƕ�λ�õ�����������ϵ�еĵ�
					//����ʼ���õ���3d�㴴�� MapPoint.����ڴ��ͷŷŵ�map��,�������������й���
					Feature *ref_feat = initializer_.ref_features_[i];
					Feature *cur_feat = initializer_.cur_features_[init_matchex[i]];
					MapPoint* map_point = new MapPoint(world_pos, cur_feat);
					ref_feat->addMapPointRef(map_point);
					cur_feat->addMapPointRef(map_point);
					map_point->addFeatureRef(ref_feat);
					map_point->addFeatureRef(cur_feat);
					map_point->computeDistinctiveDescriptors();
					map_point->updateNormalAndDepth();

					map_point->is_outlier_ = false;
					map_->addMapPoint(map_point);
					///������������map point�������Ķ�Ӧ
				}

				OPENSLAM_INFO << "New Map created with " << map_->mapPointsInMap() << " points" << std::endl;

				// ������ֵ���,���г߶ȴ���
				float median_depth = init_keyframe_->computeSceneMedianDepth();
				float inv_median_depth = 1.0f / median_depth;

				if (median_depth < 0 || cur_keyframe->trackedMapPoints(1) < 100)
				{
					OPENSLAM_WARN << "Wrong initialization, reseting..." << std::endl;
					return false;
				}

				// ����ε�ƽ�����߶ȴ���
				cv::Mat Tc2w = cur_keyframe->getCameraExternal();
				Tc2w.col(3).rowRange(0, 3) = Tc2w.col(3).rowRange(0, 3)*inv_median_depth;
				cur_keyframe->setCameraExternal(Tc2w);

				// ��map pointҲ���г߶ȴ���
				std::vector<MapPoint*> all_map_points = init_keyframe_->getMapPointMatches();
				for (size_t i = 0; i < all_map_points.size(); i++)
				{
					if (all_map_points[i])
					{
						MapPoint* map_point = all_map_points[i];
						map_point->setWorldPosition(map_point->getWorldPosition()*inv_median_depth);
					}
				}
				//������ǰ֡�Ĳο��ؼ�֡
				cur_frame_->ref_keyframe_ = cur_keyframe;
				// ֡�����꣬������һ֡
				last_keyframe_ = cur_keyframe;
				// ����ǰ֡�����ο�֡�����ֲ��Ż�ʹ��
				ref_keyframe_ = cur_keyframe;
				
				vec_local_map_points_ = map_->getAllMapPoints();
				map_->setReferenceMapPoints(vec_local_map_points_);

				stage_ = STAGE_DEFAULT_FRAME;
				tracking_state_ == TRACKING_OK;
			}

			return is_success;
		}

		bool TrackingMono::processFrame()
		{
			bool is_ok = true;
			
			return is_ok;
		}
	}
}