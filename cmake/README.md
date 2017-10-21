# `bit::cmake`

[![GitHub License](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/bitwizeshift/bit-stl/master/LICENSE.md)

## What is `bit::cmake`?

This is a small repo that contains some cmake build modules and toolchains to facilitate easier script writing.
These are used in various personal projects to promote writing c++ that follows proper coding standards

## Modules

There are various modules available, as described below. For more information, refer to the documentation
of each specific module. Documentation is in ReStructured Text format.

### `AddIndependenceCheck.cmake`

This adds the cmake command `add_independence_check` which creates a custom library that generates a list of
source files that include each header individually.
The purpose of this is to ensure that each header in a public API requires no header-ordering to properly operate.

### `AddTestExecutable.cmake`

This adds the cmake command `add_test_executable`. This is a small wrapper around `add_executable` that immediately
invokes the built binary as part of the build step. This allows chaining dependencies, and breaking builds on unit
test failures.

### `CopyTargetProperties.cmake`

This adds the cmake command `copy_target_properties`. This allows copying values of each property from one target
to another. This is especially useful when trying to image all arguments from a regular library onto an object
library

### `CopyTarget.cmake`

This adds the cmake command `copy_target`. This copies each of the basic properties for that given visibility.
For example, `INTERFACE` will copy all the `INTERFACE_*` properties, `PRIVATE` will copy the normal properties,
and `PUBLIC` will copy both.

### `GitBuildInformation.cmake`

This adds the cmake command `git_build_information`. This provides an easy way of automatically incrementing
a minor and patch number based on the revision of the 'major' release.

Though this is not as exact as a build number from a CI or one generated through SVN, it provides a decent
amount of uniqueness that can then be translated into a version header (see below)

### `MakeVersionHeader.cmake`

This adds the cmake command `make_version_header`. This generates an easy version header when given the
major, minor, patch, and build versions of the current library. Optionally a tag (e.g. branch name) can
be provided to given a unique version string identifier.

This is useful in conjunction with `get_build_information`, since all of these values can easily be
extracted from it.
