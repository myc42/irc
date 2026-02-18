# ft_irc - Internet Relay Chat Server 📡

Une implémentation complète d'un serveur IRC en C++98, développée dans le cadre du cursus de l'école 42.
📖 Description

ft_irc est un serveur de messagerie instantanée respectant le protocole IRC (RFC 1459 & 2812). L'enjeu est de gérer des communications en temps réel via des sockets TCP, sans multi-threading.
🚀 Fonctionnalités :

    Multiplexage d'E/S : Gestion simultanée des clients avec poll().

    Gestion des Channels : Création, jonction (JOIN) et messagerie par canal.

    Messagerie : Support des messages privés (PRIVMSG) et de groupe.

    Sécurité : Authentification par mot de passe obligatoire.

    Administration : Gestion des privilèges opérateurs et des modes.

🧠 Défis Techniques

Le projet repose sur une architecture robuste utilisant exclusivement les outils de la norme C++98 :

    Non-blocking Sockets : Utilisation de fcntl pour garantir que le serveur ne reste jamais bloqué sur une lecture ou une écriture.

    Poll() System : Surveillance constante de l'état des descripteurs de fichiers (lecture/écriture/erreurs).

    Buffer Management : Gestion manuelle des buffers pour traiter les messages fragmentés ou multiples reçus en une seule lecture.

🛠️ Installation et Utilisation
📥 Clonage du projet
Bash

git clone https://github.com/myc42/irc.git
cd irc

⚙️ Compilation

Le projet se compile via un Makefile rigoureux :
Bash

make        # Compile l'exécutable 'ircserv'
make clean  # Supprime les fichiers objets
make fclean # Supprime tout, y compris l'exécutable
make re     # Recompile l'intégralité du projet

⚡ Lancement

Le serveur nécessite un port et un mot de passe pour démarrer :
Bash

# Syntaxe : ./ircserv <port> <password>
./ircserv 6667 mon_password_secret

💬 Connexion des clients

Vous pouvez utiliser n'importe quel client IRC pour vous connecter.
Option A : Irssi (Recommandé) 🛡️
Bash

irssi
/connect localhost 6667 mon_password_secret

Option B : Netcat (Debug) 📟
Bash

nc localhost 6667
PASS mon_password_secret
NICK mon_pseudo
USER guest 0 * :realname

📜 Commandes Implémentées
Commande	Action
PASS	Vérification du mot de passe serveur
NICK	Définition ou changement de pseudonyme
USER	Configuration de l'identité utilisateur
JOIN	Rejoindre un canal de discussion
PART	Quitter un canal
PRIVMSG	Envoyer un message privé ou canal
KICK	Expulser un utilisateur (Opérateur)
INVITE	Inviter un utilisateur dans un canal
TOPIC	Définir ou voir le sujet du canal
MODE	Gérer les privilèges et droits d'accès
QUIT	Fermer la connexion proprement
👥 Équipe (Auteurs)

Ce projet a été réalisé avec passion par :

    myc42 (GitHub)

    kamneh18

    leblitzer
