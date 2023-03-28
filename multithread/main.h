#pragma once
#include<iostream>
#include<vector>
#include<functional>
#include<future>
#include<Windows.h>

//std::promise�Afuture
//Set�����Ƃ��܂ł̏�����Ԃ��̂Ń��C���X���b�h�Ŏ󂯎���āiGet���āj���ꂽ�甽�f���܂���B
//�������A�Ppromis�ɕt���PSetGet�����ł��Ȃ����ߕ��������Ԃ��|�C���g���~�����ꍇ�͕���Promise���K�v�B
//�󂯎��Future�͈�ł���
//
//std::thread
//�Ƃɂ����񓯊��œ����������B�A�v���P�[�V�����I�����܂łɂ����Join�Ŋ��������Ƃ��Ȃ���΂����Ȃ�

template<class T>
using shared_promise = std::shared_ptr<std::promise<T>>;

class C_Task;
/*
class C_Thread
{
public:
	C_Thread() {};
	~C_Thread() {};

	// ���̃X���b�h�ŏ������������̂�o�^����
	void RegistFunction(std::function<void()> f, int promiseRes = -1) {
		std::function<void()> func = [&f]() {f(); };
		if (promiseRes != -1) {
			shared_promise<int> promise = std::make_shared<std::promise<int>>();
			m_promises.emplace_back(promise);
			func = [=]() {
				f();
				shared_promise<int> prs = m_promises.front();
				m_promises.pop_front();
				prs->set_value(promiseRes);
			};
		}
		m_funcs.emplace_back(func);

	}
	// ���̃X���b�h���J�n����
	void ExecuteThread() {
		m_thread = new std::thread(
			[this]() {
				for (auto f : m_funcs) {
					f();
				}
			}
		);
	};

	void GetFutureFront() {
		// �o�^���ꂽPromise��������Future�����L��Ԃ������Ȃ珈��������̂������̂ŏI��
		if ((m_promises.size() == 0) && (!m_future.valid()))return;

		// ���L��Ԃ������Ă��Ȃ���ΐ擪��promise����擾
		if (!m_future.valid()) {
			std::promise<int>* promise = m_promises.front().get();
			m_future = promise->get_future();
		}

		// 1�b�ҋ@���A�������������Ă��Ȃ���΃X�L�b�v�E���Ă���Γ��������
		std::chrono::steady_clock::time_point timePoint = std::chrono::steady_clock::now();
		std::future_status res = m_future.wait_until(timePoint + std::chrono::seconds(1));
		if (res != std::future_status::timeout)
			m_future.get();
		else
			std::cout << "timeout" << std::endl;
	};

	void GetFutureAll() {
		// �z��T�C�Y���ς��̂ł�����Max�l������Ă��ꂾ����
		int max = m_promises.size();
		for (int num = 0; num < max; num++) {
			GetFutureFront();
			// �^�C���A�E�g���Ă���ΏI��
			if (m_future.valid())
				return;
		}
		m_promises.clear();
	}

	// ���̃X���b�h�̏I����҂�
	void Join() {
		m_thread->join();

	}
private:
	std::vector<std::function<void()>> m_funcs;	// ���̔񓯊��ɂ�����������
	std::list<shared_promise<int>> m_promises;	// ������ɕԂ��l
	std::future<int> m_future;					// �񓯊��������ʂ��󂯎�邽�߂̕ϐ�
	std::thread* m_thread = nullptr;
};
*/

class C_Thread
{
public:
	C_Thread() {};
	~C_Thread() {};

	void AddTask(const C_Task& task) {
		if (!m_thread)
			m_tasks.emplace_back(task);
	}
	void ExecuteTasks() {
		m_thread = new std::thread(
			[this]() {
				for (auto task : m_tasks)
					task->ExecuteTask();
			}
		);
	}
	void TryGetTask() {
		if (!m_thread)return;
		// �z��T�C�Y���ς��̂ł�����Max�l������Ă��ꂾ����
		int max = m_tasks.size();
		for (int num = 0; num < max; num++) {
			bool comp = m_tasks[num]->IsCompletedTask();
			// �^�C���A�E�g���Ă���ΏI��
			if (comp) {
				delete m_tasks[num];
				--num;
				--max;
			}
		}

		if (m_tasks.empty())
			m_thread->join();
	}
private:
	std::vector<C_Task*> m_tasks;// ���̔񓯊��ɂ�����������
	std::thread* m_thread = nullptr;

};

class C_Task
{
public:
	C_Task(int resVal = 1) {
		m_res = resVal;
		// ���L��ԍ쐬
		m_future = m_promise.get_future();
	};
	~C_Task() {};

	// ���̃^�X�N�ɂ�������������ǉ�
	void AddFunc(std::function<void()> f) {
		m_funcs.emplace_back(f);
	}
	// ����
	void ExecuteTask() {
		m_funcs.emplace_back([=]() {m_promise.set_value(m_res); });
		for (auto f : m_funcs)
			f();
	}
	// ����������ꂽ���̃`�F�b�N
	bool IsCompletedTask() {
		// 1�b�ҋ@���A�������������Ă��Ȃ����False�E���Ă���Γ��������True
		std::chrono::steady_clock::time_point timePoint = std::chrono::steady_clock::now();
		std::future_status res = m_future.wait_until(timePoint + std::chrono::seconds(1));

		if (res != std::future_status::timeout) {
			m_future.get();
			return true;
		}
		else
			std::cout << "timeout" << std::endl;

		return false;
	}

private:
	std::vector<std::function<void()>> m_funcs;	// ���̔񓯊��ɂ�����������
	std::promise<int> m_promise;				// ������ɕԂ��l
	std::future<int> m_future;					// �񓯊��������ʂ��󂯎�邽�߂̕ϐ�
	int m_res = 1;

};
