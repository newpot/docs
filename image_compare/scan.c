
/*
 * Copyright (c) 2022, Pensando Systems Inc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

static uint32_t l_start, l_end;
static int l_c, l_valid;

static void
push(int c, uint32_t start, uint32_t end)
{
    uint32_t len = end - start + 1;

    if (len >= 128) {
        if (l_valid) {
            printf("%c: 0x%08x...0x%08x (%8u)\n",
                    l_c, l_start, l_end, l_end - l_start + 1);
            l_valid = 0;
        }
        printf("%c: 0x%08x...0x%08x (%8u)\n",
                c, start, end, end - start + 1);
    } else {
        if (!l_valid) {
            l_valid = 1;
            l_start = start;
            l_end = end;
            l_c = c;
        } else {
            l_end = end;
        }
    }
}

static void
scan_filtered(FILE *gfp, FILE *bfp)
{
    uint32_t r_start;
    uint32_t offs;
    int state;
    int gc, bc;

    state = 0;
    r_start = 0;
    for (offs = 0;; offs++) {
        gc = fgetc(gfp);
        bc = fgetc(bfp);
        if (gc == EOF || bc == EOF) {
            break;
        }
        switch (state) {
        case 0:
            if (gc != bc) {
                push('G', r_start, offs - 1);
                r_start = offs;
                state = 1;
            }
            break;

        case 1:
            if (gc == bc) {
                push('B', r_start, offs - 1);
                r_start = offs;
                state = 0;
            }
            break;
        }
    }
    push((state == 0) ? 'G' : 'B', r_start, offs - 1);
}

static void
scan_simple(FILE *gfp, FILE *bfp)
{
    uint32_t r_start;
    char descr[40];
    uint32_t offs;
    int state;
    int gc, bc;
    char *s;

    state = 0;
    r_start = 0;
    for (offs = 0;; offs++) {
        gc = fgetc(gfp);
        bc = fgetc(bfp);
        if (gc == EOF || bc == EOF) {
            break;
        }
        switch (state) {
        case 0:
            if (gc != bc) {
                printf("G: 0x%08x...0x%08x (%8u)\n",
                        r_start, offs - 1, offs - r_start);
                s = descr;
                s += sprintf(descr, "%02x", bc);
                r_start = offs;
                state = 1;
            }
            break;

        case 1:
            if (gc == bc) {
                printf("B: 0x%08x...0x%08x (%8u) %s\n",
                        r_start, offs - 1, offs - r_start, descr);
                r_start = offs;
                state = 0;
            } else {
                if (s < descr + sizeof (descr) - 4) {
                    s += sprintf(s, " %02x", bc);
                }
            }
            break;
        }
    }
    printf("%c: 0x%08x...0x%08x (%8u)\n", (state == 0) ? 'G' : 'B',
            r_start, offs - 1, offs - r_start);
}

int
main(int argc, char *argv[])
{
    FILE *gfp, *bfp;

    gfp = fopen("kernel.img", "r");
    bfp = fopen("corrupt.img", "r");
    if (gfp == NULL || bfp == NULL) {
        fprintf(stderr, "No.\n");
        return 1;
    }
    if (0) {
        scan_simple(gfp, bfp);
    } else {
        scan_filtered(gfp, bfp);
    }
    return 0;
}
