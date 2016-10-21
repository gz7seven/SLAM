/*******************************************************************************
* �ļ��� notify.h
* ʱ�䣺 2016/4/10 15:55
* ���ߣ� ���
* �ʼ��� fengbing123@gmail.com
*
* ˵���� ��Ҫ��������־����
*
********************************************************************************/
#ifndef OPENSLAM_UTILS_NOTIFY_H_
#define OPENSLAM_UTILS_NOTIFY_H_

#include <openslam/base/link_pragmas.h>
#include <ostream>

namespace openslam
{
	namespace utils
	{
		/** ��ʾ֪ͨ�����س̶�
		*/
		enum NotifySeverity {
			ALWAYS = 0,
			FATAL = 1,
			WARN = 2,
			NOTICE = 3,
			INFO = 4,
			DEBUG_INFO = 5,
			DEBUG_FP = 6
		};

		/// �����ж��Ƿ���֪ͨ
#ifdef OPENSLAM_NOTIFY_DISABLED
		inline bool isNotifyEnabled(NotifySeverity) { return false; }
#else
		/** �������ô��ڵ�ǰ֪ͨ����ĲŽ���֪ͨ */
		extern BASE_IMPEXP bool isNotifyEnabled(NotifySeverity severity);
#endif

		/** ����֪ͨ������ȼ���ͨ�����û������������� OPENSLAMNOTIFYLEVEL ���� OPENSLAM_NOTIFY_LEVEL.
		*/
		extern BASE_IMPEXP void setNotifyLevel(NotifySeverity severity);

		/** �õ���ǰ֪ͨ������ȼ� */
		extern BASE_IMPEXP NotifySeverity getNotifyLevel();

		/** ��ʼ��֪ͨ�ȼ� */
		extern BASE_IMPEXP bool initNotifyLevel();

		/** ͨ������NotifyLevel�����ж������Ϣ���п��ƣ��������û����������磺
		* - ���� OPENSLAMNOTIFYLEVEL=DEBUG
		* Ĭ�ϵȼ�ΪNOTICE
		*
		* \code
		* openslam::utils::notify(openslam::utils::DEBUG) << "Hello Bugs!" << std::endl;
		* \endcode
		*/
		extern BASE_IMPEXP std::ostream& notify(const NotifySeverity severity);

		/**Ĭ��֪ͨ���*/
		inline std::ostream& notify(void) { return notify(openslam::utils::INFO); }

#define OPENSLAM_NOTIFY(level) if (openslam::utils::isNotifyEnabled(level)) openslam::utils::notify(level)
#define OPENSLAM_ALWAYS OPENSLAM_NOTIFY(openslam::utils::ALWAYS)
#define OPENSLAM_FATAL OPENSLAM_NOTIFY(openslam::utils::FATAL)
#define OPENSLAM_WARN OPENSLAM_NOTIFY(openslam::utils::WARN)
#define OPENSLAM_NOTICE OPENSLAM_NOTIFY(openslam::utils::NOTICE)
#define OPENSLAM_INFO OPENSLAM_NOTIFY(openslam::utils::INFO)
#define OPENSLAM_DEBUG OPENSLAM_NOTIFY(openslam::utils::DEBUG_INFO)
#define OPENSLAM_DEBUG_FP OPENSLAM_NOTIFY(openslam::utils::DEBUG_FP)

		/** Handler ������Ϣ�������. ����Ϊһ������۵Ĺ��ܣ�����Ϣ��Ҫͬ��ʱ
		* (����� openslam::utils::notify() << std::endl).
		* StandardNotifyHandler��Ĭ��ʹ�ã��佫��Ϣ�����stderr
		* (severity <= WARN) ���� stdout (severity > WARN).
		* ��Ϣ���Ա��ض��򵽣�����GUI�Ĵ����ϻ���windows debugger (WinDebugNotifyHandler).
		* ʹ��setNotifyHandle���Զ���handler.
		* ע�⣺��Ϣ��� API�����̰߳�ȫ�ģ������GUI��ʱ��Ҫ�����߳̿���
		* \see setNotifyHandler
		*/
		class BASE_IMPEXP NotifyHandler
		{
		public:
			virtual ~NotifyHandler(){}
			virtual void notify(openslam::utils::NotifySeverity severity, const char *message) = 0;
		};

		/** ������Ϣhandler, Ĭ����StandardNotifyHandler
		* \see NotifyHandler
		*/
		extern BASE_IMPEXP void setNotifyHandler(NotifyHandler *handler);

		/** �õ���ǰ����Ϣ֪ͨhandler. */
		extern BASE_IMPEXP NotifyHandler *getNotifyHandler();

		/** ����Ϣ֪ͨ���ض���stderr (severity <= WARN) ���� stdout (severity > WARN).
		* fputs()�������ڽ���Ϣд�뵽��׼�ļ��У����׼��
		* std::out and std::cerr streams�����ᱻʹ��.
		* \see setNotifyHandler
		*/
		class BASE_IMPEXP StandardNotifyHandler : public NotifyHandler
		{
		public:
			void notify(openslam::utils::NotifySeverity severity, const char *message);
		};

		/** ����Ϣ֪ͨ���ض��� windows debugger ͨ��ʹ�� OuputDebugString ����.
		* \see setNotifyHandler
		*/
		class BASE_IMPEXP WinDebugNotifyHandler : public NotifyHandler
		{
		public:
			void notify(openslam::utils::NotifySeverity severity, const char *message);
		};
	}

}



#endif // OPENSLAM_UTILS_NOTIFY_H_
