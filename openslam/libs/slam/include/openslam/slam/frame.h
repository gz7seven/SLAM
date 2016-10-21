/*************************************************************************
* �ļ����� frame
*
* ���ߣ� ���
* �ʼ��� fengbing123@gmail.com
* ʱ�䣺 2016/4/9
*
* ˵���� �ο�rpg_svo,orb_slam2
*************************************************************************/
#ifndef OPENSLAM_SLAM_FRAME_H_
#define OPENSLAM_SLAM_FRAME_H_
#include <openslam/slam/link_pragmas.h>
#include <vector>
#include <memory>
#include <opencv2/opencv.hpp>
#include <DBoW2/BowVector.h>
#include <DBoW2/FeatureVector.h>

#include <openslam/slam/abstract_camera.h>
#include <openslam/slam/orb_extractor.h>
#include <openslam/slam/orb_vocabulary.h>

namespace openslam
{
	namespace slam
	{
		//Ŀǰͨ��������Ϊ�̶�ֵ�����ڽ��в�������
#define FRAME_GRID_ROWS 48
#define FRAME_GRID_COLS 64
		class Feature;
		class KeyFrame;
		typedef std::vector<Feature *> Features;

		class SLAM_IMPEXP Frame
		{
		public:
			Frame(){}//����ת�ؼ�֡��ʱ��

			/** ֡�Ĺ��죬������Ӧ�����������Ӧԭʼ֡����Ӧ��ʱ����
			*/
			Frame(AbstractCamera* cam, const cv::Mat& img, double timestamp, ORBextractor* extractor, ORBVocabulary* voc = nullptr, bool is_rgb_order = false);
			virtual ~Frame();

			/** ��ͼ�����������
			*/
			void addFeature(Feature* ftr);

			/** \brief ���������Ƶ�ĳ�������ڣ��������������ƥ��
			*/
			std::vector<size_t> getFeaturesInArea(const float &x, const float  &y, const float  &r, const int min_level = -1, const int max_level = -1) const;

			/** ��������ת��BoW�ĵ��ʱ�ʾ
			*/
			void computeBoW();

			/** ����֡��Ӧ�������λ��
			*/
			cv::Mat getCameraCenter();

			/** ���ڵõ�������ͼ���ÿ��߶�ֵ
			*/
			std::vector<float> getScaleFactors();
			/** �õ��������Ĳ���
			*/
			inline int getLevelsNum(){ return levels_num_; }

			/** �õ��ؼ��������
			*/
			inline int getKeypointsNum(){ return keypoints_num_; }

			/**�õ�������*/
			inline cv::Mat getCameraExternal() { return Tcw_; }

			/**�õ����λ��*/
			inline cv::Mat getPose() { return Tcw_.inv(); }

			/**�����������ξ���*/
			inline void setCameraExternal(cv::Mat Tcw) { Tcw_ = Tcw.clone(); }

			/**�ж���ά���Ƿ���ͼ��֡��,ͨ���������ǻ����ͶӰ���Ƿ��ڻ��䴦��֮���ͼ��Χ��*/
			bool isInFrame(const cv::Mat &point3d, cv::Point2f &point2d);
		protected:
			/**�ṩ֡������ؼ�֡���ݵĿ�����������������*/
			Frame(const Frame &frame);
			/**\briefͼ�����Ԥ��������ɫͼ��ת�Ҷ�
			*/
			void prepareImage(const cv::Mat& input_image, cv::Mat& gray_image);

			/** \brief ��ͼ�����ORB�������
			*/
			void extractORB(const cv::Mat &image);

			/** \brief ����ͼ��ı߽磬����ͼ�����
			*/
			void computeImageBounds(const cv::Mat &image);

			/** \brief ÿ��ͼ��֡�����ʱ����ã����ڽ���������䵽�������Լӿ�����ƥ�䡣
			*/
			void assignFeaturesToGrid();

			/** �������������ڵ�Ԫ�񣬲��ڸ����ڷ���false
			*/
			bool posInGrid(const cv::KeyPoint &kp, int &pos_x, int &pos_y);

			/** \brief ������������vector��ʽ������ת��BoW�ṹ
			*/
			std::vector<cv::Mat> toDescriptorVector(const Features &features);
		public:
			static long unsigned int     frame_counter_;//!< ����֡�ļ���������������֡��Ψһid
			long unsigned int            id_;           //!< ֡��id
			double                       timestamp_;    //!< ֡��ʱ���
			AbstractCamera*              cam_;          //!< ���ģ��
			cv::Mat                      img_;          //!< ֡��Ӧ��ԭʼͼ��
			cv::Mat                      gray_img_;     //!< ֡��Ӧ�ĻҶ�ͼ��
			bool                         is_rgb_order_; //!< ͼ��˳��trueͼ����RGB��˳��false��BGR��˳��
			float                        scale_factor_; //!< ��Ӧ������ͼ��ĳ߶�����		
			Features                     features_;     //!< ֡��Ӧ������
			cv::Mat                      Tcw_;        //!< ����������ϵ(w)orldת�����������ϵ(c)amera�����Ա任Rt
			ORBextractor*                extractor_;    //!< ��������ȡ�ŵ�֡��

			// Bag of Words Vector structures.
			ORBVocabulary* orb_vocabulary_;
			DBoW2::BowVector bow_vec_;
			DBoW2::FeatureVector feat_vector_;

			// �����ͼ��߽���Ҫ���ǻ���ͼ��			
			static float                 min_bound_x_;//!<�����������С�߽�x������
			static float                 max_bound_x_;//!<������������߽�x������
			static float                 min_bound_y_;//!<�����������С�߽�y������
			static float                 max_bound_y_;//!<������������߽�y������
			static float                 grid_element_height_;//!<����֮����ӵ�����
			static float                 grid_element_width_;//!<����֮����ӵĿ��
			static bool                  is_initial_computations_;//!<����һ�μ���ı�ʶ��ture��ʾ�Ѿ��������
			std::vector<std::size_t>     grid_[FRAME_GRID_COLS][FRAME_GRID_ROWS];//!<һ��ͼ�����ĸ���,����洢�����������

			KeyFrame*  ref_keyframe_;//!< ��ǰ֡�ο��Ĺؼ�֡
		protected:
			int                          keypoints_num_;//!< ������ĸ���
			int                          levels_num_;   //!< ��Ӧ�������Ĳ���
		};

		typedef std::shared_ptr<Frame> FramePtr;
	}
}

#endif // OPENSLAM_SLAM_FRAME_H_
