# PolyBuild Basic Usage

This example shows how to define a minimal `polybuild.in` file for orchestrating builds.

## Example `polybuild.in`

```ini
@directive [plugin:cmake]
input.project_dir = ./src
input.build_type = release

action.compile
action.clean


