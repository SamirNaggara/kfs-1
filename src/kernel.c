#include "../include/terminal/terminal.h"
#include "../include/terminal/vga.h"
#include "../include/terminal/print.h"

void kmain(void) 
{
    // Initialisation de l'écran (fond noir, texte gris clair par défaut)
    terminal_initialize();

    /* ==========================================
     * PARTIE OBLIGATOIRE : Afficher 42
     * ========================================== */
    terminal_write("\x03 42 \x03");
    // terminal_write("\n\n");

    /* ==========================================
     * PARTIE BONUS : Démonstration des features
     * ========================================== */
    
    // // 1. Démonstration des couleurs
    // terminal_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    // terminal_write("--- SECTION BONUS KFS-1 ---\n");
    
    // terminal_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    // terminal_write("[ OK ] Gestion des couleurs activee.\n\n");
    
    // // On repasse sur une couleur standard pour la suite
    // terminal_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

    // // 2. Démonstration du printk (Outil Kernel)
    // terminal_set_color(VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
    // terminal_write("1. Test du printk :\n");
    // terminal_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
    // printk("   String : %s | Char : %c\n", "Kernel", 'K');
    // printk("   Nombres: Pos=%d | Neg=%d | Zero=%d\n", 42, -1337, 0);
    // printk("   Hexa   : 0x%x | 0x%x\n\n", 255, 0xABCD);

    // // 3. Démonstration du printf (Outil User/Libc)
    // terminal_set_color(VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
    // terminal_write("2. Test du printf :\n");
    // terminal_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
    // printf("   Hello %s ! Bienvenue sur KFS-1.\n", "42");
    // printf("   Voici un nombre magique : %d\n\n", 1337);

    // 4. Démonstration du Scrolling et du Curseur matériel
    // terminal_set_color(VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK);
    // printf("Génération de lignes pour tester le scrolling de l'ecran...\n");
    
    // Une boucle de 15 lignes suffit pour dépasser la hauteur (25) 
    // et forcer l'écran à remonter.
    // for (int i = 1; i <= 15; i++) {
    //     printf("Ligne de remplissage %d...\n", i);
    // }
    
    // terminal_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    // printf("\nSi vous voyez le curseur physique clignoter juste ici -> ");
}