#include "C_Thread.h"

void C_Thread::AddTask(C_Task& task)
{
	// ������ԂɂȂ��ꍇ�ǉ��ł���
	if (m_state == STATE::DEFAULT)
		m_tasks.emplace_back(&task);
}

void C_Thread::ExecuteTasks()
{
	// ������ԂɂȂ��ꍇ�J�n
	if (m_state != STATE::DEFAULT)return;

	m_state = STATE::EXECUTE;
	m_thread = new std::thread(
		[this]() {
			for (auto task : m_tasks)
				task->ExecuteTask();
		}
	);
}

void C_Thread::TryGetTask()
{
	// ������ԂɂȂ��ꍇ�ł��Ȃ�
	if (m_state != STATE::EXECUTE)return;

	// �z��T�C�Y���ς��̂ł�����Max�l������Ă��ꂾ����
	int max = m_tasks.size();
	for (int num = 0; num < max; num++) {
		bool res = m_tasks[num]->IsCompletedTask();
		// �������Ă����ꍇ��菜��
		if (res) {
			auto itr = m_tasks.begin();
			std::advance(itr, num);
			m_tasks.erase(itr);
			--num;
			--max;
		}
	}

	if (m_tasks.empty())
		Join();
}

void C_Thread::Join()
{
	m_thread->join();
	m_state = STATE::DEFAULT;
}



void C_Task::AddFunc(std::function<void()> f)
{
	m_funcs.emplace_back(f);
}

void C_Task::ExecuteTask()
{
	// �Ō��SetValue���I����ʒm����
	m_funcs.emplace_back([=]() {m_promise.set_value(m_res); });
	for (auto f : m_funcs)
		f();
}

bool C_Task::IsCompletedTask()
{
	// 1�b�ҋ@���A�������������Ă��Ȃ����False�E���Ă���Γ��������True
	std::chrono::steady_clock::time_point timePoint = std::chrono::steady_clock::now();
	std::future_status res = m_future.wait_until(timePoint + std::chrono::seconds(1));

	if (res != std::future_status::timeout) {
		m_future.get();
		return true;
	}
	else
		std::cout << "task timeout" << std::endl;

	return false;
}
