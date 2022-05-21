#include "SerialPortLister.h"

int main()
{
	SerialPortLister serialPortLister;

	std::cout << "\nBefore sorting...\n" << std::endl;

	for (auto& port : serialPortLister.ports())
	{
		std::cout
			<< port.portName << "\t"
			<< port.description << "\t"
			<< port.friendlyName << "\t"
			<< port.manufacturer << std::endl;
	}

	serialPortLister.sort();

	std::cout << "\nAfter sorting...\n" << std::endl;

	for (auto& port : serialPortLister.ports())
	{
		std::cout
			<< port.portName << "\t"
			<< port.description << "\t"
			<< port.friendlyName << "\t"
			<< port.manufacturer << std::endl;
	}

	return 0;
}