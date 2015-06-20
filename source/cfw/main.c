#include <stdint.h>
#include <draw.h>
#include "fs.h"
#include "menu.h"
#include "firm.h"
#include "patch.h"
#include "hid.h"
#include "fatfs/ff.h"

// TODO: Put this in a nice place
#define MENU_MAIN_VALID_MAX (int)2

void menu_main(void)
{
    char *options[] = { "Boot CFW with sig patches, emunand and reboot",
                        "Boot CFW with sig patches and emunand",
                        "Boot CFW with sig patches"};
    char boot_option = -1;
    while (1) {
        boot_option = draw_menu("Main menu", 0, sizeof(options) / sizeof(char *), options);
        if (boot_option <= MENU_MAIN_VALID_MAX) {
            //Save it then boot. I don't wanna know if it is saved really.
            //Also the filename could be defined as a macro.
            write_file(&boot_option, "0:cakefw.config", 1);
            boot_cfw(boot_option);
        }
    }
}

void main(void)
{
    char boot_option = 0;   // Currently supports 8 for a char. Could be expanded to an array or splited.
    draw_init((uint32_t *)0x23FFFE00);
    mount_sd();
    //In fact it should return FR_NO_FILE if not such a file.
    if (read_file_offset(&boot_option, "0:cakefw.config", 1, 0) != FR_OK) {
        menu_main();
    } else {
        if (boot_option <= MENU_MAIN_VALID_MAX) {
            // Think it as valid.
            if (*hid_regs) {
                // Any key is still held when started.
                menu_main();
            } else {
                boot_cfw(boot_option);
            }
        }
    }
    
}
