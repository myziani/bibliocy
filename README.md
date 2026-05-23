# CY-biblioTECH

CY Tech — Pré-ING1, Semestre 2 — 2025/2026

---

## Description

Programme en langage C de gestion d'une bibliothèque universitaire.
Il permet de gérer les comptes utilisateurs, le catalogue de livres, et les emprunts en appliquant les règles de la bibliothèque selon le rôle de l'utilisateur.
Toutes les données sont sauvegardées dans des fichiers texte lisibles et rechargées automatiquement à chaque lancement.

---

## Fonctionnalités

**Comptes utilisateurs**
- Création de compte avec login, mot de passe et rôle (étudiant ou professeur)
- Connexion sécurisée
- Deux rôles : `E` = étudiant, `P` = professeur

**Catalogue de livres**
- Affichage trié par titre, par auteur ou filtré par catégorie
- Ajout d'un nouveau livre (professeurs uniquement)
- Sauvegarde automatique dans `books.txt`

**Emprunts**
- Emprunt d'un livre disponible avec vérification des règles
- Retour d'un livre
- Affichage des emprunts en cours avec le temps restant (ou le retard en secondes)
- Sauvegarde automatique dans `loans.txt`

**Règles de la bibliothèque**
| Règle | Étudiant | Professeur |
|---|---|---|
| Livres simultanés max | 3 | 5 |
| Durée d'emprunt | 2 minutes | 3 minutes |
| Emprunt bloqué si retard | oui | oui |

> En cas de retard, l'utilisateur doit d'abord rendre tous ses livres en retard avant de pouvoir en emprunter un nouveau. Il est impossible d'emprunter un livre déjà pris par quelqu'un d'autre.

---

## Structure du projet

```
CY-biblioTECH/
├── main.c            # Point d'entrée, menus, fonctions utilitaires partagées
├── livre.c           # Chargement, sauvegarde, tri et ajout de livres
├── livre.h
├── utilisateur.c     # Inscription, connexion, gestion des comptes
├── utilisateur.h
├── emprunt.c         # Emprunter, rendre, vérification des règles et des retards
├── emprunt.h
├── books.txt         # Catalogue des livres (généré/mis à jour automatiquement)
├── users.txt         # Liste des utilisateurs (généré/mis à jour automatiquement)
├── loans.txt         # Emprunts en cours (généré/mis à jour automatiquement)
└── test.sh           # Script de compilation et de lancement
```

---

## Format des fichiers

Les fichiers sont en texte brut, lisibles et modifiables directement.
Les lignes vides et les lignes commençant par `#` sont ignorées.

**`books.txt`** — 4 lignes par livre :
```
id
titre
auteur
categorie
```

**`users.txt`** — 3 lignes par utilisateur :
```
login
mot_de_passe
role  (E ou P)
```

**`loans.txt`** — 3 lignes par emprunt :
```
id_livre
login
timestamp_unix
```

---

## Compilation et lancement

**Sous Linux / WSL :**
```bash
gcc -Wall -std=c99 -O2 main.c livre.c utilisateur.c emprunt.c -o bibliotech
./bibliotech
```

**Ou avec le script fourni :**
```bash
bash test.sh
```

---

## Comptes de test

| Login | Mot de passe | Rôle |
|---|---|---|
| admin | admin | Professeur |
| alice | 1234 | Étudiante |
| choco | chocolat | Professeur |

---

## Auteurs

- **Myriam Ziani**
- **Zineb Glizi**
- **Yasser Torkhani**
