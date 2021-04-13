#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from conans import ConanFile, CMake


class InbaConan(ConanFile):
    name = "inba"
    version = "0.1"
    license = "MIT License"
    url = "https://github.com/gyzkard/index-base"
    author = "gyzkard (gyzkard@gyzkard.com)"
    description = "Strong types to distinguish between zero, one or maybe even other numbers based indexes."
    homepage = "https://github.com/gyzkard/index-base"
    no_copy_source = True
    # note: the settings are only used for the unit tests and should not lead to a different package id on different platforms; see https://stackoverflow.com/questions/67062596/how-to-create-a-conan-package-including-tests-for-a-header-only-library-which-ca
    settings = "os", "compiler", "build_type", "arch"
    scm = {
        "type": "git",
        "url": "auto",
        "revision": "auto"
    }
    
	# build and execute unit tests; the binaries won't be part of the package
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        self.copy("LICENSE", "licenses")
        self.copy("*.hpp")
    
    def package_id(self):
        self.info.header_only()

