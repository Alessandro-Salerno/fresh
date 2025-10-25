<p align="center">
    <h1 align="center">fresh</h1>
    <p align="center"> The freestanding shell toolkit </p>
</p>

<div align="center">

[contributors-shield]: https://img.shields.io/github/contributors/Alessandro-Salerno/fresh.svg?style=flat-square
[contributors-url]: https://github.com/Alessandro-Salerno/fresh/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/Alessandro-Salerno/fresh.svg?style=flat-square
[forks-url]: https://github.com/Alessandro-Salerno/fresh/network/members
[stars-shield]: https://img.shields.io/github/stars/Alessandro-Salerno/fresh.svg?style=flat-square
[stars-url]: https://github.com/Alessandro-Salerno/fresh/stargazers
[issues-shield]: https://img.shields.io/github/issues/Alessandro-Salerno/fresh.svg?style=flat-square
[issues-url]: https://github.com/Alessandro-Salerno/fresh/issues
[license-shield]: https://img.shields.io/github/license/Alessandro-Salerno/fresh.svg?style=flat-square
[license-url]: https://github.com/Alessandro-Salerno/fresh/blob/master/LICENSE.txt

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
![](https://tokei.rs/b1/github/Alessandro-Salerno/fresh)

</div>


## What is fresh?
`fresh` is a collection of self-contained (freestanding) programs to test basic program loading and I/O in Unix-like hobby operating systems initially developed as a userland for the [SalernOS Kernel](https://github.com/Alessandro-Salerno/SalernOS-Kernel), and later replaced by GNU with [mlibc](https://github.com/managarm/mlibc).

This repository includes some basic shell utilities (command interpreter, cat, echo), the scripts needed to build them, and a freestanding implementation of a subset of C Standard Library functions shared by all subprojects.

`fresh` _should_ be quite easy to port to other OSes, see `sysdeps/salernos-x86_64`.
> [!WARNING]
> `fresh` is not meant to be a replacement for existing userland suites. Do not use it as such!

## License
`fresh`, and all other "original" subprojects are distributed under the MIT license. See LICENSE for details.

This repository, however, contains several components which may be redistributions of free and open source software authored by third parties. Credit and licenses for these modules are included in the relevant source files.
