
#include <iostream>

bool isValidNickname(const std::string& nick)
{
    if (nick.empty() || nick.length() > 9)
        return false;

    // Le premier caractère : lettre ou underscore
    if (!std::isalpha(nick[0]) && nick[0] != '_')
        return false;

    // Vérification des caractères
    for (size_t i = 0; i < nick.length(); i++)
    {
        if (!std::isalnum(nick[i]) && nick[i] != '_')
            return false;
    }

    return true;
}


void tryAuthWithPass(Client *client, const Command &cmd, const std::string &serverPassword)
{
	std::string pass = cmd.params[0];
	if(pass != serverPassword)
		return;
	client->setAuthenticated(true);
}