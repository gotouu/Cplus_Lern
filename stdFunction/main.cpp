#include<iostream>
#include<functional>

using namespace std;

class Hoge
{
public:
	Hoge() {};
	~Hoge() {};

	void SetType(bool isA = true) {
		if (isA)
		{
			// ラムダで代入できる
			m_func = [this]() {TypeA();};
		}
		else {
			// バインドで代入すると（存在しているかチェックー＞実行）のフローで処理されるので重たく、危険
			m_func = std::bind(&Hoge::TypeB, this);
		}
		// 代入された関数が実行される
		m_func();
	};



	std::function<void()> m_func = nullptr;
private:
	void TypeA() { printf_s("A"); };
	void TypeB() { printf_s("B"); };


};

int main() {
	Hoge hoge;

	hoge.SetType();

	
	return 0;
}