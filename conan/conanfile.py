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
        #("glfw/3.3@bincrafters/stable"),
    )

    default_options = {
        "shared": False,
    }
