#include<iostream>
#include<vector>
#include<functional>
#include<future>
#include<Windows.h>
//���s�����N���X
//class C_Thread
//{
//public:
//	C_Thread() {
//	};
//	~C_Thread() {};
//
//	// ���̃X���b�h�ŏ������������̂�o�^����
//	void RegistFunction(std::function<void()> f) {
//		m_funcs.push_back(f);
//	}
//
//	// ���̃X���b�h���J�n����
//	void ExecuteThread() {
//		m_thread = new std::thread([=]() {ExecuteFuncs(); });
//		m_future = m_promise.get_future();
//	};
//
//	// ���̃X���b�h�̏I����҂�
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
//	std::vector<std::function<void()>> m_funcs;// ���̔񓯊��ɂ�����������
//	std::promise<void> m_promise;// ������ɕԂ��l
//	std::future<void> m_future;// �񓯊��������ʂ��󂯎�邽�߂̕ϐ�
//	std::thread* m_thread = nullptr;
//	int m_res;// �񓯊��������ʕۑ��p
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

	std::thread th([&p00, &num]() {
		printf("m1\n");
	// ���̌�ɂQ�E�R������Join�̏����҂��ł��Q�����čŌ�ɂT�����Ăق���
	num = 0;
	for (num = 0; num < 100; num++) {
		num++;
		Sleep(10);

	}
	p00.set_value(num);
	printf("m2\n");
		});

	p00.get_future().get();//�񓯊������J�n
	printf("2\n");
	printf("3\n");
	printf("4\n");
	th.join();//�񓯊������I���҂�
	printf("5\n");
}
