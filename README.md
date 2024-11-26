# Groupe 9 - ESME

## **Auteurs**
- **Marwa Mostaghfir**
- **Kim Davy NDAYONGEJE**
- **Gaby Paultre**
- **Lou Allard**

---

## **Description du projet**
Ce projet illustre les différences de gestion de la mémoire entre deux types de systèmes :
- **Avec MMU** (Memory Management Unit) : Protection stricte empêchant les processus d'accéder à des zones mémoire non autorisées.
- **Avec MPU** (Memory Protection Unit) : Protection moins stricte permettant des accès inter-processus, au risque de compromettre la stabilité ou la sécurité.

Le programme met en évidence ces différences en manipulant un buffer global pour simuler des scénarios d'accès autorisé et non autorisé.

### **Objectifs principaux**
- Sur un système avec **MMU** : Toute tentative d'accès non autorisé provoque une erreur de segmentation.
- Sur un système avec **MPU** : L'accès non autorisé est possible, démontrant l'absence de protection stricte.

---

## **Fonctionnalités principales**
### **Mode 1 :**
- Création d'un buffer global contenant la chaîne `"Il fait beau"`.
- Affichage de l'adresse de ce buffer.
- Attente d'une minute avant de terminer, permettant à un autre processus de modifier cette zone mémoire.

### **Mode 2 :**
- Permet la modification du contenu du buffer global en utilisant l'adresse affichée par le Mode 1.
- Remplace `"Il fait beau"` par `"La tempête arrive !"`.

---

## **Prérequis**
- Un système embarqué ou une plateforme compatible (testé sur **uClinux**).
- Un compilateur compatible (natif ou croisé, comme `gcc` ou `arm-uclinuxeabi-gcc`).
- Le **Makefile** fourni pour faciliter la compilation.

---

## **Lancement du programme**

### **Étape 1 : Compilation**
Utilisez le Makefile pour compiler le programme :
```bash
make
```

### **Étape 2 : Exécution**
1. **Exécuter le Mode 1 en arrière-plan :**
   ```bash
   ./explomem2 1 &
   ```
   Cela affichera l'adresse du buffer global et attendra une minute.

2. **Exécuter le Mode 2 dans un autre terminal :**
   ```bash
   ./explomem2 2 [adresse_du_buffer]
   ```
   Remplacez `[adresse_du_buffer]` par l'adresse affichée par le Mode 1.

---

## **Structure des fichiers**
- **`explomem2.c`** : Code source principal du programme.
- **`Makefile`** : Automatisation de la compilation pour plateformes natives et embarquées.
- **`README.md`** : Documentation du projet.

---

## **Variables du Makefile**
Le **Makefile** inclus supporte la cross-compilation et permet une personnalisation facile via les variables suivantes :

- **`S`** : Répertoire source contenant les fichiers `.c`.
- **`D`** : Répertoire de destination pour les fichiers compilés.
- **`ARM_CC`** : Compilateur pour la cross-compilation (ex. `arm-uclinuxeabi-gcc`).
- **`ARM_CFLAGS`** : Options spécifiques pour le processeur ARM Cortex-M4.
- **`LDFLAGS`** : Options de l'éditeur de liens.
- **`DEFINES`** : Macros conditionnelles activables (ex. `-DWITH_TEXT`).

Ces variables permettent de configurer facilement le Makefile en exportant les chemins et options nécessaires.

---

## **Résultats attendus**

### **Système avec MMU :**
- Toute tentative de modification (Mode 2) provoque une **Erreur de segmentation**.

### **Système avec MPU :**
- La modification réussit, mettant en lumière l'absence de protection stricte.

---
