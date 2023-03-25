#include<iostream>
#include<vector>
#include<functional>
#include<future>
#include<Windows.h>
//失敗したクラス
//class C_Thread
//{
//public:
//	C_Thread() {
//	};
//	~C_Thread() {};
//
//	// このスレッドで処理したいものを登録する
//	void RegistFunction(std::function<void()> f) {
//		m_funcs.push_back(f);
//	}
//
//	// このスレッドを開始する
//	void ExecuteThread() {
//		m_thread = new std::thread([=]() {ExecuteFuncs(); });
//		m_future = m_promise.get_future();
//	};
//
//	// このスレッドの終了を待つ
//	void Join() {
//		m_thread->join();
//		
//	}
//private:
//	void ExecuteFuncs() {
//		for (auto f : m_funcs)
//			f();
//		m_promise.set_value();
//	};
//	std::vector<std::function<void()>> m_funcs;// この非同期にさせたい処理
//	std::promise<void> m_promise;// 処理後に返す値
//	std::future<void> m_future;// 非同期処理結果を受け取るための変数
//	std::thread* m_thread = nullptr;
//	int m_res;// 非同期処理結果保存用
//};

void main() {
	/*for (int i = 0; i < 5; i++)
	{

		printf("1");
		C_Thread th;
		th.RegistFunction([]()
			{
				printf("2");
		Sleep(1000);
		printf("4");
			});
		th.ExecuteThread();
		printf("3");
		th.Join();
		printf("5\n");
	}*/

	std::promise<int> p00;
	int num = 0;
	printf("1\n");

	//非同期処理登録・開始
	std::thread th([&p00, &num]() {
		printf("m1\n");
	num = 0;
	// この待ちの間にに２・３・４が来てJoinの処理同期でｍ２が来て最後に５が来てほしい
	for (num = 0; num < 100; num++) {
		num++;
		Sleep(10);

	}
	p00.set_value(num);
	printf("m2\n");
		});

	// 非同期のオブジェクトを受け取る future を宣言する
	//std::future<int> f00 = p00.get_future();
	
	printf("2\n");
	printf("3\n");
	printf("4\n");
	//f00.get();
	th.join();//スレッド終了待ち（合流
	printf("5\n");

	std::cout << "------複数スレッド同時処理------"<<std::endl;
	// 実行する度処理順が変わるから〇
	auto threadF = [](std::string str) { for (int i = 0; i < 10; i++) std::cout << str << " : " << i << std::endl; };
	std::thread thread1([=]() {threadF("thread1"); });
	std::thread thread2([=]() {threadF("thread2"); });
	std::thread thread3([=]() {threadF("thread3"); });

	thread1.join();
	thread2.join();
	thread3.join();

}

