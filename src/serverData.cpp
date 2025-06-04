
#include "serverData.h"
#include "nbt/nbtSerializer.h"

void serverData::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, L"name", serverName);
	serializeNBTValue(s, L"port", serverPort);
	if (s.write) {
		std::wstring ipAdressString = stringToWString(serverIPAddress.toString());
		serializeNBTValue(s, L"ip adress", ipAdressString);
	}
	else {
		std::wstring ipAdressString;
		serializeNBTValue(s, L"ip adress", ipAdressString);
		if (const auto validAdress = sf::IpAddress::resolve(WStringToString(ipAdressString))) {
			serverIPAddress = *validAdress;
		}
		else {
			serverIPAddress = sf::IpAddress(0);
		}
	}

}
