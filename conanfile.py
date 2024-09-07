from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout

class CommanderConan(ConanFile):
    name = 'commander'
    version = '1.0.0'
    license = "MIT"

    settings = 'os', 'compiler', 'build_type', 'arch'

    exports_sources = 'CMakeLists.txt', 'include/*', 'src/*'


    options = {
        'shared' : [True, False],
        'fPIC'   : [True, False]
    }

    default_options = {
        'shared' : False,
        'fPIC'   : True
    }

    def requirements(self):
        self.requires('cppzmq/4.10.0', transitive_headers=True)
        self.requires('emu/1.0.0', transitive_headers=True)

    def layout(self):
        cmake_layout(self)

    generators = 'CMakeToolchain', 'CMakeDeps'

    def build(self):
        cmake = CMake(self)

        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs   = ['commander']
