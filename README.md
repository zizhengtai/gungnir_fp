# Gungnir

[![Build Status](https://travis-ci.org/ZizhengTai/gungnir.svg?branch=master)](https://travis-ci.org/ZizhengTai/gungnir)
[![License](https://img.shields.io/badge/license-Apache_2.0-blue.svg)](LICENSE)
[![Join the chat at https://gitter.im/ZizhengTai/gungnir](https://badges.gitter.im/ZizhengTai/gungnir.svg)](https://gitter.im/ZizhengTai/gungnir?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Gungnir is a library that provides abstractions for functional programming in C++. It is made with ease of use, efficiency, and full compatibility with the C++11 standard in mind.

## Data Structures

Gungnir provides a collection of data structures to facilitate functional programming in C++, most of which are immutable functional data structures. Their implementations are highly optimized, utilizing techniques including [structural sharing](https://en.wikipedia.org/wiki/Persistent_data_structure) and [lazy evaluation](https://en.wikipedia.org/wiki/Lazy_evaluation), so you should not hold back from using them for performance considerations.

Some of the more frequently used data structures include:

* [`List`](include/gungnir/List.hpp)
* `Vector`
* `Stream`
* `Iterator`

## License

This project is licensed under the Apache License, Version 2.0. See the [LICENSE](LICENSE) file for details.
