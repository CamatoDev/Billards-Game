# Jeu de Billard avec SFML/C++

## Description

Ce projet est une simulation de jeu de billard développée en C++ utilisant la bibliothèque graphique SFML (Simple and Fast Multimedia Library). Il propose une expérience de jeu immersive avec des graphismes détaillés, une physique réaliste et une gestion complète des règles du jeu.

## Table des matières

- [Fonctionnalités](#fonctionnalités)
- [Installation et Configuration](#Installation-et-Configuration)
- [Utilisation](#utilisation)
- [Conception du Jeu](#Conception-du-Jeu)
- [Gestion des Entrées](#Gestion-des-Entrées)
- [Graphismes et Ressources](#Graphismes-et-Ressources)
- [Interface Utilisateur (UI)](#Interface-Utilisateur (UI))
- [Optimisation et Débogage](#Optimisation-et-Débogage)
- [Perspectives d'Avenir](#Perspectives-d'Avenir)
- [Contributions](#Contributions)
- [Auteur](#Auteur)
- [Licence](#Licence)
- [Liens Utiles](#Liens-Utiles)

## Fonctionnalités

- **Simulation Physique Réaliste** : Mouvements et collisions des boules basés sur des modèles physiques avancés.
- **Graphismes et Interface Utilisateur** : Sprites détaillés pour les boules, la table, les poches et la queue de billard.
- **Gestion des Règles du Jeu** : Détection des poches, gestion des scores et des tours des joueurs.
- **Effets Sonores et Musique** : Sons pour les collisions, les poches et les coups de queue, avec option de musique de fond.
- **Contrôles Intuitifs** : Utilisation de la souris, du clavier ou de la manette pour viser et frapper les boules.

## Installation et Configuration

### Prérequis

- [SFML](https://www.sfml-dev.org/download.php) (version 2.5.1 ou supérieure)
- Un compilateur compatible avec C++11 ou supérieur (GCC, Clang, MSVC, etc.)

### Étapes d'installation

1. **Téléchargement de SFML** : Assurez-vous d'avoir téléchargé la version correcte de SFML pour votre système d'exploitation.
2. **Configuration du projet** : Configurez votre projet C++ pour inclure les bibliothèques et les headers de la SFML.
   - Incluez les chemins d'inclusion et de bibliothèque dans votre IDE ou makefile.

```bash
git clone https://github.com/votreutilisateur/JeuDeBillardSFML.git
cd JeuDeBillardSFML
mkdir build
cd build
cmake ..
make
```

## Utilisation

Lancez le jeu depuis l'exécutable généré :

```bash
./BilliardGame
```

Utilisez la souris ou la manette pour viser et frapper les boules. Les règles du jeu suivront celles du billard américain 8-ball.

## Conception du Jeu

### Mécanique de jeu

Le jeu implémente les règles classiques du billard 8-ball.

### Modèle physique

Les mouvements et les collisions des boules sont simulés en utilisant des équations de la physique des cercles, incluant les forces de frottement pour ralentir les boules progressivement.

## Gestion des Entrées

Les joueurs peuvent interagir avec le jeu en utilisant la souris, le clavier ou une manette. Les événements SFML sont utilisés pour gérer les entrées des joueurs.

## Graphismes et Ressources

Les images pour les boules, la table, les poches et la queue de billard sont chargées et affichées en utilisant les classes `sf::Texture` et `sf::Sprite` de SFML.

## Interface Utilisateur (UI)

L'interface utilisateur affiche les scores des joueurs et propose des menus pour démarrer le jeu, mettre en pause ou afficher les règles.

## Audio

Des effets sonores sont ajoutés pour les collisions, les poches et les coups de queue. Une musique de fond peut également être ajoutée pour enrichir l'expérience de jeu.

## Optimisation et Débogage

Le code a été optimisé pour assurer une simulation fluide. Les calculs physiques et le rendu graphique ont été optimisés pour de meilleures performances. 

## Perspectives d'Avenir

- **Optimisation des Performances** : Améliorer les calculs physiques et le rendu graphique.
- **Nouveaux Modes de Jeu** : Ajouter des modes de jeu supplémentaires, y compris un mode multijoueur en ligne.
- **Amélioration des Graphismes** : Ajouter des animations et améliorer les détails graphiques pour plus de réalisme.

## Contributions

1. Forkez ce dépôt.
2. Créez une branche pour votre fonctionnalité ou correction de bug (git checkout -b feature/amazing-feature).
3. Commitez vos modifications (git commit -m 'Add some amazing feature').
4. Poussez vers la branche (git push origin feature/amazing-feature).
5. Ouvrez une Pull Request.

## Auteur

- **[CamatoDev](https://github.com/CamatoDev))**

## Licence

Ce projet est sous licence MIT.

## Liens Utiles

- [Mon profil LinkedIn](https://www.linkedin.com/in/votreprofil)
