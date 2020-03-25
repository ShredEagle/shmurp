# Shmurp

A game where you have to shoot them urp

## Development

The following procedure requires :

* The platform compiler (MSVC, Clang, GCC)
* [Conan](https://conan.io/)
* CMake (>= 3.16)
* [Anyrepo](https://github.com/Adnn/anyrepo) helper

`Shmurp` is currently developed alongside related modules, all configured in a development folder.

    mkdir $DEV_FOLDER && cd $DEV_FOLDER
    git clone --recursive git@github.com:Adnn/shmurp.git
    $ANYREPO_FOLDER/setupdev.py shmurp/conan/anyrepo_development.json
