#include"main.h"

void main() {

	printf("1\n");
	std::unique_ptr<C_Thread> th = std::make_unique<C_Thread>();
	th->RegistFunction([]() {Sleep(500); printf("m2\n"); }, 1);
	th->RegistFunction([]() {Sleep(500); printf("m2\n"); }, 1);
	th->ExecuteThread();
	th->GetFutureAll();
	th->GetFutureAll();
	printf("3\n");
	printf("4\n");
	Sleep(2000);
	th->Join();
	/*
	//std::promise<int> p00;
	//std::promise<int> p01;
	//int num = 0;
	//printf("1\n");

	////�񓯊������o�^�E�J�n
	//std::thread th([&p00,&p01, &num]() {
	//	printf("m1\n");
	//p00.set_value(num);
	//num = 0;
	//// ���̑҂��̊ԂɂɂQ�E�R�E�S������Join�̏��������ł��Q�����čŌ�ɂT�����Ăق���
	//for (num = 0; num < 100; num++) {
	//	Sleep(10);
	//	num++;

	//}
	//p01.set_value(num);
	//printf("m2\n");
	//	});

	//// �񓯊��̃I�u�W�F�N�g���󂯎�� future ��錾����
	//
	//std::future<int> f00 = p00.get_future();
	//f00.get();
	//
	//printf("2\n");
	//printf("3\n");
	//printf("4\n");
	//f00 = p01.get_future();
	//f00.get();
	//th.join();//�X���b�h�I���҂��i����
	//printf("5\n");

	//std::cout << "------�����X���b�h��������------"<<std::endl;
	//// ���s����x���������ς�邩��Z
	//auto threadF = [](std::string str) { for (int i = 0; i < 10; i++) std::cout << str << " : " << i << std::endl; };
	//std::thread thread1([=]() {threadF("thread1"); });
	//std::thread thread2([=]() {threadF("thread2"); });
	//std::thread thread3([=]() {threadF("thread3"); });

	//thread1.join();
	//thread2.join();
	//thread3.join();
	*/

}
