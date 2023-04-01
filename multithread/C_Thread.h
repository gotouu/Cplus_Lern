#pragma once
#include<iostream>
#include<vector>
#include<functional>
#include<future>
//MEMO
//std::promise、future
//Setしたとこまでの処理を返すのでメインスレッドで受け取って（Getして）くれたら反映しますよ。
//ただし、１promisに付き１SetGetしかできないため複数処理返すポイントが欲しい場合は複数Promiseが必要。
//受け取るFutureは一つでいい
//
//std::thread
//とにかく非同期で動かすだけ。アプリケーション終了時までにこれをJoinで完了させとかなければいけない

class C_Task;

/*
* ＜概要＞非同期にTaskを処理するクラス。
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
	std::vector<C_Task*> m_tasks;// この非同期にさせたい処理
	std::thread* m_thread = nullptr;

};

/*
* ＜概要＞Threadにさせたい処理をするクラス。
* ＜警告＞Thread側では配列で所持状態を管理するだけなので必ずUniquePointerで宣言しガベージコレクションさせる事
*/
class C_Task
{
public:
	C_Task(int resVal = 1) {
		m_res = resVal;
		// 共有状態作成
		m_future = m_promise.get_future();
	};
	~C_Task() {
		std::cout << "delete task" << std::endl;
	};

	// このタスクにさせたい処理を追加
	void AddFunc(std::function<void()> f);
	// 処理
	void ExecuteTask();
	// 完了させられたかのチェック
	bool IsCompletedTask();
private:
	std::vector<std::function<void()>> m_funcs;	// この非同期にさせたい処理
	std::promise<int> m_promise;				// 処理後に返す値
	std::future<int> m_future;					// 非同期処理結果を受け取るための変数
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

	// このスレッドで処理したいものを登録する
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
	// このスレッドを開始する
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
		// 登録されたPromiseが無くてFutureも共有状態が無いなら処理するものが無いので終了
		if ((m_promises.size() == 0) && (!m_future.valid()))return;

		// 共有状態を持っていなければ先頭のpromiseから取得
		if (!m_future.valid()) {
			std::promise<int>* promise = m_promises.front().get();
			m_future = promise->get_future();
		}

		// 1秒待機し、処理が完了していなければスキップ・していれば同期を取る
		std::chrono::steady_clock::time_point timePoint = std::chrono::steady_clock::now();
		std::future_status res = m_future.wait_until(timePoint + std::chrono::seconds(1));
		if (res != std::future_status::timeout)
			m_future.get();
		else
			std::cout << "timeout" << std::endl;
	};

	void GetFutureAll() {
		// 配列サイズが変わるのでここでMax値を取ってそれだけ回す
		int max = m_promises.size();
		for (int num = 0; num < max; num++) {
			GetFutureFront();
			// タイムアウトしていれば終了
			if (m_future.valid())
				return;
		}
		m_promises.clear();
	}

	// このスレッドの終了を待つ
	void Join() {
		m_thread->join();

	}
private:
	std::vector<std::function<void()>> m_funcs;	// この非同期にさせたい処理
	std::list<shared_promise<int>> m_promises;	// 処理後に返す値
	std::future<int> m_future;					// 非同期処理結果を受け取るための変数
	std::thread* m_thread = nullptr;
};
*/
