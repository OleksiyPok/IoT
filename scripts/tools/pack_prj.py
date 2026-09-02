from pathlib import Path
from datetime import datetime
import shutil
import sys

# ============================================================
# Configuration
# ============================================================

BASE_DIR = Path(__file__).resolve().parent


# ============================================================
# Input
# ============================================================

if len(sys.argv) < 2:
    print("Usage:")
    print("  py pack_src.py <source_path>")
    sys.exit(1)

SOURCE_DIR = Path(sys.argv[1]).expanduser().resolve()

if not SOURCE_DIR.exists() or not SOURCE_DIR.is_dir():
    print("Error: invalid source directory:")
    print(f"  {SOURCE_DIR}")
    sys.exit(1)


# ============================================================
# Target
# ============================================================

print(f"Source directory:")
print(f"  {SOURCE_DIR}")

target_input = input("\nPackage to [script directory]: ").strip()

if target_input == "" or target_input == ".":
    TARGET_BASE_DIR = BASE_DIR
else:
    TARGET_BASE_DIR = Path(target_input).expanduser().resolve()


# ============================================================
# Derived paths
# ============================================================

PACKAGE_NAME = SOURCE_DIR.name

TARGET_DIR = TARGET_BASE_DIR / PACKAGE_NAME
ARCHIVE = TARGET_BASE_DIR / f"{PACKAGE_NAME}.zip"

CURRENT_TIME = datetime.now().strftime("%Y-%m-%d %H:%M")


# ============================================================
# Confirmation
# ============================================================

while True:
    print()
    print("=" * 60)
    print("PACKAGE PLAN")
    print("=" * 60)
    print(f"SOURCE_DIR : {SOURCE_DIR}")
    print(f"TARGET_DIR : {TARGET_DIR}")
    print(f"ARCHIVE    : {ARCHIVE}")
    print(f"TIME       : {CURRENT_TIME}")
    print("=" * 60)

    print()
    print("What would you like to do?")
    print("  [1] / o = OK, start packaging")
    print("  [2] / e = Edit target directory")
    print("  [3] / q = Cancel")

    choice = input("\nChoice: ").strip().lower()

    if choice in ("", "1", "o"):
        break

    if choice in ("2", "e"):
        target_input = input("\nPackage to [script directory]: ").strip()

        if target_input == "" or target_input == ".":
            TARGET_BASE_DIR = BASE_DIR
        else:
            TARGET_BASE_DIR = Path(target_input).expanduser().resolve()

        TARGET_DIR = TARGET_BASE_DIR / PACKAGE_NAME
        ARCHIVE = TARGET_BASE_DIR / f"{PACKAGE_NAME}.zip"

        continue

    if choice in ("3", "q"):
        print("\nCancelled.")
        sys.exit(0)

    print("\nInvalid choice. Please enter 1, 2 or 3.")

# ============================================================
# Prepare target
# ============================================================

TARGET_BASE_DIR.mkdir(parents=True, exist_ok=True)


# ============================================================
# Step 1: Copy source
# ============================================================

print("\n1. Copying source...")

if TARGET_DIR.exists():
    shutil.rmtree(TARGET_DIR)

shutil.copytree(SOURCE_DIR, TARGET_DIR)


# ============================================================
# Step 2: Remove unnecessary files
# ============================================================

print("2. Removing unnecessary files...")

ALLOWED_EXTENSIONS = {
    ".cpp",
    ".h",
    ".hpp",
    ".c",
    ".cc",
    ".cxx",
}


for path in list(TARGET_DIR.rglob("*")):
    if not path.is_file():
        continue

    # Remove Python cache files/directories
    if "__pycache__" in path.parts:
        path.unlink()
        continue

    # Remove README files
    if path.name.lower().startswith("readme"):
        path.unlink()
        continue

    # Remove everything except source files
    if path.suffix.lower() not in ALLOWED_EXTENSIONS:
        path.unlink()


# Remove empty directories
for path in sorted(TARGET_DIR.rglob("*"), reverse=True):
    if path.is_dir():
        try:
            path.rmdir()
        except OSError:
            pass


# ============================================================
# Step 3: Create archive
# ============================================================

print("3. Creating archive...")

if ARCHIVE.exists():
    ARCHIVE.unlink()

shutil.make_archive(
    base_name=str(ARCHIVE.with_suffix("")),
    format="zip",
    root_dir=TARGET_BASE_DIR,
    base_dir=TARGET_DIR.name,
)


# ============================================================
# Step 4: Remove temporary directory
# ============================================================

print("4. Removing temporary directory...")

shutil.rmtree(TARGET_DIR)


# ============================================================
# Done
# ============================================================

print()
print(f"Done! {CURRENT_TIME}")
print(f"Archive: {ARCHIVE}")
