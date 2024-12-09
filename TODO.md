# AS_2021_Projet_Systeme
Projet systeme en C, conception du jeu PACMAN avec communication serveur-client

###### Options :

- Si le nombre de fantômes / d'étoiles est trop grand la recherche aléatoire des positions de départ s'arrête.
- Il est impossible en début de partie de tomber sur un fantôme / étoile
- Il n'y a pas de limite de terrain, si le joueur / un fantôme dépasse la zone, il apparait de l'autre coté à la même ligne / colonne.
- Les déplacements se font par z(haut),q(gauche),d(droite),s(bas) des touches du clavier.
- Un fantôme peut se déplacer sur un autre fantôme

###### ATTENTION:

- Lors de la connexion client/serveur:
    - dans le fichier client, la fonction existFruit() qui vérifie s'il reste des fruits pour mettre fin à la partie est buguée. Car si un fantôme se trouve sur le fruit dans la gameGrid, il ne comptabilise pas ce fruit et la partie s'arrête sans être finie.
      NB: Une prochaine mise à jour corrigera ce bug dans la v1.0.5.
