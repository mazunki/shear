# `shear`

Because cut doesn't support multi-char delimiters.

Compile with `cc -o bin/shear src/shear.c`.

Running: `shear [-d delimiter] [-f fields]`.
`fields` is a comma-separated list of entries.
- Entries can be numbers, open ranges, or closed ranges.
    - Ranges are expressed with a double-dot (`..`).
    - If no start of range is given, 1 is assumed.
    - If no end of range is given, size of fields is assumed.

The `delimiter` defaults to `" - "`, but any string is valid.

Flags may be passed several times, but only the last one will be accounted for.

Example
```sh
printf "hello world - abba is cool - 2024 - cool beans" |
    ./bin/shear -d " - " -f 2,..3
```

