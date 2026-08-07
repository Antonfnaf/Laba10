#include <iostream>

#include "Core/enums.h"

#include "UIEngine/rendering/Renderer.h"

#include "Tests/Graphics/test1.h"
            
void PrintAllColors() {
	int n, m;
	n = 1;
	m = 16;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			std::cout << "\033[48;5;" << (i * m + j) % 256 << "m  ";
		}
		std::cout << std::endl;
	}
	n = 6;
	m = 36;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			std::cout << "\033[48;5;" << (i * m + j+ 16) % 256 << "m  ";
		}
		std::cout << std::endl;
	}
	n = 1;
	m = 24;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			std::cout << "\033[48;5;" << (i * m + j + 232) % 256 << "m  ";
		}
		std::cout << std::endl;
	}
}

int main()
{
	
    //PrintAllColors();
    Test1::StartTest(100000);
    //PrintAllColors();
	//std::cout << "\033[48;2;23;34;123mD";
    //Field* field = new Field(222, 222, 4, 7);*/
    ////field->Print();
    //std::cout << '\u256C\n';
}
