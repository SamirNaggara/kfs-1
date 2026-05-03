# KFS-1 : Kernel From Scratch

Ce projet est le premier volet de la série **Kernel From Scratch** de l'école 42. L'objectif est de créer de zéro un noyau (kernel) de système d'exploitation bootable, capable de s'exécuter dans un environnement "freestanding" (sans bibliothèque standard, ni OS sous-jacent) et d'afficher des informations à l'écran.

## 🚀 Fonctionnalités

Le projet remplit la partie obligatoire (afficher "42") et intègre plusieurs fonctionnalités bonus grâce à une architecture modulaire :

- **Boot Multiboot** : Noyau amorçable via GRUB.
- **Affichage VGA (Mode texte)** : Gestion complète de la mémoire vidéo (`0xB8000`).
- **Défilement (Scrolling)** : L'écran défile automatiquement vers le bas lorsque le texte dépasse la 25ème ligne.
- **Curseur Matériel** : Mise à jour de la position du curseur clignotant via les ports I/O du processeur (`outb`).
- **Gestion des Couleurs (Bonus)** : Interface permettant de changer dynamiquement la couleur du texte et du fond.
- **Formatage de chaînes (Bonus)** :
  - Implémentation d'un `printk` (usage interne noyau) gérant les entiers, les caractères, les chaînes et l'hexadécimal.
  - Implémentation d'un `printf` (préparation pour le User Space).

## Architecture du Projet

Le code est structuré en couches pour séparer la logique d'affichage des interactions matérielles pures :

```text
.
├── include/
│   └── terminal/
│       ├── io.h         # Déclarations des ports d'entrée/sortie (outb/inb)
│       ├── keyboard.h   # Headers du clavier (pour KFS-2)
│       ├── printk.h     # Moteur de formatage Kernel
│       ├── stdio.h      # Moteur de formatage User
│       ├── terminal.h   # Logique d'affichage (X/Y, scrolling)
│       └── vga.h        # Constantes matérielles de la carte graphique
├── src/
│   ├── boot.s           # Point d'entrée Assembleur, Multiboot Header & I/O
│   ├── kernel.c         # Fonction kmain() (Entry point C)
│   └── terminal/
│       ├── printk.c     # Implémentation variadique de printf/printk
│       ├── terminal.c   # Algorithmes de l'interface et du scrolling
│       └── vga.c        # Manipulation de l'octet de couleur VGA et du curseur
├── linker.ld            # Script de l'éditeur de liens (Linker)
└── Makefile             # Automatisation de la compilation et création de l'ISO
```

## Prérequis et Compilation

Pour compiler et tester ce noyau, vous aurez besoin des outils suivants installés sur votre machine (idéalement sous Linux) :

- gcc (avec support -m32 pour compiler en 32 bits)
- nasm (pour assembler le bootloader)
- qemu-system-i386 (pour l'émulation)
- grub-pc-bin et xorriso (pour la génération de l'ISO bootable)

## Commandes disponibles

Compiler le noyau et générer l'ISO (kfs.iso) :

make
Lancer l'émulateur QEMU avec le noyau : make run
Nettoyer les fichiers objets : make clean
Nettoyage complet (objets, binaire et ISO) : make fclean

## Détails Techniques

Poids de l'image : Optimisation du grub-mkrescue pour ne charger que le module i386-pc. L'ISO finale pèse environ ~4 Mo, bien en dessous de la limite des 10 Mo imposée par le sujet. Le binaire du kernel pèse quant à lui environ 11 Ko.

Flags de compilation : Le noyau est compilé avec -nostdlib, -nodefaultlibs, -fno-builtin, et -fno-stack-protector pour garantir l'absence totale de dépendances externes.

Arguments Variables : L'absence de la libc requiert l'utilisation des built-ins de GCC (\_\_builtin_va_list, etc.) pour le fonctionnement de printk.
