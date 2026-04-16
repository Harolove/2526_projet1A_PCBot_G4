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
Début du projet: présentation des différents projets; découvrir git et github; recherche de datasheet sur les différents composants et sur le PCBot, lister les différentes fonctionnalités du PCBot

### 12/02/2026
Schéma architectural sur draw.io; début de schematic sur Kicad et modification des symboles déjà existant sur Kicad pour avoir des composants qui se rapprochent de la réalité

### 19/02/2026
Création des feuilles hiérarchiques
Continuation du schematic sur Kicad

### 12/03/2026
Continuation du schematic sur Kicad

### 19/03/2026
Kicad : ajout de connexions inter-feuille
CubeIDE : début de code (détermination de position, calcul de distance avec obstacle, déplacement dans l'espace)

### 25/03/2026
Kicad :
- Début du routage (ajout des Mounting_Hole_Pad)
- ajout des plans de masse (GND pour couches n°1, 2 et 4)

### 26/03/2026
CubeIDE : Modification du code et ajout de code test (car nous n'avons pas encore la carte)

### 09/04/2026
CubeIDE :
- Correction d'erreurs
