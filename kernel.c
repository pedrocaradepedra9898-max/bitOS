/* ASSINATURA DO CRIADOR... CARADEPEDRA9898*/

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define WHITE_ON_BLACK 0x0F

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

unsigned short* vga_buffer = (unsigned short*)VGA_ADDRESS;
int cursor_x = 0;
int cursor_y = 0;


static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}


void clear_screen(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = (WHITE_ON_BLACK << 8) | ' ';
    }
    cursor_x = 0;
    cursor_y = 0;
}

void putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
            vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = (WHITE_ON_BLACK << 8) | ' ';
        }
    } else {
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = (WHITE_ON_BLACK << 8) | c;
        cursor_x++;
    }

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= VGA_HEIGHT) {
        clear_screen();
    }
}

void print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        putchar(str[i]);
    }
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}


char scancode_to_char(unsigned char scancode) {
    static const char ascii[] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
        '*', 0, ' '
    };
    if (scancode < sizeof(ascii)) {
        return ascii[scancode];
    }
    return 0;
}

char read_char(void) {
    while (1) {
        if (inb(KEYBOARD_STATUS_PORT) & 1) {
            unsigned char scancode = inb(KEYBOARD_DATA_PORT);
            
            if (!(scancode & 0x80)) {
                char c = scancode_to_char(scancode);
                if (c != 0) return c;
            }
        }
    }
}


void execute_command(const char* cmd) {
    if (strcmp(cmd, "clear") == 0) {
        clear_screen();
    } 
    else if (strcmp(cmd, "helpbitos") == 0) {
        print("Comandos disponiveis do BitOS:\n");
        print("  clear      - Limpa a tela\n");
        print("  helpbitos  - Exibe esta mensagem de ajuda\n");
        print("  nothing    - Exibe uma curiosidade\n");
        print("  version    - Mostra a versao e a logo do BitOS\n");
    } 
    else if (strcmp(cmd, "nothing") == 0) {
        print("voce sabia que da pra colocar comandos aqui ne?\n");
        print("Did you know you can put commands here, right?\n");
    } 
    else if (strcmp(cmd, "version") == 0) {
        print("bitos(pode mudar esse nome se quiser)\n");
        print("seu mundo livre esta aqui\n\n");

        
                                                                  
         print("  BITOS BITOS BITOS BITOS   \n");                                   
         print(" apenas faz o que voce sabe fazer  \n");                                   
         print(" e e livre! \n");                                   
         print(" seu sistema open source \n");                                   
         print(" BITOS BITOS BITOS BITOS   \n");                                   
        
    } 
    else if (cmd[0] != '\0') {
        print("Comando nao reconhecido: ");
        print(cmd);
        print("\nDigite 'helpbitos' para ver os comandos disponiveis.\n");
    }
}


void kernel_main(void) {
    clear_screen();
    print("===========================================\n");
    print("       Bem-vindo ao bitos - open source       \n");
    print("   Digite 'helpbitos' para ver os comandos \n");
    print("===========================================\n\n");

    char buffer[128];
    int index = 0;

    while (1) {
        print("BitOS> ");
        index = 0;

        while (1) {
            char c = read_char();

            if (c == '\n') {
                putchar('\n');
                buffer[index] = '\0';
                break;
            } else if (c == '\b') {
                if (index > 0) {
                    index--;
                    putchar('\b');
                }
            } else {
                if (index < 127) {
                    buffer[index++] = c;
                    putchar(c);
                }
            }
        }

        execute_command(buffer);
    }
}