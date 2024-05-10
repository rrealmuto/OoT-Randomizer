import os
import shutil
import subprocess

subprocess.run(
    ['cargo', '+nightly', 'build', '-Zbuild-std=core', '-Zbuild-std-features=compiler-builtins-mem', '--release', '--package=rust-n64-test', '--target=crate/rust-n64-test/assets/mips-ultra64-cpu.json'],
    env={**os.environ, 'RUSTFLAGS': '-Csymbol-mangling-version=v0'}, # symbol-mangling-version (https://github.com/rust-lang/rust/issues/60705) required for armips to accept the symbols
    check=True,
)
shutil.copyfile('target/mips-ultra64-cpu/release/librust_n64_test.a', 'ASM/build/librust_n64_test.a')
