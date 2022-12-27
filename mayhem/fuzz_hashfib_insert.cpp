#include <stdint.h>
#include <stdio.h>
#include <climits>

#include <fuzzer/FuzzedDataProvider.h>

extern "C" {
    #include "hashfib.h"
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    FuzzedDataProvider provider(data, size);
    int key = provider.ConsumeIntegral<int>();
    HashFib* hf = hashfib_construct();

    hashfib_insert(hf, key);

    hashfib_destroy(hf);

    return 0;
}
