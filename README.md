# md2manc

`md2manc` is yet another tool joining the "markdown to manpage converter" club. Unlike many such other tools, however:

- `md2manc` is pretty plain and portable to any POSIX-compliant system.

- `md2manc` seeks to keep the source markdown suitable for translation to HTML too, rather than locking the entire document to man page generation only. To this end:

  - `md2manc` actually bothers to highlight C code blocks in the synopsis instead of treating them the same as any other literal block.

  - `md2manc` also supports highlighting for command usage syntax, so the synopsis in section 1 man pages can also be a code block.

  - `md2manc` strips relative links, so an hypothetical HTML website can still link to elsewhere within it without cluttering the man page.

See [**md2manc**(1)](docs/md2manc.1.md) for details.

## Building

### Prerequisites

- POSIX.1-2008 environment
- C99 compiler
- Meson ≥1.1
- [md4c](https://github.com/mity/md4c) ≥0.5
- (Optional) [sigtrace](https://github.com/deimonn/sigtrace) ≥0.1

### Procedure

1. Configure and change into the build directory with `meson setup builddir && cd builddir`

2. Build with `ninja`

3. Optionally install the tool with `ninja install`

See Meson's [Quickstart Guide](https://mesonbuild.com/Quick-guide.html).

## Contributing

Contributions are welcome. The code here follows the [Linux kernel coding style](https://kernel.org/doc/html/latest/process/coding-style.html) but with 4-space indentation, disregarding the **Conditional Compilation** section, and disregarding that which is only applicable inside the kernel.

Code format is partially enforced using GNU indent by a **check-format** target, which prints out a diff when changes are necessary. Invoke the target with `ninja check-format` (ensure you have `indent` installed).

Beyond that, if you wish to contribute changes to the project, just ensure it builds cleanly and that all tests are passing.

## Future directions

Feature-wise, the program is complete. New additions and changes are likely to be in the form of optimizations, correctness changes or minor conveniences only.

However, there may be bugs or undesirable quirks, specially as the project currently lacks decent test coverage. Thus it'll remain in version 0._x_ until there's high certainty that the program is in good shape.
