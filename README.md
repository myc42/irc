# ft_irc - Internet Relay Chat Server 💬

Implémentation d’un serveur **IRC (Internet Relay Chat)** en **C++98**, réalisée dans le cadre du cursus de l’:contentReference[oaicite:0]{index=0}.

Repository : https://github.com/myc42/irc

---

## 📌 Description

**ft_irc** est une implémentation d’un serveur IRC respectant le protocole IRC.

Le serveur :

- Gère plusieurs clients simultanément
- Permet la création et la gestion de channels
- Supporte les messages privés et publics
- Implémente une authentification par mot de passe
- Gère les permissions et opérateurs

L’objectif du projet est de comprendre en profondeur la programmation réseau bas niveau et le fonctionnement d’un serveur multi-clients en respectant des contraintes strictes en **C++98**.

---

## 📥 Installation

```bash
git clone https://github.com/myc42/irc.git
cd irc
```

---

## ⚙️ Compilation

```bash
make
```

---

## 🚀 Lancement

```bash
make 1
```

Configuration par défaut :

- **Port** : `6667`
- **Mot de passe** : `0`

---

## 💬 Connexion

Le serveur peut être testé avec :

### 🟢 :contentReference[oaicite:1]{index=1}

```bash
irssi
/connect 127.0.0.1 6667 0
```

### 🟢 :contentReference[oaicite:2]{index=2}

```bash
nc 127.0.0.1 6667
```

---

## 🧠 Défis techniques

- Utilisation de `poll()` pour le multiplexage d’E/S
- Gestion des sockets en mode non-bloquant
- Aucune utilisation de threads
- Respect strict du standard C++98 (pas de C++11+)
- Aucune bibliothèque externe

---

## 🧩 Commandes implémentées

- `PASS`
- `NICK`
- `USER`
- `JOIN`
- `PART`
- `PRIVMSG`
- `QUIT`

---

## 🛠 Technologies utilisées

- **C++98**
- Sockets BSD (TCP)
- `poll()`
- Makefile

---

## 👥 Auteurs

- myc42  
- kamel  
- younes  
