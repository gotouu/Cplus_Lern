#pragma once
#include<iostream>
#include<vector>
#include<functional>
#include<future>
//MEMO
//std::promise�Afuture
//Set�����Ƃ��܂ł̏�����Ԃ��̂Ń��C���X���b�h�Ŏ󂯎���āiGet���āj���ꂽ�甽�f���܂���B
//�������A�Ppromis�ɕt���PSetGet�����ł��Ȃ����ߕ��������Ԃ��|�C���g���~�����ꍇ�͕���Promise���K�v�B
//�󂯎��Future�͈�ł���
//
//std::thread
//�Ƃɂ����񓯊��œ����������B�A�v���P�[�V�����I�����܂łɂ����Join�Ŋ��������Ƃ��Ȃ���΂����Ȃ�

class C_Task;

/*
* ���T�v���񓯊���Task����������N���X�B
*/
class C_Thread
{
public:
	C_Thread() {};
	~C_Thread() {};

	void AddTask(C_Task& task);
	void ExecuteTasks();
	void TryGetTask();
private:
	enum class STATE
	{
		DEFAULT,
		EXECUTE,
	};
	STATE m_state = STATE::DEFAULT;
	std::vector<C_Task*> m_tasks;// ���̔񓯊��ɂ�����������
	std::thread* m_thread = nullptr;

};

/*
* ���T�v��Thread�ɂ�����������������N���X�B
* ���x����Thread���ł͔z��ŏ�����Ԃ��Ǘ����邾���Ȃ̂ŕK��UniquePointer�Ő錾���K�x�[�W�R���N�V���������鎖
*/
class C_Task
{
public:
	C_Task(int resVal = 1) {
		m_res = resVal;
		// ���L��ԍ쐬
		m_future = m_promise.get_future();
	};
	~C_Task() {
		std::cout << "delete task" << std::endl;
	};

	// ���̃^�X�N�ɂ�������������ǉ�
	void AddFunc(std::function<void()> f);
	// ����
	void ExecuteTask();
	// ����������ꂽ���̃`�F�b�N
	bool IsCompletedTask();
private:
	std::vector<std::function<void()>> m_funcs;	// ���̔񓯊��ɂ�����������
	std::promise<int> m_promise;				// ������ɕԂ��l
	std::future<int> m_future;					// �񓯊��������ʂ��󂯎�邽�߂̕ϐ�
	int m_res = 1;

};

// Ver1
/*
template<class T>
using shared_promise = std::shared_ptr<std::promise<T>>;

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
