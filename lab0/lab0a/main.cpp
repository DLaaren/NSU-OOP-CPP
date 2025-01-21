#include "module1.h"
#include "module2.h"
#include <iostream>

int main(int argc, char** argv)
{
	std::cout <<  "Hello world!" << "\n";
	
	std:: cout << "------\n";
	std:: cout << "Прямое обращение серез пространство имен\n\n";

	std::cout << Module1::getMyName() << "\n";
	std::cout << Module2::getMyName() << "\n";


	std:: cout << "------\n";
	std:: cout << "Подключение пространства имен Module1\n\n";

	using namespace Module1;
	std::cout << getMyName() << "\n"; // (A)
	std::cout << Module2::getMyName() << "\n";

	// приведет к ошибке потому как два пространства имеют одинаковые по названию функции
	//using namespace Module2; // (B)
	//std::cout << getMyName() << "\n"; // COMPILATION ERROR (C)

	std:: cout << "------\n";
	std:: cout << "Использование только функции getMyName из пространства имен Module2\n\n";
	
	using Module2::getMyName;
	std::cout << getMyName() << "\n"; // (D)

}
