// vim:sw=8

#include <stdlib.h>
#include <sys/stat.h>

#include "i3status.h"

static bool check_mail() {
        struct stat sb;
        if (stat(getenv("MAIL"), &sb) != 0) perror("stat");
        return sb.st_size > 0;
}

void print_mail(yajl_gen json_gen, char *buffer, const char *format) {
        const char *walk;
        char *outwalk = buffer;
        const bool exists = check_mail();

        if (exists) START_COLOR("color_degraded");

        for (walk = format; *walk != '\0'; walk++) {
                if (*walk != '%')
                        *(outwalk++) = *walk;
                else if (strncmp(walk + 1, "status", strlen("status")) == 0) {
                        outwalk += sprintf(outwalk, "%s", (exists ? "yes" : "no"));
                        walk += strlen("status");
                }
        }

        if (exists) END_COLOR;
        OUTPUT_FULL_TEXT(buffer);
}
