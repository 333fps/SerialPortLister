#pragma once

#pragma comment(lib, "Setupapi.lib")

#include <Windows.h>
#include <setupapi.h>

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

class SerialPortLister
{
public:
	struct SerialPortInfo
	{
		std::string portName;
		std::string description;
		std::string friendlyName;
		std::string manufacturer;

		bool operator<(const SerialPortInfo& oher) const
		{
			return std::stoi(portName.substr(3, portName.length() - 3))
				< std::stoi(oher.portName.substr(3, oher.portName.length() - 3));
		}
	};

	SerialPortLister();
	~SerialPortLister() {}

	void sort();

	const std::vector<SerialPortInfo>& ports() { return this->m_ports; };

private:
	SP_DEVINFO_DATA m_deviceInfoData{};
	HDEVINFO m_deviceInfo;

	std::vector<SerialPortInfo> m_ports;
	void name(std::string& p_portName);
	void registryProperty(DWORD p_property, std::string& p_string);
};
