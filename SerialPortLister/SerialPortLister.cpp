#include "SerialPortLister.h"

SerialPortLister::SerialPortLister()
{
	GUID guidComPorts = GUID(GUID_DEVINTERFACE_COMPORT);

	this->m_deviceInfo = SetupDiGetClassDevsA(
		&guidComPorts, 0, nullptr,
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	if (this->m_deviceInfo == INVALID_HANDLE_VALUE)
	{
		return;
	}

	int deviceIdx{ 0 };
	while (true)
	{
		this->m_deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

		if (!SetupDiEnumDeviceInfo(this->m_deviceInfo, deviceIdx, &this->m_deviceInfoData))
		{
			break;
		}

		this->m_ports.emplace_back(SerialPortInfo());
		this->name(this->m_ports.back().portName);
		this->registryProperty(SPDRP_DEVICEDESC, this->m_ports.back().description);
		this->registryProperty(SPDRP_FRIENDLYNAME, this->m_ports.back().friendlyName);
		this->registryProperty(SPDRP_MFG, this->m_ports.back().manufacturer);

		++deviceIdx;
	}
}

void SerialPortLister::name(std::string& p_portName)
{
	auto hKey = SetupDiOpenDevRegKey(this->m_deviceInfo, &this->m_deviceInfoData,
		DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);

	if (hKey == nullptr)
	{
		return;
	}

	DWORD type;
	DWORD size;

	if (RegQueryValueExA(hKey, "PortName", NULL, &type, NULL, &size) != ERROR_SUCCESS)
	{
		return;
	}

	if (RegQueryValueExA(hKey, "PortName", NULL, &type, (LPBYTE)p_portName.c_str(), &size) != ERROR_SUCCESS)
	{
		return;
	}

	p_portName = std::string(p_portName.c_str());
}

void SerialPortLister::registryProperty(DWORD p_property, std::string& p_string)
{
	DWORD type;
	DWORD size;

	if (!SetupDiGetDeviceRegistryPropertyA(this->m_deviceInfo, &this->m_deviceInfoData, p_property, &type, nullptr, 0, &size))
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			return;
		}
	}

	std::vector<BYTE> buffer{ size, std::allocator<BYTE>{} };
	if (!SetupDiGetDeviceRegistryPropertyA(this->m_deviceInfo, &this->m_deviceInfoData, p_property, &type, (PBYTE)buffer.data(), size, &size))
	{
		return;
	}

	if (type != REG_SZ)
	{
		return;
	}

	p_string = reinterpret_cast<const char*>(buffer.data());
}

void SerialPortLister::sort()
{
	std::sort(m_ports.begin(), m_ports.end());
}