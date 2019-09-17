/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief String table and text obfuscation functions.
 *
 * @copyright Horizon is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "dipthong.h"
#include "gamedebug.h"
#include <string.h>

#define DIP_EX_ASCII_FLAG 27 // 0x1B
#define DIP_REG_ASCII_MAX 127 // 0x7F
#define DIP_EX_ASCII_BIT 128 // 0x80

namespace
{
// These tables are used for the dipthong algorithm
const char Common[] = " etainosrlhcdupm";
const char Dipthong[][9] = { "tasio wb",
    " rnsdalm",
    "h ieoras",
    "nrtlc sy",
    "nstcloer",
    " dtgesio",
    "nr ufmsw",
    " tep.ica",
    "e oiadur",
    " laeiyod",
    "eia otru",
    "etoakhlr",
    " eiu,.oa",
    "nsrctlai",
    "leoiratp",
    "eaoip bm" };

} // namespace

/**
 * @brief Decodes extended ASCII values from 0x1B 0xXX pairs
 * @warning Length of the destination is not checked, overruns possible.
 */
void Fixup_Text(const char *input, char *output)
{
    if (input == nullptr || output == nullptr) {
        return;
    }

    while (*input != '\0') {
        char currentbyte = *input++;

        if (currentbyte == DIP_EX_ASCII_FLAG) {
            currentbyte = DIP_REG_ASCII_MAX + (unsigned char)(*input++);
        }

        *output++ = currentbyte;
    }
    *output = '\0';
}

/**
 * @brief Encodes string with "Dipthong" algorithm and encodes extended ASCII values to a regular ASCII multi char.
 * @warning Length of the destination is not checked, overruns possible.
 */
int Dip_Text(const char *input, char *output)
{
    //  Check for valid Input and Dest.
    if (input == nullptr || output == nullptr) {
        return 0;
    }

    // Initialise length counter.
    int length = 0;

    for (const char *string = input; *string != '\0'; ++string) {
        char currentbyte = *string;
        char nextbyte = *(string + 1);

        if (currentbyte > 0) {
            // Iterate through Common table to find match to current byte if any
            for (int diptable = 0; diptable < 16; ++diptable) {
                char commonbyte = Common[diptable];

                // If we have a match, xor them and look for match in Dipthong table
                if (commonbyte == currentbyte) {
                    char xorchar = currentbyte ^ commonbyte;

                    for (int dipchar = 0; dipchar < 8; ++dipchar) {
                        // If we have a match, encode both bytes such that we can
                        // recover them from the tables later
                        if (Dipthong[diptable][dipchar] == nextbyte) {
                            ++string;
                            currentbyte = xorchar | (diptable << 3) | DIP_EX_ASCII_BIT;
                        }

                        ++xorchar;
                    }
                }
            }
            // This bit is the reverse of Fixup_Text, that func should perhaps be folded into UnDip_Text
        } else {
            output[length++] = DIP_EX_ASCII_FLAG;
            currentbyte = (unsigned char)currentbyte - DIP_REG_ASCII_MAX;
        }

        output[length++] = currentbyte;
    }

    output[length] = '\0';

    return length;
}

/**
 * @brief Decodes string encoded with "Dipthong" algorithm.
 * @warning Length of the destination is not checked, overruns possible.
 */
int UnDip_Text(const char *input, char *output)
{
    //  Check for valid Input and Dest.
    if (input == nullptr || output == nullptr) {
        return (0);
    }

    //  Initialise the length counter.
    int count = 0; // loop counter

    for (const char *string = input; *string != '\0'; ++string) {
        char currentbyte = *string;

        if ((currentbyte & DIP_EX_ASCII_BIT) != 0) {
            currentbyte &= DIP_REG_ASCII_MAX;
            output[count++] = Common[currentbyte >> 3];
            currentbyte = Dipthong[currentbyte >> 3][currentbyte & 0x07];
        }

        output[count++] = currentbyte;
    }

    output[count] = '\0';

    return count;
}

/**
 * @brief Recover a string from the string table based on its ID.
 */
const char *Extract_String(char *file, int str_id)
{
    if (file != nullptr && str_id >= 0) {
        return reinterpret_cast<uint16_t *>(file)[str_id] + file;
    }
    return nullptr;
}
