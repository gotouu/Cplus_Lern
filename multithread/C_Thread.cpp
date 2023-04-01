#include "C_Thread.h"

void C_Thread::AddTask(C_Task& task)
{
	// 処理状態にない場合追加できる
	if (m_state == STATE::DEFAULT)
		m_tasks.emplace_back(&task);
}

void C_Thread::ExecuteTasks()
{
	// 処理状態にない場合開始
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
	// 処理状態にない場合できない
	if (m_state != STATE::EXECUTE)return;

	// 配列サイズが変わるのでここでMax値を取ってそれだけ回す
	int max = m_tasks.size();
	for (int num = 0; num < max; num++) {
		bool res = m_tasks[num]->IsCompletedTask();
		// 完了していた場合取り除く
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
	// 最後にSetValueし終了を通知する
	m_funcs.emplace_back([=]() {m_promise.set_value(m_res); });
	for (auto f : m_funcs)
		f();
}

bool C_Task::IsCompletedTask()
{
	// 1秒待機し、処理が完了していなければFalse・していれば同期を取るTrue
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
