char test_string[] = "This is a test string. It will be dumped on stdout.";

#define HEXD_BYTE       (0x01u)
#define HEXD_WORD       (0x02u)
#define HEXD_QWORD      (0x03u)
#define HEXD_LSBFIRST   (0x00u)
#define HEXD_MSBFIRST   (0x80u)

#define HEXD_BUFLEN     (256)

static uint16_t hex_to_asc(uint8_t v)
{
    uint8_t t;
    uint16_t r;

    t = v & 0x0fu;
    ((uint8_t* )&r)[1] = (t > 9u) ? (t + ('A' - 10u)) : (t + '0');   // LS byte
    t = v >> 4;
    ((uint8_t* )&r)[0] = (t > 9u) ? (t + ('A' - 10u)) : (t + '0');   // MS byte

    return r;
}

static void append_ascii(char* buf, const char* tail)
{
    strcat(buf,"|  ");
    strcat(buf,tail);
    strcat(buf," \n");
}

static void printf_line(const char* line)
{
    printf("%s",line);
}

typedef void (*output_line)(const char* line);

// this buffer is big enough to contain full line of 16 number and their ASCII representation
static char strBuffer[HEXD_BUFLEN];

void dump_hex(const void* data, size_t size, output_line func) {
    char ascii[17];
    size_t i, j, sx;
    ascii[16] = '\0';
    sx = 0;
    memset(strBuffer,'\0',sizeof(strBuffer));
    for (i = 0; i < size; ++i) {
        uint16_t hdig = hex_to_asc(((unsigned char*)data)[i]);
        strBuffer[sx++] = ((uint8_t* )&hdig)[0];
        strBuffer[sx++] = ((uint8_t* )&hdig)[1];
        strBuffer[sx++] = ' ';

        if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= 255) { // '~'
            ascii[i % 16] = ((unsigned char*)data)[i];
        } else {
            ascii[i % 16] = '.';
        }
        if ((i+1) % 8 == 0 || i+1 == size) {
            strBuffer[sx++] = ' ';
            if ((i+1) % 16 == 0) {
                goto output;
            } else if (i+1 == size) {
                ascii[(i+1) % 16] = '\0';
                if ((i+1) % 16 <= 8) {
                    strBuffer[sx++] = ' ';
                }
                for (j = (i+1) % 16; j < 16; ++j) {
                    strBuffer[sx++] = ' ';
                    strBuffer[sx++] = ' ';
                    strBuffer[sx++] = ' ';
                }
output:
                append_ascii(strBuffer,ascii);
                func(strBuffer);
                memset(strBuffer,'\0',sizeof(strBuffer));
                sx = 0;
            }
        }
    }
}

int test_hexdump(void)
{
    dump_hex(test_string,sizeof(test_string),printf_line);

    return 0;
}
