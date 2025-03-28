
#include "serverData.h"
#include "nbt/nbtSerializer.h"

void serverData::serializeValue(nbtSerializer& s)
{
	s.serializeValue(L"name", serverName);
	s.serializeValue(L"port", serverPort);
	if (s.write) {
		std::wstring ipAdressString = stringToWString(serverIPAddress.toString());
		s.serializeValue(L"ip adress", ipAdressString);
	}
	else {
		std::wstring ipAdressString;
		s.serializeValue(L"ip adress", ipAdressString);
		if (const auto validAdress = sf::IpAddress::resolve(WStringToString(ipAdressString))) {
			serverIPAddress = *validAdress;
		}
		else {
			serverIPAddress = sf::IpAddress(0);
		}
	}

}
