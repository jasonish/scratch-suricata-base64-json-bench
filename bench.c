#include "suricata.h"
#include "suricata-common.h"
#include "flow.h"
#include "detect.h"
#include "rust-context.h"
#include "rust-bindings.h"
#include "util-crypt.h"
#include <sys/time.h>

static int inner = 100;

void rust_bench(uint8_t *buf, size_t len, int count)
{
    struct timeval start, end, diff;
    gettimeofday(&start, NULL);
    for (int i = 0; i < count; i++) {
        JsonBuilder *jb = jb_new_array();
        assert(jb != NULL);

        for (int j = 0; j < inner; j++) {
            jb_append_base64(jb, buf, len);
        }
        assert(jb_len(jb) == 1092700);
        jb_free(jb);
    }
    gettimeofday(&end, NULL);
    timersub(&end, &start, &diff);
    printf("Rust: %ld.%06ld\n", diff.tv_sec, diff.tv_usec);
}

void c_bench(uint8_t *buf, size_t len, int count)
{
    struct timeval start, end, diff;
    gettimeofday(&start, NULL);
    for (int i = 0; i < count; i++) {
        JsonBuilder *jb = jb_new_array();
        assert(jb != NULL);

        for (int j = 0; j < inner; j++) {
            unsigned long olen = BASE64_BUFFER_SIZE(len);
            uint8_t encoded[olen];
            assert(Base64Encode(buf, len, encoded, &olen) == SC_BASE64_OK);
            jb_append_string_safe(jb, (char *)encoded);
        }
        assert(jb_len(jb) == 1092700);
        jb_free(jb);
    }
    gettimeofday(&end, NULL);
    timersub(&end, &start, &diff);
    printf("C:   %ld.%06ld\n", diff.tv_sec, diff.tv_usec);
}

int main(int argc, char **argv)
{
    /* Create the buffer to be encoded. */
    uint8_t buf[8192];
    for (int i = 0; i < sizeof(buf); i++) {
        buf[i] = i % 0xff;
    }

    int count = 5000;

    rust_bench(buf, sizeof(buf), count);
    c_bench(buf, sizeof(buf), count);
    rust_bench(buf, sizeof(buf), count);
    c_bench(buf, sizeof(buf), count);
    rust_bench(buf, sizeof(buf), count);
    c_bench(buf, sizeof(buf), count);

    return 0;
}
