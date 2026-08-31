# scripts/tools/path_header.py

# Path Header Script
#
# Description:
# This script adds a header with the relative path to source files
# inside the specified target directory.
#
# Supported file types:
#   Python:
#       .py  -> # path/to/file.py
#
#   C++:
#       .cpp, .cc, .cxx, .h, .hpp, .hh, .hxx
#       -> // path/to/file.cpp
#
# The script removes any existing path-like header before adding
# the new one.
#
# By default, the script processes files inside the "app" directory.
#
# Excluded directories:
# .venv, __pycache__, .pytest_cache, .git,
# build, cmake-build-debug, cmake-build-release
#
# Usage:
#   python scripts/tools/path_header.py
#   python scripts/tools/path_header.py app
#   python scripts/tools/path_header.py src
#   python scripts/tools/path_header.py tests


import re
import sys
from pathlib import Path

EXCLUDE_DIRS = {
    ".venv",
    "__pycache__",
    ".pytest_cache",
    ".git",
    "build",
    "cmake-build-debug",
    "cmake-build-release",
}

LOCK_FILE = ".add_path_header.lock"


LANGUAGES = {
    "python": {
        "extensions": {".py"},
        "comment": "#",
    },
    "cpp": {
        "extensions": {
            ".cpp",
            ".cc",
            ".cxx",
            ".h",
            ".hpp",
            ".hh",
            ".hxx",
        },
        "comment": "//",
    },
}


PATH_LIKE_REGEX = {
    "#": re.compile(r"^\s*#\s*[\w\-.\/\\]+\.[\w]+$" r"|^\s*#\s*[\w\-.\/\\]+$"),
    "//": re.compile(r"^\s*//\s*[\w\-.\/\\]+\.[\w]+$" r"|^\s*//\s*[\w\-.\/\\]+$"),
}


def get_language(path: Path) -> dict | None:
    extension = path.suffix.lower()

    for language in LANGUAGES.values():
        if extension in language["extensions"]:
            return language

    return None


def should_process(path: Path, target_dir: Path) -> bool:
    if not path.is_file():
        return False

    if any(part in EXCLUDE_DIRS for part in path.parts):
        return False

    if get_language(path) is None:
        return False

    try:
        path.relative_to(target_dir)
        return True
    except ValueError:
        return False


def build_header_path(path: Path, repo_root: Path) -> str:
    return path.relative_to(repo_root).as_posix()


def remove_existing_header(
    lines: list[str],
    comment: str,
) -> list[str]:

    path_regex = PATH_LIKE_REGEX[comment]

    index = 0

    # Remove leading empty lines.
    while index < len(lines) and lines[index].strip() == "":
        index += 1

    # Remove existing path headers.
    while index < len(lines) and path_regex.fullmatch(lines[index].rstrip("\r\n")):
        index += 1

        # Remove empty lines after the header.
        while index < len(lines) and lines[index].strip() == "":
            index += 1

    return lines[index:]


def process_file(file_path: Path, repo_root: Path) -> None:
    language = get_language(file_path)

    if language is None:
        return

    comment = language["comment"]

    lines = file_path.read_text(encoding="utf-8-sig").splitlines(keepends=True)

    remaining_lines = remove_existing_header(
        lines,
        comment,
    )

    header = f"{comment} " f"{build_header_path(file_path, repo_root)}\n" "\n"

    file_path.write_text(
        header + "".join(remaining_lines),
        encoding="utf-8",
    )


def main():
    repo_root = Path.cwd().resolve()
    lock_path = repo_root / LOCK_FILE

    target_arg = sys.argv[1] if len(sys.argv) > 1 else "app"
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

        for file_path in repo_root.rglob("*"):
            if should_process(file_path, target_path):
                process_file(file_path, repo_root)
                processed_files += 1

    finally:
        if lock_path.exists():
            lock_path.unlink()

    print(f"Processed {processed_files} file(s) " f"under '{target_arg}'.")


if __name__ == "__main__":
    main()
