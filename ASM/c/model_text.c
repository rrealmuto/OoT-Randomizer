#include "model_text.h"
#include <stdbool.h>

uint16_t illegal_model = 0;

void draw_illegal_model_text(z64_disp_buf_t *db) {
    // Only draw when paused
    if (!(illegal_model && z64_game.pause_ctxt.state == 6)) {
        return;
    }

    // Setup draw location
    int str_len = 38;
    int total_w = str_len * font_sprite.tile_w;
    int draw_x = Z64_SCREEN_WIDTH / 2 - total_w / 2;
    int draw_y_text = 5;

    // Create collected/required string
    char text[str_len + 1];
    strncpy(text, "Race advisory:irregular model skeleton\0", str_len + 1);

    // Call setup display list
    gSPDisplayList(db->p++, &setup_db);
    gDPPipeSync(db->p++);
    gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);

    text_print(text, draw_x, draw_y_text);

    text_flush(db);
    gDPFullSync(db->p++);
    gSPEndDisplayList(db->p++);
}

typedef struct Limb {
    int x;
    int y;
    int z;
} Limb;

const int endianVal = 1;
uint32_t EndianSwap32(uint32_t val) {
    bool smallEndian = ((*(char *)&endianVal) != 0);
    if (smallEndian) {
        uint32_t new_val = 0;
        for (int i = 0; i < 4; i++) {
            new_val <<= 8;
            new_val |= (val & 0x00FF);
            val = val >> 8;
        }
        return new_val;
    }
    return val;
}

uint32_t EndianSwap16(uint16_t val) {
    bool smallEndian = ((*(char *)&endianVal) != 0);
    if (smallEndian) {
        uint16_t new_val = 0;
        for (int i = 0; i < 2; i++) {
            new_val <<= 8;
            new_val |= (val & 0x00FF);
            val = val >> 8;
        }
        return new_val;
    }
    return val;
}

// bool check_skeleton(int hierarchy, int address, Limb* skeleton) {
//   // Get what the hierarchy pointer points to (pointer to limb 0)
//   int limbPointer = EndianSwap32(*(int*)(rom + address + hierarchy)) & 0x00FFFFFF;
//   // Get the limb this points to
//   int limb = EndianSwap32(*(int*)(rom + address + limbPointer)) & 0x00FFFFFF;
//   // Go through each limb in the table
//   bool hasVanillaSkeleton = true;
//   for (int i = 0; i < 21; i++) {
//     int offset = limb + i * 0x10;
//     // X, Y, Z components are 2 bytes each
//     int limbX = EndianSwap16(*(uint16_t*)(rom + address + offset));
//     int limbY = EndianSwap16(*(uint16_t*)(rom + address + offset + 2));
//     int limbZ = EndianSwap16(*(uint16_t*)(rom + address + offset + 4));
//     int skeletonX = skeleton[i].x;
//     int skeletonY = skeleton[i].y;
//     int skeletonZ = skeleton[i].z;
//     // Check if the X, Y, and Z components all match
//     if (limbX != skeletonX || limbY != skeletonY || limbZ != skeletonZ) {
//       return false;
//     }
//   }
//   return true;
// }

// // Finds the address of the model's hierarchy so we can write the hierarchy pointer
// // Based on
// // https://github.com/hylian-modding/Z64Online/blob/master/src/Z64Online/common/cosmetics/UniversalAliasTable.ts
// // function findHierarchy()
// int FindHierarchy(uint8_t* zobj, int zobjlen, const char* agestr) {
//   // Scan until we find a segmented pointer which is 0x0C or 0x10 more than
//   // the preceeding data and loop until something that's not a segmented pointer is found
//   // then return the position of the last segemented pointer.
//   for (int i = 0; i < zobjlen; i += 4) {
//     if (zobj[i] == 0x06) {

//       int possible = EndianSwap32(*(int*)(zobj + i)) & 0x00FFFFFF;

//       if (possible < zobjlen) {
//         int possible2 = EndianSwap32(*(int*)(zobj + i - 4)) & 0x00FFFFFF;
//         int diff = possible - possible2;
//         if ((diff == 0x0C) || (diff == 0x10)) {
//           int pos = i + 4;
//           int count = 1;
//           while (zobj[pos] == 0x06) {
//             pos += 4;
//             count += 1;
//           }
//           uint8_t a = zobj[pos];
//           if (a != count) {
//             continue;
//           }

//           // printf("[C]FindHierarchy returns pos: %i, possible1: %X, possible2: %X\n", pos - 4,
//           // possible, possible2);
//           return pos - 4;
//         }
//       }
//     }
//   }

// void check_model_skeletons() {
//   // Get the hierarchy pointer for child and adult
//   //Not sure how best to do this part, maybe the hierarchy address can be stored somewhere?
//   //int childHierarchy = FindHierarchy(zobj, zobjlen, agestr);
//   //int adultHierarchy = FindHierarchy(zobj, zobjlen, agestr);

//   bool childIsVanilla = check_skeleton(childHierarchy, childRomAddress, childSkeleton);
//   bool adultIsVanilla = check_skeleton(adultHierarchy, adultRomAddress, adultSkeleton);

//   if (!childIsVanilla || !adultIsVanilla) {
//     illegal_model = 1;
//   }
// }
