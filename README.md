# 2526_projet1A_PCBot_G4
PCBot fait par le groupe de 4

## Rappels git

### Clé SSH

Dans git bash, lancez la commande suivante :

```bash
ssh-keygen
```

puis affichez la clé :

```bash
cat /c/Users/<USER>/.ssh/id_ed25519.pub
```

Créez une nouvelle clé dans github et collez la clé

### Clonez le projet

Pour récupérer le projet, à faire une fois (par projet).

```bash
git clone git@github.com:Harolove/2526_projet1A_PCBot_G4.git
```

### Faire un commit

A faire à chaque fois que quelque chose fonctionne, et après une séance de travail.

```bash
git status
git add .
git commit -m "Message"
git push
```

### Récupérer la dernière version

A faire à chaque fois qu'on commence à travailler

```bash
git pull
```
## Timeline
### 05/02/2026
Début du projet: Le professeur nous présente différents projets, on a choisi le PCBot car le projet nous paraît très intéressant même si c'est un gros projet. Nous avons également utiliser cette séance pour découvrir git et github. Nous avons commencé les recherches de datasheets sur les différents composants et sur le PCBot; et nous avons listé les différentes fonctionnalités du PCBot.

### 12/02/2026
Draw.io :
- Schéma architectural

Kicad : 
- Début de schematic
- Modification des symboles déjà existant sur Kicad pour avoir des composants qui se rapprochent de la réalité

### 19/02/2026
Kicad :
- Création des feuilles hiérarchiques
- Continuation du schematic sur Kicad

### 12/03/2026
Kicad :
- Continuation du schematic sur Kicad

### 19/03/2026
Kicad : 
- Ajout de connexions inter-feuille
CubeIDE : 
- Début de code (détermination de position, calcul de distance avec obstacle, déplacement dans l'espace)

### 25/03/2026
Kicad :
- Début du routage (ajout des Mounting_Hole_Pad)
- Ajout des plans de masse (GND pour couches n°1, 2 et 4)

### 26/03/2026
CubeIDE :
- Modification du code et ajout de code test (car nous n'avons pas encore la carte)

### 09/04/2026
CubeIDE :
- Correction d'erreurs

### 16/04/2026
CubeIDE :
- Commenter les différents fichiers .c et .h
- Recherche d'une librairie pour le capteur TOF

### 06/05/2026
Software : 
- Réussite à faire communiquer 2 nRF24 avec 2 nucléos

### 07/05/2026
Pratique :
- Soudure du PCB
Software :
- Test du code pour la centrale inertielle

### 13/05/2026
Pratique :
- Resoudure du PCB (car nous avons eu des problèmes pour la première fois)

### 15/05/2026
Software :
- Fonctionnement de la centrale inertielle

### 18/05/2026
Software :
- Test du code pour le capteur tof (mais n'a pas réussi à le faire fonctionner)
Onshape :
- Modélisation d'un support pour le PCB

### 19/05/2026
Onshape :
- Impression et assemblage du support



Intro
Le projet sur lequel nous travaillons est la conception d'un PCBot qui connaît sa position exacte dans l'espace et se déplace. Il permet de cartographier une portion de l'espace autour de lui en calculant la distance qui le sépare de l'obstacle, et en communiquant avec d'autres PCBot, ils peuvent reconstruire la cartographie d'un environnement.

Choix des composants : 
- On a choisi un BMS BQ25896RTWR, qui permet de charger la batterie car elle possède une large place de tension d'entrée (3.9V à 14V), permettant l'utilisation d'adaptateurs standards(5V) ou haute tension (9V/12V) avec une efficacité de plus de 90% à 3A.
- On a choisi un driver pour le moteur DRV8411APWPR pour la compacité du boîtier et sa simplicité, avec une large plage de fonctionnement de 1.65V à 11V.
- On a choisi la centrale inertielle LSM6DSOX pour repérer le robot, qui possède un accéléromètre et un gyroscope (que l'on n'utilisera pas dans le projet) qui a une haute résistance aux chocs mécaniques
- On a choisi les moteurs DFR1224 à courant continu pour les roues
- On a choisi le capteur TOF VL53LOCXVODH1 qui mesure la distance entre l'obstacle et le robot pour sa haute performance même en journée, 
- On a choisi le nRF24 pour la communication entre les robots sans fil

Pour le routage, nous avons choisi d'optimiser le placement en mettant à côté ceux qui doivent rester proches, et de minimiser au maximum la distance entre les composants comme les condensateurs de découplage.
On a optimisé la taille du PCB en choisissant la taille minimum nécessaire qui est de 5,9 x 6.8cm pour l'écologie.

Plus généralement, on a choisi ces composants pour l'écologie, l'optimisation de l'espace et de coût financier, et/ou pour ses fonctionnalités.

Ce qui nous a le plus surpris lors du projet ce sont le soudage en général où nous avons échoué une première fois car lorsqu'on a mis au four, les composants ont bougé et nous devions recommencer, et également lors du soudage du BMS qui est assez complexe, et le fonctionnement du capteur TOF qui ne fonctionnait pas au début car on a oublié de mettre XSHUNT à 1 dans notre code.


Conclusion : À la fin du projet, nous avons réussi à faire communiquer deux nRF24 entre eux; à faire fonctionner les 2 moteurs de roue, donc le robot peut se déplacer mais il ne peut pas encore s'arrêter librement ni changer de vitesse; les 2 LEDs fonctionnent, D1 est allumé pour indiqué que le PCB est alimenté et D2 clignote et indique qu'il y a une erreur au niveau de la batterie (qui n'est pas connecté).
