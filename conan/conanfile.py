from conans import ConanFile, CMake, tools

from os import path


class ShmurpConan(ConanFile):
    name = "shmurp"
    version = "0.0.0"
    license = "The Unlicense"
    author = "adnn"
    url = "https://github.com/Adnn/shmurp"
    description = "Shoot them urp!"
    topics = ("opengl", "2D", "game")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {
        "shared": False,
    }

    requires = (
        ("glad/0.1.34"),

        ("aunteater/local"),
        ("graphics/local"),
        ("math/local"),
        ("websocket/local"),
    )

    build_requires = ("cmake/3.20.4")

    generators = "cmake_paths", "cmake", "cmake_find_package"
    build_policy = "missing"

    scm = {
        "type": "git",
        "url": "auto",
        "revision": "auto",
        "submodule": "recursive",
    }


    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions["CMAKE_PROJECT_Shmurp_INCLUDE"] = \
            path.join(self.source_folder, "cmake", "conan", "customconan.cmake")
        cmake.configure()
        return cmake


    def build(self):
        cmake = self._configure_cmake()
        cmake.build()


    def package(self):
        cmake = self._configure_cmake()
        cmake.install()
