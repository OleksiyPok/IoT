# scripts/tools/path_header_cpp.py

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

LOCK_FILE = ".path_header_cpp.lock"

EXTENSIONS = {
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


def get_script_dir() -> Path:
    return Path(__file__).resolve().parent


def resolve_path(base_dir: Path, value: str) -> Path:
    value = value.strip()

    if value in ("", "."):
        return base_dir

    path = Path(value)

    if path.is_absolute():
        return path.resolve()

    return (base_dir / path).resolve()


def request_existing_path(
    base_dir: Path,
    message: str,
) -> Path | None:

    while True:
        try:
            value = input(message).strip()
        except (EOFError, KeyboardInterrupt):
            print()
            return None

        if value.lower() in {"q", "quit", "cancel"}:
            return None

        path = resolve_path(base_dir, value)

        if path.exists() and path.is_dir():
            return path

        print(f"Path '{value}' does not exist " "or is not a directory.")
        print("Try again or enter 'q' to cancel.")


def get_header_base_path(target_dir: Path) -> list[str]:
    return [target_dir.name]


def parse_header_path(
    target_dir: Path,
    file_relative: Path,
    specification: str,
) -> str:

    specification = specification.strip()

    parts = [part for part in specification.replace("\\", "/").split("/") if part]

    if not parts or parts == ["."]:
        remove_count = 0
        parent_count = 0
    else:
        remove_count = 0
        parent_count = 0

        for part in parts:
            if part == "*":
                remove_count += 1

            elif part == "..":
                parent_count += 1

            elif part == ".":
                continue

            else:
                raise ValueError(f"Unsupported path component: '{part}'")

    base_parts = get_header_base_path(target_dir)

    if remove_count > len(base_parts):
        raise ValueError("Too many '*' components for the scan path.")

    base_parts = base_parts[remove_count:]

    parent_parts = []

    parent_dir = target_dir

    for _ in range(parent_count):
        parent_dir = parent_dir.parent

        if parent_dir == parent_dir.parent:
            raise ValueError("Cannot go above the filesystem root.")

        parent_parts.insert(
            0,
            parent_dir.name,
        )

    all_parts = parent_parts + base_parts + list(file_relative.parts)

    return Path(*all_parts).as_posix()


def request_header_specification(
    target_dir: Path,
) -> str | None:

    while True:
        try:
            value = input("Path in header [., */ or ../]: ").strip()
        except (EOFError, KeyboardInterrupt):
            print()
            return None

        if value.lower() in {"q", "quit", "cancel"}:
            return None

        try:
            parse_header_path(
                target_dir,
                Path("file.cpp"),
                value,
            )
            return value

        except ValueError as error:
            print(f"Invalid path specification: {error}")
            print("Try again or enter 'q' to cancel.")


def format_header_preview(
    target_dir: Path,
    specification: str,
) -> str:
    """Return a compact example of the header path shown in the plan."""
    parts = [part for part in specification.replace("\\", "/").split("/") if part]

    remove_count = sum(part == "*" for part in parts)
    parent_count = sum(part == ".." for part in parts)

    preview_parts = []

    # '*' removes components from the scan path in the header specification.
    # For the plan, show only the resulting directory prefix and represent
    # all scanned files by <files>.
    base_dir = target_dir
    for _ in range(remove_count):
        base_dir = base_dir.parent

    for _ in range(parent_count):
        base_dir = base_dir.parent

    if not parts or parts == ["."]:
        preview_parts = [target_dir.name]
    elif parent_count > 0:
        preview_parts = [target_dir.parents[parent_count - 1].name]
        if parent_count > 1:
            preview_parts = [
                target_dir.parents[index].name
                for index in range(parent_count - 1, -1, -1)
            ]
    elif remove_count > 0:
        preview_parts = []
    else:
        preview_parts = [target_dir.name]

    return "/".join(preview_parts + ["*"])


def should_process(
    path: Path,
    target_dir: Path,
) -> bool:

    if not path.is_file():
        return False

    if path.suffix.lower() not in EXTENSIONS:
        return False

    if any(part in EXCLUDE_DIRS for part in path.parts):
        return False

    try:
        path.relative_to(target_dir)
        return True
    except ValueError:
        return False


def remove_existing_header(
    lines: list[str],
) -> list[str]:

    index = 0

    while index < len(lines) and lines[index].strip() == "":
        index += 1

    while index < len(lines):
        line = lines[index].rstrip("\r\n")

        if not PATH_LIKE_REGEX.fullmatch(line):
            break

        index += 1

        while index < len(lines) and lines[index].strip() == "":
            index += 1

    return lines[index:]


def process_file(
    file_path: Path,
    target_dir: Path,
    specification: str,
) -> None:

    lines = file_path.read_text(encoding="utf-8-sig").splitlines(keepends=True)

    remaining_lines = remove_existing_header(lines)

    file_relative = file_path.relative_to(target_dir)

    header_path = parse_header_path(
        target_dir,
        file_relative,
        specification,
    )

    header = f"// {header_path}\n\n"

    file_path.write_text(
        header + "".join(remaining_lines),
        encoding="utf-8",
    )


def print_help():
    print("Usage:")
    print("  py path_header_cpp.py [scan_path]")
    print()
    print("Add or update a path header in C/C++ source and header files recursively")
    print("under the selected Scan path.")
    print()
    print("Arguments:")
    print("  scan_path    Directory to scan recursively. Relative paths are resolved")
    print(
        "               from the terminal's current working directory. If omitted, the"
    )
    print("               script asks for Scan path interactively.")
    print()
    print("Interactive input:")
    print("  Scan path       Used when the command-line path is missing or invalid.")
    print(
        "  Path in header  Controls the path written into each file and is calculated"
    )
    print("                  relative to Scan path. '.', '*/' and '../' keep their")
    print("                  existing meanings.")
    print()
    print("The script shows the number of files to be modified before confirmation.")
    print(
        "Only files with the supported extensions are processed; excluded directories"
    )
    print("are skipped.")
    print()
    print("Confirmation:")
    print("  1 / q = Cancel")
    print("  2 / e = Edit")
    print("  3 / o = OK, continue")
    print("  Empty choice: first time = warning and repeat; second time = exit.")


def main():
    if any(arg in ("-h", "--help") for arg in sys.argv[1:]):
        print_help()
        return

    if len(sys.argv) > 2:
        print("Error: only one command-line path is allowed.")
        print("Use -h or --help for help.")
        return

    terminal_dir = Path.cwd()
    script_dir = get_script_dir()

    target_arg = sys.argv[1] if len(sys.argv) == 2 else None
    target_path = (
        resolve_path(terminal_dir, target_arg) if target_arg is not None else None
    )

    print("=" * 60)
    print("UPDATE C++ PATH HEADERS")
    print("=" * 60)
    print("The script will recursively scan the selected Scan path, find files with")
    print("supported extensions, and update their leading path header.")
    print(
        f"Relative filesystem paths are resolved from the terminal directory: {terminal_dir}"
    )
    print("Path in header is different: it is calculated relative to Scan path.")
    print("The script will show the number of files to modify and ask for confirmation")
    print("before changing anything.")
    print("Enter 'q' at an input prompt to cancel.")
    print("=" * 60)
    print()

    if target_path is None:
        target_path = request_existing_path(terminal_dir, "Scan path: ")
        if target_path is None:
            print("Cancelled.")
            return
    elif not target_path.exists() or not target_path.is_dir():
        print(f"Target path '{target_arg}' does not exist or is not a directory.")
        target_path = request_existing_path(terminal_dir, "Scan path: ")
        if target_path is None:
            print("Cancelled.")
            return

    print(f"Selected scan path: {target_path}")

    specification = request_header_specification(target_path)
    if specification is None:
        print("Cancelled.")
        return

    empty_choice_seen = False

    while True:
        files_to_process = [
            file_path
            for file_path in target_path.rglob("*")
            if should_process(file_path, target_path)
        ]

        print()
        print("=" * 60)
        print("UPDATE PLAN")
        print("=" * 60)
        print(f"SCAN_PATH  : {target_path}")
        print(f"PATH HEADER: {specification}")
        print(f"HEADER PATH: {format_header_preview(target_path, specification)}")
        print(f"FILES      : {len(files_to_process)}")
        print("=" * 60)
        print()
        print("What would you like to do?")
        print("  [1] / q = Cancel")
        print("  [2] / e = Edit")
        print("  [3] / o = OK, continue")

        choice = input("\nChoice: ").strip().lower()
        if choice in ("3", "o"):
            break

        if choice in ("2", "e"):
            print()
            print("Edit scan settings:")
            target_path = request_existing_path(terminal_dir, "Scan path: ")
            if target_path is None:
                print("Cancelled.")
                return
            specification = request_header_specification(target_path)
            if specification is None:
                print("Cancelled.")
                return
            continue

        if choice in ("1", "q", "quit", "cancel"):
            print("\nCancelled.")
            return

        if choice == "":
            if not empty_choice_seen:
                empty_choice_seen = True
                print("\nYou have not made a selection. Please make a choice.")
                continue
            print("\nNo selection was made. Exiting.")
            return

        print("\nInvalid choice. Please enter 1, 2, 3, e, o or q.")

    lock_path = script_dir / LOCK_FILE
    if lock_path.exists():
        print("Script already running.")
        return

    processed_files = 0
    failed_files = []

    try:
        lock_path.write_text("lock", encoding="utf-8")

        for file_path in files_to_process:
            try:
                process_file(file_path, target_path, specification)
                processed_files += 1
            except (OSError, UnicodeError, ValueError) as error:
                failed_files.append((file_path, error))

    finally:
        if lock_path.exists():
            lock_path.unlink()

    print()
    print("RESULT")
    print("=" * 60)
    print(f"Scan path : {target_path}")
    print(f"Requested : {len(files_to_process)} file(s)")
    print(f"Processed : {processed_files} file(s)")
    print(f"Failed    : {len(failed_files)} file(s)")
    if failed_files:
        print()
        for file_path, error in failed_files:
            print(f"  {file_path}: {error}")
    print("=" * 60)


if __name__ == "__main__":
    main()
