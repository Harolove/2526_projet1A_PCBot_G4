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

05/02/2026
Début du projet: présentation des différents projets; découvrir git et github; recherche de datasheet sur les différents composants et sur le PCBot, lister les différentes fonctionnalités du PCBot

12/02/2026
Schéma architectural sur draw.io; début de schematic sur Kicad et modification des symboles déjà existant sur Kicad pour avoir des composants qui se rapprochent de la réalité
