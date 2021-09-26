# Daisy Generator example help

To build these test patches:

Install `arm-none-eabi-gcc` on your respective OS.

Then convert one of the examples to daisy:
`hvcc seed_test.pd -m seed_meta.json -n seed_test -g daisy -o .`

Then pull in, and compile, libdaisy:

```bash
git clone https://github.com/electro-smith/libDaisy.git libdaisy
cd libdaisy
make
```

When it's done building, from the examples dir:

```bash
cd daisy/source/
make
```

And if your daisy is in dfu mode, then:

```bash
make program-dfu
```

This should result in a `build/` dir with some artifacts:

```ls
-rw-r--r-- 1 dreamer dreamer 3.5K Jul 15 19:35 HeavyDaisy_seed_test.d
-rw-r--r-- 1 dreamer dreamer 9.9K Jul 15 19:35 HeavyDaisy_seed_test.o
-rw-r--r-- 1 dreamer dreamer  42K Jul 15 19:35 HeavyDaisy_seed_test.lst
-rw-r--r-- 1 dreamer dreamer 957K Jul 15 19:35 HeavyDaisy_seed_test.map
-rwxr-xr-x 1 dreamer dreamer 1.2M Jul 15 19:35 HeavyDaisy_seed_test.elf
-rw-r--r-- 1 dreamer dreamer 171K Jul 15 19:35 HeavyDaisy_seed_test.hex
-rwxr-xr-x 1 dreamer dreamer  61K Jul 15 19:35 HeavyDaisy_seed_test.bin
```
