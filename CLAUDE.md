# CLAUDE.md

## GitHub workflow

Claude works on this repo through its own bot account. The full design,
setup and reasoning are in `docs/claude-workflow.md`; the rules are:

- Run every GitHub-facing `git` or `gh` command (commit, push, issues,
  PRs, review replies) through `scripts/as-claude`. Never use the
  owner's identity for them. If the wrapper reports that setup is
  missing, stop and point the owner to the Setup section.
- One issue per PR, with `Closes #<n>` in the description. Only issues
  that are duplicates of each other can share a PR.
- One branch per issue: `issue-<n>-<short-slug>`, off the latest `main`.
- Problems found along the way get a new issue, not a fix in the
  current PR.
- Never merge, approve, or push to `main`; the owner merges.
- Once review has started, add follow-up commits; don't amend or
  force-push unless the owner asks for a rebase.
- Verify in the devcontainer image before opening a PR, and list what
  was run in the PR description.

## Building

The target environment is the devcontainer (Alpine, clang, ARM64 on
Apple Silicon). See `readme.md` for build, test and run commands.
