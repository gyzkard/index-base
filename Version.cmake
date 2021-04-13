# To update the range-v3 version, from a working directory that is clean except for an
# updated doc/release_notes.md file, update the version numbers below. This makefile will
# generate a new version.hpp, *AMEND THE MOST RECENT COMMIT*, and git-tag the commit.

set(INBA_VERSION_MAJOR 0)
set(INBA_VERSION_MINOR 1)
set(INBA_VERSION_PATCHLEVEL 0)

# prefix with - if set
set(INBA_VERSION_PRERELEASE_ID "")
# prefix with + if set
set(INBA_VERSION_BUILD_METADATA "")
