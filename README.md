
## What is this?
`fresh` is a collection of self-contained (freestanding) programs to test basic program loading and I/O in Unix-like hobby operating systems initially developed as a userland for the [SalernOS Kernel](https://github.com/Alessandro-Salerno/SalernOS-Kernel), and later replaced by GNU and [mlibc](https://github.com/managarm/mlibc).

This repository includes some basic shell utilities (command interpreter, cat, echo), the scripts needed to build them, and a freestanding implementation of a subset of C Standard Library functions shared by all subprojects.

`fresh` _should_ be quite easy to port to other OSes, see `sysdeps/salernos-x86_64`.
> [!WARNING]
> `fresh` is not meant to be a replacement for existing userland suites. Do not use it as such!

## License
`fresh`, and all other "original" subprojects are distributed under the MIT license. See LICENSE for details.

This repository, however, contains several components which may be redistributions of free and open source software authored by third parties. Credit and licenses for these modules are included in the relevant source files.
