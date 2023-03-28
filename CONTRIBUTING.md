# Contributing

Thanks for taking a look at the project. Small, focused patches are welcome.

## Workflow

1. Build with `make`.
2. Run `make test` before sending a change.
3. Keep commits scoped to one topic and describe the reason in the message.
4. Add a regression test for every bug fix.

## Style

The project follows the formatting in `.clang-format`. Run `clang-format -i`
on the files you touch. Public functions live in headers and are documented
with a short comment.

## Reporting bugs

Include the command line you used, the pattern file if it is relevant, and the
output of `./build/life --version`.
