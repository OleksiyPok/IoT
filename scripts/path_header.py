# scripts/tools/path_header_cpp.py

# C++ Path Header Script
#
# Description:
# This script adds a header with the relative path to each C++ file
# inside the specified target directory.
# It removes any existing path-like header before adding the new one.
#
# By default, the script processes C++ files inside the "src" directory.
# The following directories are excluded:
# .git, build, cmake-build-debug, cmake-build-release.
#
# A lock file is used to prevent multiple instances of the script
# from running at the same time.
#
# Usage:
# python scripts/tools/path_header_cpp.py
#
# Or specify a target directory:
# python scripts/tools/path_header_cpp.py <directory>
#
# Examples:
# python scripts/tools/path_header_cpp.py
# python scripts/tools/path_header_cpp.py src
# python scripts/tools/path_header_cpp.py tests
#
# The script:
# 1. Checks that the target directory exists.
# 2. Prevents multiple instances of the script from running simultaneously.
# 3. Finds C++ source and header files inside the target directory.
# 4. Removes existing path headers.
# 5. Adds the file's relative repository path as a header.
# 6. Removes the lock file after processing.
#
# Example result:
# Before:
# #include "user.h"
#
# After:
# // src/services/user.cpp
#
# #include "user.h"

import re
import sys
from pathlib import Path

EXCLUDE_DIRS = {
    ".git",
    "build",
    "cmake-build-debug",
    "cmake-build-release",
}

LOCK_FILE = ".add_cpp_path_header.lock"

CPP_EXTENSIONS = {
    ".cpp",
    ".cc",
    ".cxx",
    ".h",
    ".hpp",
    ".hh",
    ".hxx",
}

PATH_LIKE_REGEX = re.compile(
    r"^\s*//\s*[\w\-.\/\\]+\.[\w]+$" r"|^\s*//\s*[\w\-.\/\\]+$"
)


def should_process(path: Path, target_dir: Path) -> bool:
    if path.suffix.lower() not in CPP_EXTENSIONS:
        return False

    if any(part in EXCLUDE_DIRS for part in path.parts):
        return False

    try:
        path.relative_to(target_dir)
        return True
    except ValueError:
        return False


def build_header_path(path: Path, repo_root: Path) -> str:
    return path.relative_to(repo_root).as_posix()


def process_file(file_path: Path, repo_root: Path) -> None:
    lines = file_path.read_text(encoding="utf-8-sig").splitlines(keepends=True)

    index = 0

    # Remove leading empty lines.
    while index < len(lines) and lines[index].strip() == "":
        index += 1

    # Remove existing path headers.
    while index < len(lines) and PATH_LIKE_REGEX.fullmatch(lines[index].rstrip("\r\n")):
        index += 1

        # Remove empty lines after an existing header.
        while index < len(lines) and lines[index].strip() == "":
            index += 1

    remaining_lines = lines[index:]

    header = f"// {build_header_path(file_path, repo_root)}\n" "\n"

    file_path.write_text(
        header + "".join(remaining_lines),
        encoding="utf-8",
    )


def main():
    repo_root = Path.cwd().resolve()
    lock_path = repo_root / LOCK_FILE

    target_arg = sys.argv[1] if len(sys.argv) > 1 else "src"

    target_path = (repo_root / target_arg).resolve()

    if not target_path.exists():
        print(f"Target path '{target_arg}' does not exist.")
        sys.exit(1)

    if lock_path.exists():
        print("Script already running.")
        sys.exit(1)

    processed_files = 0

    try:
        lock_path.write_text("lock")

        for cpp_file in repo_root.rglob("*"):
            if cpp_file.is_file() and should_process(cpp_file, target_path):
                process_file(cpp_file, repo_root)
                processed_files += 1

    finally:
        if lock_path.exists():
            lock_path.unlink()

    print(f"Processed {processed_files} file(s) under '{target_arg}'.")


if __name__ == "__main__":
    main()
