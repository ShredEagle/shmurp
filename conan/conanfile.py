from conans import ConanFile, CMake, tools


class ShpongConan(ConanFile):
    name = "shmurp"
    version = "0.0.0"
    license = "The Unlicense"
    author = "adnn"
    url = "https://github.com/Adnn/shmurp"
    description = "Shoot them urp!"
    topics = ("opengl", "2D", "Game")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    generators = "cmake_paths", "cmake"
    build_policy = "missing"

    requires = (
        ("glad/0.1.29@bincrafters/stable"),
        ("boost/1.71.0@conan/stable"),
        #Aunteater
        #Graphics
        #Math
        #Websockets
    )

    default_options = {
        "shared": False,
    }
