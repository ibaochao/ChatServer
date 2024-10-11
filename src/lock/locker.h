#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>


// 信号量
class sem
{
	public:
		sem()  // 无参构造函数
		{
			if (sem_init(&m_sem, 0, 0) != 0)
			{
				throw std::exception();
			}
		}
		
		sem(int num)  // 含参构造函数
		{
			if (sem_init(&m_sem, 0, num) != 0)
			{
				throw std::exception();
			}
		}
		
		~sem()  // 析构函数
		{
			sem_destroy(&m_sem);
		}
		
		bool wait()  // P操作（减一）
		{
			return sem_wait(&m_sem) == 0;
		}
		
		bool post()  // V操作（加一）
		{
			return sem_post(&m_sem) == 0;
		}
	private:
		sem_t m_sem;  // 成员变量
};


// 锁
class locker
{
	public:
		locker()  // 构造函数
		{
			if (pthread_mutex_init(&m_mutex, NULL) != 0)
			{
				throw std::exception();
			}
		}
		
		~locker()  // 析构函数
		{
			pthread_mutex_destroy(&m_mutex);
		}
		
		bool lock()  // 加锁
		{
			return pthread_mutex_lock(&m_mutex) == 0;
		}
		
		bool unlock()  // 释放锁
		{
			return pthread_mutex_unlock(&m_mutex) == 0;
		}
		
		pthread_mutex_t *get()  // 获取成员变量
		{
			return &m_mutex;
		}

	private:
		pthread_mutex_t m_mutex;  // 成员变量
};


// 条件变量
class cond
{
	public:
		cond()  // 构造函数
		{
			if (pthread_cond_init(&m_cond, NULL) != 0)
			{
				//pthread_mutex_destroy(&m_mutex);
				throw std::exception();
			}
		}
		
		~cond()  // 析构函数
		{
			pthread_cond_destroy(&m_cond);
		}
		
		bool wait(pthread_mutex_t *m_mutex)  // 等待
		{
			int ret = 0;
			//pthread_mutex_lock(&m_mutex);
			ret = pthread_cond_wait(&m_cond, m_mutex);
			//pthread_mutex_unlock(&m_mutex);
			return ret == 0;
		}
		
		bool timewait(pthread_mutex_t *m_mutex, struct timespec t)  // 按时等待
		{
			int ret = 0;
			//pthread_mutex_lock(&m_mutex);
			ret = pthread_cond_timedwait(&m_cond, m_mutex, &t);
			//pthread_mutex_unlock(&m_mutex);
			return ret == 0;
		}
		
		bool signal()  // 唤醒一个
		{
			return pthread_cond_signal(&m_cond) == 0;
		}
		
		bool broadcast()  // 唤醒全部
		{
			return pthread_cond_broadcast(&m_cond) == 0;
		}

	private:
		//static pthread_mutex_t m_mutex;
		pthread_cond_t m_cond;  // 成员变量
};


#endif