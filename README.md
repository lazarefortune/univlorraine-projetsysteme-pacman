# AS_2021_Projet_Systeme
Projet systeme en C, conception du jeu PACMAN avec communication serveur-client

## Description
Ce projet consiste à développer une version console du célèbre jeu PACMAN. 
Il met en œuvre des concepts de programmation système en C, notamment la gestion des entrées utilisateur et la communication entre un client et un serveur.

## Fonctionnalités principales :
- Gameplay console : Les déplacements sont effectués via les touches du clavier. 
- Objectif : Manger tous les fruits présents dans le labyrinthe tout en évitant les fantômes. 
- Pseudo IA des fantômes : Les fantômes se déplacent de manière aléatoire grâce à un script intégré.
- Conditions de fin :
  - Le jeu se termine lorsque tous les fruits sont collectés. 
  - Une collision avec un fantôme met fin à la partie.

---

## Structure du projet
- main.c : Le script complet sans communication client/serveur. 
- client.c : Gère la logique côté client pour les interactions utilisateur. 
- server.c : Gère la logique serveur pour le suivi de l'état du jeu et la synchronisation. 



## Prérequis
Avant de commencer, assurez-vous que votre environnement remplit les conditions suivantes :
- Un compilateur C est installé sur votre machine (par exemple gcc). 
- Un terminal est disponible pour exécuter le jeu. 
- Optionnel : Docker pour exécuter le projet dans un environnement isolé.
---

## Installation
### Option 1 : Installation avec Docker

1. Construire l'image Docker
```bash
docker build -t pacman .
```

2. Lancer le conteneur Docker
```bash
docker run -it pacman
```

### Option 2 : Installation sans Docker
1. Compiler le programme
```bash
gcc -o pacman main.c
```

2. Lancer le jeu
```bash
./pacman
```
