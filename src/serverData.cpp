
#include "serverData.h"
#include "nbt/nbtSerializer.h"

void serverData::serializeMembers(nbtSerializer& s)
{
	s.serializeMembers(L"name", serverName);
	s.serializeMembers(L"port", serverPort);
	if (s.write) {
		std::wstring ipAdressString = stringToWString(serverIPAddress.toString());
		s.serializeMembers(L"ip adress", ipAdressString);
	}
	else {
		std::wstring ipAdressString;
		s.serializeMembers(L"ip adress", ipAdressString);
		if (const auto validAdress = sf::IpAddress::resolve(WStringToString(ipAdressString))) {
			serverIPAddress = *validAdress;
		}
		else {
			serverIPAddress = sf::IpAddress(0);
		}
	}

}
