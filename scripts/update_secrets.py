from pathlib import Path
import re
import tempfile

PROJECT_ROOT = Path(__file__).resolve().parent.parent

CREDENTIALS_DIR = PROJECT_ROOT / "credentials"
SECRETS_FILE = PROJECT_ROOT / "src" / "secrets.h"


FILES = {
    "AWS_CERT_CA": CREDENTIALS_DIR / "AmazonRootCA1.pem",
    "AWS_CERT_CRT": "-certificate.pem.crt",
    "AWS_CERT_PRIVATE": "-private.pem.key",
}


# ---------------------------------


def find_credential(path_or_suffix: Path | str) -> Path:
    if isinstance(path_or_suffix, Path):
        return path_or_suffix

    files = list(CREDENTIALS_DIR.glob(f"*{path_or_suffix}"))

    if not files:
        raise FileNotFoundError(
            f"Credential file not found: *{path_or_suffix} in {CREDENTIALS_DIR}"
        )

    if len(files) > 1:
        raise RuntimeError(
            f"Multiple credential files found for *{path_or_suffix}: {files}"
        )

    return files[0]


def read_credential(path: Path) -> str:
    if not path.is_file():
        raise FileNotFoundError(f"Credential file not found: {path}")

    text = path.read_text(encoding="utf-8")

    if not text.strip():
        raise ValueError(f"Credential file is empty: {path}")

    return text.strip()


def make_raw_string(name: str, content: str) -> str:
    return f'static const char {name}[] = R"EOF(\n' f"{content}\n" f')EOF";'


def replace_credential(text: str, name: str, content: str) -> str:
    pattern = rf'static const char {re.escape(name)}\[\] = R"EOF\(' rf".*?" rf'\)EOF";'

    replacement = make_raw_string(name, content)

    result, count = re.subn(
        pattern,
        replacement,
        text,
        count=1,
        flags=re.DOTALL,
    )

    if count != 1:
        raise RuntimeError(f"Could not find block for {name} in {SECRETS_FILE}")

    return result


# ---------------------------------


def main() -> None:
    if not SECRETS_FILE.is_file():
        raise FileNotFoundError(f"Secrets file not found: {SECRETS_FILE}")

    text = SECRETS_FILE.read_text(encoding="utf-8")

    for name, path_or_suffix in FILES.items():
        path = find_credential(path_or_suffix)
        content = read_credential(path)
        text = replace_credential(text, name, content)

        print(f"Loaded: {path.name}")

    # Write atomically: first create a temporary file,
    # then replace the original only after everything succeeded.
    with tempfile.NamedTemporaryFile(
        "w",
        encoding="utf-8",
        newline="",
        delete=False,
        dir=SECRETS_FILE.parent,
    ) as temp_file:
        temp_path = Path(temp_file.name)
        temp_file.write(text)

    temp_path.replace(SECRETS_FILE)

    print(f"Updated: {SECRETS_FILE}")


# ---------------------------------


if __name__ == "__main__":
    main()
