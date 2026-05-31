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
Début du projet : Le professeur nous présente différents projets.
Nous avons choisi le PCBot car le projet nous paraît très intéressant même s'il en est un gros. Nous avons également utilisé cette séance pour découvrir git et github. Nous avons commencé les recherches des datasheets des différents composants du PCBot; et nous avons listé les différentes fonctionnalités de celui-ci.

### 12/02/2026
Draw.io :
- Schéma architectural

Kicad : 
- Début du schematic
- Modification des symboles déjà existants sur Kicad pour avoir des composants qui se rapprochent de ceux que nous allons utiliser

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
- Commentaire des différents fichiers .c et .h
- Recherche d'une librairie pour le capteur TOF

### 06/05/2026
Software : 
- Réussite à faire communiquer deux nRF24 avec deux Nucléos-L476RG

### 07/05/2026
Pratique :
- Soudure du PCB
Software :
- Test du code pour la centrale inertielle

### 13/05/2026
Pratique :
- Resoudure du PCB (car nous avons eu des problèmes la première fois)

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



Introduction
Le projet sur lequel nous travaillons est la conception d'un PCBot qui connaît sa position exacte dans l'espace et se déplace. Il permet de cartographier une portion de l'espace autour de lui en calculant la distance qui le sépare de l'obstacle, et en communiquant avec d'autres PCBot, ils peuvent reconstruire la cartographie d'un environnement.

Choix des composants : 
- On a choisi un BMS BQ25896RTWR, qui permet de charger la batterie car elle possède une large place de tension d'entrée (3.9V à 14V), permettant l'utilisation d'adaptateurs standards(5V) ou haute tension (9V/12V) avec une efficacité de plus de 90% à 3A.
- On a choisi un driver pour le moteur DRV8411APWPR pour la compacité du boîtier et sa simplicité, avec une large plage de fonctionnement de 1.65V à 11V.
- On a choisi la centrale inertielle LSM6DSOX pour repérer le robot, qui possède un accéléromètre et un gyroscope (que l'on n'utilisera pas dans le projet) qui a une haute résistance aux chocs mécaniques
- On a choisi les moteurs DFR1224 à courant continu pour les roues
- On a choisi le capteur TOF VL53LOCXVODH1 qui mesure la distance entre l'obstacle et le robot pour sa haute performance même en journée, 
- On a choisi le nRF24 pour la communication entre les robots sans fil

Pour le routage, nous avons choisi d'optimiser le placement en mettant à côté ceux qui doivent rester proches, et de minimiser au maximum la distance entre les composants comme les condensateurs de découplage.
On a optimisé la taille du PCB en choisissant la taille minimum nécessaire qui est de 5.9 x 6.8cm pour l'écologie.

Plus généralement, on a choisi ces composants pour l'écologie, l'optimisation de l'espace et de coût financier, et/ou pour ses fonctionnalités.

Ce qui nous a le plus surpris lors du projet ce sont le soudage en général où nous avons échoué une première fois car lorsqu'on a mis au four, les composants ont bougé et nous devions recommencer, et également lors du soudage du BMS qui est assez complexe, et le fonctionnement du capteur TOF qui ne fonctionnait pas au début car on a oublié de mettre XSHUNT à 1 dans notre code.



Dans le readme : pas forcément tout mettre. il vaut mieux être précis et aller en profondeur, plutôt qu'essayer de tout mettre mais de façon superficielle. on peut mettre un bout du kicad/routage si par exemple on cherche à montrer quelque chose en particulier (condensateurs de découplage proches du composant principal?)


### IMU
Dans le cadre du projet, nous avons réalisé quelques tests avec le composant LSM6DSOX (Adafruit 4438), notre objectif était de pouvoir obtenir la distance parcourue par le composant à partir des données accélérométriques (par double intégration). Nous avons relié les lignes SDA et SCL de l’imu sur les broches PB6 et PB7 du microcontrôleur STM32L476RG.
Avant de faire ces tests, nous avons utilisé le registre WHO_AM_I du composant d’adresse 0x0F et qui contient 0x6C (108). Après lecture du contenu à cette adresse nous avons obtenu cette valeur, cela montre que le capteur est bien connecté.
Les registres OUTX et OUTY permettent de mesurer l'accélération selon l’axe x et l’axe y, cependant, dans le cadre de ces tests, nous allons plutôt nous concentrer sur un seul axe.
L'accélération est codée sur 16 bits au total, mais le bus I2C ne peut transférer que 8 bits à la fois. Donc le capteur découpe la valeur en deux morceaux de 8 bits et les range dans deux registres séparés :
- OUTX_L_A (0x28) contient les 8 bits de poids faible
- OUTX_H_A (0x29) contient les 8 bits de poids fort.

*ajouter tableaux 9.34 et 9.35*

Le registre CTRL1_XL d’adresse 0x10 est le registre de configuration principal de l'accéléromètre. Il est composé de 8 bits répartis en trois parties :
- Les 4 bits de poids fort (ODR_XL3 à ODR_XL0) règlent la fréquence de mesure
- Les 2 bits suivants (FS1_XL, FS0_XL) règlent la plage de mesure
- Le bit LPF2_XL_EN active ou non un filtre passe-bas

*ajouter photo CRL1_XL (10h)*

Pour nos tests nous avons choisi d’envoyer 0x40 à ce registre qui correspond en binaire à 0100 0000, on a donc :
- 0100 pour ODR_XL qui correspond à 104 Hz dans le tableau pour un mode normal
- 00 pour FS_XL qui correspond à la plage ±2g (valeur par défaut)
- 00 pour le reste: filtre désactivé

L'erreur renvoyée par l'IMU sur la position est justifiée par le fait qu'on intègre deux fois l'erreur, vu qu'on intègre l'accélération puis la position.
Voici ce qu'on observe :
![Mon super GIF](IMU.gif)


### nRF24
Pour ce projet, nous avons besoin de faire communiquer deux robots distants.
Les capteurs LiDAR permettent une mesure précise, mais leur coût élevé et leur complexité de mise en œuvre (drivers, protocoles temps réel) dépassent les contraintes du projet. Nous avons donc opté pour une communication radio bas-coût avec le module nRF24L01+, qui offre une liaison sans fil simple à intégrer via SPI et suffisamment fiable pour nos besoins.
Nous avons relié deux modules nRF24L01+ à deux cartes Nucleo-L476RG et vérifié que les deux microcontrôleurs pouvaient s'échanger des messages de manière bidirectionnelle :
![Mon super GIF](nRF24.gif)

## Connexion matérielle
Le nRF24L01+ communique via le bus SPI. Voici le câblage utilisé sur le STM32L476RG :
-	CE → PA8 (contrôle émission/réception)
-	CSN → PB6 (Chip Select SPI)
-	SCK → PA5 (SPI1_SCK)
-	MOSI → PA7 (SPI1_MOSI)
-	MISO → PA6 (SPI1_MISO)
-	VCC → 3.3 V  |  GND → GND

## Configuration du module
Les principaux paramètres configurés dans nos tests :
-	Canal RF : canal 76, libre des perturbations Wi-Fi les plus courantes.
-	Débit : 1 Mbps — bon compromis portée/fiabilité pour notre usage.
-	Puissance d'émission : 0 dBm (niveau max), pour assurer la fiabilité en intérieur.
-	Taille de payload : 32 octets (mode statique).
-	Adresses : une adresse TX et une adresse RX configurées de façon symétrique sur les deux cartes.

La validation de la connexion SPI a été réalisée en lisant le registre CONFIG (adresse 0x00) : si la valeur retournée est cohérente (typiquement 0x08 après reset), le module est bien connecté et répond correctement.

## Approche logicielle
Nous avons distingué deux rôles : un émetteur (TX) et un récepteur (RX), chacun configuré sur une Nucleo. Les fonctions principales sont :
-	nrf24_init() : initialise le SPI et configure les registres (canal, débit, adresses, taille payload).
-	nrf24_send(data, len) : place le module en mode TX, envoie le payload, attend l'acquittement (Auto-ACK).
-	nrf24_receive(buf) : place le module en mode RX, poll le registre STATUS pour détecter une donnée disponible, puis lit le FIFO.

Le bouton poussoir PC13 de la carte émettrice déclenche l'envoi d'un message. La carte réceptrice allume une LED (PA5) à chaque réception confirmée, permettant une vérification visuelle sans débogueur.

## Résultats
Les tests montrent une communication stable entre les deux cartes.



Conclusion
À la fin du projet, nous avons réussi à faire communiquer deux nRF24 entre eux; à faire fonctionner les 2 moteurs de roue, donc le robot peut se déplacer mais il ne peut pas encore s'arrêter librement ni changer de vitesse; les 2 LEDs fonctionnent, D1 est allumé pour indiqué que le PCB est alimenté et D2 clignote et indique qu'il y a une erreur au niveau de la batterie (qui n'est pas connecté).
