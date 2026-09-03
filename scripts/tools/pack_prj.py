# scripts/tools/pack_prj.py

from pathlib import Path
from datetime import datetime
import shutil
import sys

ALLOWED_EXTENSIONS = {
    ".cpp",
    ".h",
    ".hpp",
    ".c",
    ".cc",
    ".cxx",
}


def print_help():
    print("Usage:")
    print("  py pack_prj.py <source_path>")
    print()
    print("Create a ZIP package containing only C/C++ source and header files")
    print("from the selected source directory.")
    print()
    print("Arguments:")
    print("  source_path  Directory to package. Relative paths are resolved from")
    print("               the terminal's current working directory.")
    print()
    print("Interactive input:")
    print("  Package to   Destination directory for the temporary package folder")
    print("               and resulting ZIP archive. Empty input or '.' means")
    print("               the terminal's current working directory.")
    print()
    print("The temporary package directory is removed after the ZIP is created.")
    print("Existing package directory and archive at the target are replaced.")
    print()
    print("Confirmation:")
    print("  1 / q         = Cancel")
    print("  2 / e         = Edit destination")
    print("  3 / o         = OK, continue")
    print("  Empty choice: first time = warning and repeat; second time = exit.")


def resolve_from_terminal(terminal_dir, value):
    path = Path(value).expanduser()
    if path.is_absolute():
        return path.resolve()
    return (terminal_dir / path).resolve()


def main():
    if any(arg in ("-h", "--help") for arg in sys.argv[1:]):
        print_help()
        return

    if len(sys.argv) != 2:
        print("Usage: py pack_prj.py <source_path>")
        print("Use -h or --help for help.")
        return

    terminal_dir = Path.cwd()
    source_dir = resolve_from_terminal(terminal_dir, sys.argv[1])

    if not source_dir.exists() or not source_dir.is_dir():
        print("Error: invalid source directory:")
        print(f"  {source_dir}")
        return

    package_name = source_dir.name
    current_time = datetime.now().strftime("%Y-%m-%d %H:%M")

    print("=" * 60)
    print("PACKAGE C/C++ PROJECT")
    print("=" * 60)
    print("The script will copy the selected source directory to a temporary")
    print("package directory, remove files that are not C/C++ source/header files,")
    print("create a ZIP archive, and then remove the temporary directory.")
    print(f"Relative paths are resolved from the terminal directory:")
    print(f"  {terminal_dir}")
    print(
        "The confirmation plan will show whether existing target items will be replaced."
    )
    print()
    print("Enter 'q' at an input prompt to cancel.")
    print("=" * 60)
    print()
    print("Source directory:")
    print(f"  {source_dir}")

    target_input = input("\nPackage to: ").strip()
    if target_input.lower() in ("q", "quit", "cancel"):
        print("Cancelled.")
        return

    target_base_dir = resolve_from_terminal(terminal_dir, target_input or ".")

    empty_choice_seen = False

    while True:
        target_dir = target_base_dir / package_name
        archive = target_base_dir / f"{package_name}.zip"

        try:
            target_dir.relative_to(source_dir)
            invalid_target = True
        except ValueError:
            invalid_target = False

        if invalid_target or target_dir == source_dir:
            print()
            print("Error: target package directory cannot be the source directory")
            print("or a directory inside it.")
            target_input = input("Package to: ").strip()
            if target_input.lower() in ("q", "quit", "cancel"):
                print("Cancelled.")
                return
            target_base_dir = resolve_from_terminal(terminal_dir, target_input or ".")
            continue

        print()
        print("=" * 60)
        print("PACKAGE PLAN")
        print("=" * 60)
        print(f"SOURCE_DIR : {source_dir}")
        print(f"TARGET_DIR : {target_dir}")
        print(f"ARCHIVE    : {archive}")
        print(f"TIME       : {current_time}")
        if target_dir.exists():
            print(
                "WARNING    : target package directory already exists and will be replaced."
            )
        if archive.exists():
            print("WARNING    : archive already exists and will be replaced.")
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
            target_input = input("\nPackage to: ").strip()
            if target_input.lower() in ("q", "quit", "cancel"):
                print("Cancelled.")
                return
            target_base_dir = resolve_from_terminal(terminal_dir, target_input or ".")
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
        target_base_dir.mkdir(parents=True, exist_ok=True)

        print("\n1. Copying source...")
        if target_dir.exists():
            shutil.rmtree(target_dir)
        shutil.copytree(source_dir, target_dir)

        print("2. Removing unnecessary files...")
        removed_files = 0
        for path in list(target_dir.rglob("*")):
            if not path.is_file():
                continue
            if "__pycache__" in path.parts:
                path.unlink()
                removed_files += 1
                continue
            if path.name.lower().startswith("readme"):
                path.unlink()
                removed_files += 1
                continue
            if path.suffix.lower() not in ALLOWED_EXTENSIONS:
                path.unlink()
                removed_files += 1

        for path in sorted(target_dir.rglob("*"), reverse=True):
            if path.is_dir():
                try:
                    path.rmdir()
                except OSError:
                    pass

        print("3. Creating archive...")
        if archive.exists():
            archive.unlink()
        shutil.make_archive(
            base_name=str(archive.with_suffix("")),
            format="zip",
            root_dir=target_base_dir,
            base_dir=target_dir.name,
        )

        print("4. Removing temporary directory...")
        shutil.rmtree(target_dir)

        print()
        print("RESULT")
        print("=" * 60)
        print(f"Source  : {source_dir}")
        print(f"Archive : {archive}")
        print(f"Removed : {removed_files} file(s) from package copy")
        print(f"Time    : {current_time}")
        print("Temporary directory removed: yes")
        print("=" * 60)

    except (OSError, shutil.Error) as error:
        print()
        print("RESULT")
        print("=" * 60)
        print("Error: package could not be created completely.")
        print(error)
        print("=" * 60)


if __name__ == "__main__":
    main()
