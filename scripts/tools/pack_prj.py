from pathlib import Path
from datetime import datetime
import shutil


# ============================================================
# Configuration
# ============================================================

BASE_DIR = Path(__file__).resolve().parent

SOURCE_DIR = BASE_DIR.parent / "emb" / "src"
TARGET_DIR = BASE_DIR / "src"

ARCHIVE_NAME = "src.zip"


# ============================================================
# Derived paths
# ============================================================

ARCHIVE = BASE_DIR / f"{TARGET_DIR.name}.zip"

CURRENT_TIME = datetime.now().strftime("%Y-%m-%d %H:%M")


# ============================================================
# Info
# ============================================================

print(f"BASE_DIR   : {BASE_DIR}")
print(f"SOURCE_DIR : {SOURCE_DIR}")
print(f"TARGET_DIR : {TARGET_DIR}")
print(f"ARCHIVE    : {ARCHIVE}")
print(f"TIME       : {CURRENT_TIME}")


# ============================================================
# Step 1: Copy source
# ============================================================

print("\n1. Copying source...")

if TARGET_DIR.exists():
    shutil.rmtree(TARGET_DIR)

shutil.copytree(SOURCE_DIR, TARGET_DIR)


# ============================================================
# Step 2: Remove __pycache__
# ============================================================

print("2. Removing __pycache__ directories...")

for pycache_dir in TARGET_DIR.rglob("__pycache__"):
    if pycache_dir.is_dir():
        shutil.rmtree(pycache_dir)


# ============================================================
# Step 3: Create archive
# ============================================================

print("3. Creating archive...")

if ARCHIVE.exists():
    ARCHIVE.unlink()

shutil.make_archive(
    base_name=str(ARCHIVE.with_suffix("")),
    format="zip",
    root_dir=BASE_DIR,
    base_dir=TARGET_DIR.name,
)


# ============================================================
# Step 4: Remove copied directory
# ============================================================

# Uncomment if the temporary copy is not needed afterwards
print("4. Removing copied source...")
shutil.rmtree(TARGET_DIR)


print()
print(f"Done! {CURRENT_TIME}")
print(f"Archive: {ARCHIVE}")