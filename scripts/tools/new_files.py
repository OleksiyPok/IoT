# scripts/tools/new_files.py

from pathlib import Path
import sys

HEADER_TEMPLATE = """// {name}.h

#pragma once

// #include ""

"""

CPP_TEMPLATE = """// {name}.cpp

#include "{name}.h"

// ---------------------------------

// ---------------------------------

"""


def print_help():
    print("Usage:")
    print("  py new_files.py [base_path]")
    print()
    print("Create a C++ header (.h) and source (.cpp) file pair.")
    print()
    print("Arguments:")
    print("  base_path   Base directory. Relative paths are resolved from the")
    print("              terminal's current working directory. Defaults to '.'.")
    print()
    print("Interactive input:")
    print("  Path        Subdirectory inside base_path where files will be created.")
    print("  File name   Base name for the .h and .cpp files.")
    print()
    print("Confirmation:")
    print("  Enter / 1 / o  = OK, create files")
    print("  2 / e           = Edit")
    print("  3 / q           = Cancel")
    print("  Empty choice: first time = warning and repeat; second time = exit.")


def input_required(prompt):
    while True:
        value = input(prompt).strip()
        if value.lower() in ("q", "quit", "cancel"):
            return None
        if value:
            return value
        print("Error: input cannot be empty. Please try again.")


def normalize_name(name):
    if name.endswith(".cpp") or name.endswith(".h"):
        return Path(name).stem
    return name


def main():
    if any(arg in ("-h", "--help") for arg in sys.argv[1:]):
        print_help()
        return

    if len(sys.argv) > 2:
        print("Error: only one command-line path is allowed.")
        print("Use -h or --help for help.")
        return

    terminal_dir = Path.cwd()

    print("=" * 60)
    print("CREATE C++ FILES")
    print("=" * 60)
    print("This script will ask for a base directory, a subdirectory, and a file name.")
    print("It will then show exactly where the .h and .cpp files will be created")
    print("and ask for confirmation before making any changes.")
    print(f"Relative paths are resolved from the terminal directory: {terminal_dir}")
    print("Enter 'q' at an input prompt to cancel.")
    print("=" * 60)
    print()

    base_path = Path(sys.argv[1]) if len(sys.argv) == 2 else Path(".")
    base_dir = (terminal_dir / base_path).resolve()

    while True:
        path = input("Path: ").strip()
        if path.lower() in ("q", "quit", "cancel"):
            print("Cancelled.")
            return
        if path in ("", "."):
            path = ""
        name = input_required("File name: ")
        if name is None:
            print("Cancelled.")
            return
        name = normalize_name(name)
        break

    empty_choice_seen = False

    while True:
        target_dir = (base_dir / path).resolve()
        header_file = target_dir / f"{name}.h"
        cpp_file = target_dir / f"{name}.cpp"

        print()
        print("=" * 60)
        print("CREATE PLAN")
        print("=" * 60)
        print(f"BASE_DIR   : {base_dir}")
        print(f"TARGET_DIR : {target_dir}")
        print(f"HEADER     : {header_file}")
        print(f"CPP        : {cpp_file}")
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
            print("Edit target:")
            print(f"  Base path: {base_dir}")
            path = input("Path: ").strip()
            if path.lower() in ("q", "quit", "cancel"):
                print("Cancelled.")
                return
            if path in ("", "."):
                path = ""
            name = input_required("File name: ")
            if name is None:
                print("Cancelled.")
                return
            name = normalize_name(name)
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

    try:
        target_dir.mkdir(parents=True, exist_ok=True)
        created_files = []

        if not header_file.exists():
            header_file.write_text(HEADER_TEMPLATE.format(name=name), encoding="utf-8")
            created_files.append(header_file.name)

        if not cpp_file.exists():
            cpp_file.write_text(CPP_TEMPLATE.format(name=name), encoding="utf-8")
            created_files.append(cpp_file.name)

        print()
        print("RESULT")
        print("=" * 60)
        print(f"Folder : {target_dir}")
        if created_files:
            print(f"Created: {len(created_files)} file(s)")
            for file_name in created_files:
                print(f"  {file_name}")
        else:
            print("Created: 0 files")
            print("Both files already exist.")
        print("=" * 60)

    except OSError as error:
        print()
        print("RESULT")
        print("=" * 60)
        print("Error: files could not be created.")
        print(error)
        print("=" * 60)


if __name__ == "__main__":
    main()
