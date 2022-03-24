# Commander

Commander is a command line tool interface library.

## Installation

Commander need some dependencies to be installed. You can easily install them by using conan package manager.

- [nlohmann_json 3.10](https://github.com/nlohmann/json/releases/tag/v3.10.5)
- [cppzmq 4.8](https://github.com/zeromq/cppzmq/releases/tag/v4.8.1)
- [boost 1.78](https://www.boost.org/users/history/version_1_78_0.html)
- [fmt 8.1](https://github.com/fmtlib/fmt/releases/tag/8.1.1)

### Using Conan package manager.

You can install conan using `pip`.

```bash
python -m pip install conan --user
```

Then you need to configure the conan profile:

This command will create a profile named `default` that list information about OS and compiler among other things.

```bash
tools/configure_conan.sh
```

The script will adds `cosmic` repository to conan. which is not available in conancenter.

It will also configure the conan profile based on compiler and platform.

### Install Commander manually

Run:

```bash
./configure.sh
./build.sh
```

`configure.sh` will download and build dependencies if needed into your local conan cache and generate cmake configuration files.

## Package Commander into the local conan cache.

Run:

```bash
conan create .
```

It will create `commander` package in your local conan cache.