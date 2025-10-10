from conan import ConanFile
from conan.tools.cmake import cmake_layout


class GameOfLifeConan(ConanFile):
    name = "game-of-life"
    version = "1.0"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Dependencies
    def requirements(self):
        self.requires("nlohmann_json/3.11.3")

    def build_requirements(self):
        self.test_requires("gtest/1.15.0")

    # Generators
    generators = "CMakeDeps", "CMakeToolchain"

    # This enables the modern preset-based workflow
    def layout(self):
        cmake_layout(self)

    # Configure C++20
    def configure(self):
        self.settings.compiler.cppstd = "20"