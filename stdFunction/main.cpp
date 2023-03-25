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
			// �����_�ő���ł���
			m_func = [this]() {TypeA();};
		}
		else {
			// �o�C���h�ő������Ɓi���݂��Ă��邩�`�F�b�N�[�����s�j�̃t���[�ŏ��������̂ŏd�����A�댯
			m_func = std::bind(&Hoge::TypeB, this);
		}
		// ������ꂽ�֐������s�����
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