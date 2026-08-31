from pathlib import Path
import sys

# ============================================================
# Templates
# ============================================================
HEADER_TEMPLATE = """// {name}.h

#pragma once

// #include ""

"""


CPP_TEMPLATE = """// {name}.cpp

#include "{name}.h"

// ---------------------------------

// ---------------------------------

"""


# ============================================================
# Helpers
# ============================================================


def input_required(prompt):
    while True:
        value = input(prompt).strip()

        if value.lower() in ("q", "quit", "cancel"):
            return None

        if value:
            return value

        print("Error: input cannot be empty. Please try again.")


# ============================================================
# Main
# ============================================================


def main():
    script_dir = Path(__file__).resolve().parent

    print("Create C++ files")
    print("Enter 'q' to cancel.")
    print()

    # --------------------------------------------------------
    # Base path from command line
    # --------------------------------------------------------

    if len(sys.argv) > 2:
        print("Error: only one command-line path is allowed.")
        return

    if len(sys.argv) == 2:
        base_path = Path(sys.argv[1])
    else:
        base_path = Path(".")

    # Make command-line path relative to the script location
    base_dir = (script_dir / base_path).resolve()

    # --------------------------------------------------------
    # Additional path
    # --------------------------------------------------------

    path = input("Path: ").strip()

    if path.lower() in ("q", "quit", "cancel"):
        print("Cancelled.")
        return

    # Empty path and "." mean the base directory
    if path == ".":
        path = ""

    # --------------------------------------------------------
    # File name
    # --------------------------------------------------------

    name = input_required("File name: ")

    if name is None:
        print("Cancelled.")
        return

    # Remove extensions if they were entered accidentally
    if name.endswith(".cpp") or name.endswith(".h"):
        name = Path(name).stem

    # --------------------------------------------------------
    # Target directory
    # --------------------------------------------------------

    target_dir = base_dir / path

    try:
        target_dir.mkdir(parents=True, exist_ok=True)

        header_file = target_dir / f"{name}.h"
        cpp_file = target_dir / f"{name}.cpp"

        created_files = []

        # ----------------------------------------------------
        # Header
        # ----------------------------------------------------

        if not header_file.exists():
            header_file.write_text(HEADER_TEMPLATE.format(name=name), encoding="utf-8")

            created_files.append(header_file.name)

        # ----------------------------------------------------
        # CPP
        # ----------------------------------------------------

        if not cpp_file.exists():
            cpp_file.write_text(CPP_TEMPLATE.format(name=name), encoding="utf-8")

            created_files.append(cpp_file.name)

        # ----------------------------------------------------
        # Result
        # ----------------------------------------------------

        print()

        if created_files:
            print("Success.")
            print(f"Folder: {target_dir}")

            for file_name in created_files:
                print(f"Created: {file_name}")

        else:
            print("Nothing created.")
            print("Both files already exist.")
            print(f"Folder: {target_dir}")

    except OSError as error:
        print()
        print("Error: files could not be created.")
        print(error)


if __name__ == "__main__":
    main()
