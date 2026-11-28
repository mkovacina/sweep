# Working with Claude

Claude (Claude Code) works on this repository through its own GitHub
account, so its issues, commits, pull requests and review replies are
clearly separate from the owner's. The owner reviews every pull request
and is the only one who merges.

## Roles

| | Claude (bot account) | Owner |
|---|---|---|
| Issues | Opens issues for bugs and work it finds; works on issues assigned to it | Opens, triages, assigns and closes issues |
| Branches | One branch per issue, off the latest `main` | — |
| Pull requests | Opens one PR per issue; answers review questions; pushes follow-up fixes | Reviews, approves, requests changes |
| `main` | Never pushes to it, never merges into it | Merges PRs |

## Rules

1. **One issue per pull request.** A PR fixes exactly one issue and its
   description says `Closes #<n>`. The only exception is issues that
   are duplicates of each other: close the duplicates and list every
   one in the PR (`Closes #12, closes #15`).
2. **One branch per issue**, named `issue-<n>-<short-slug>`
   (e.g. `issue-12-eof-on-arm`) and branched from the latest `main`.
3. **Stay in scope.** If the work turns up another problem, Claude opens
   a new issue for it instead of fixing it in the current PR.
4. **Claude never merges, approves, or pushes to `main`.** The owner
   merges.
5. **Don't rewrite history once review starts.** After a review is
   requested, Claude adds follow-up commits instead of amending or
   force-pushing, so the reviewer can see what changed. Rebasing onto
   `main` happens only when the owner asks for it.
6. **Everything Claude does on GitHub uses the bot account**, through
   `scripts/as-claude`. The owner's own git and `gh` identity stays
   unchanged in this clone.

## Workflow

1. **Issue.** The owner opens an issue and assigns it to the bot, or
   Claude opens one for something it found (with what's wrong, where,
   and how it was found). Claude works on one only when the owner asks
   in a Claude Code session.
2. **Branch.** Claude fetches `main`, creates `issue-<n>-<slug>`, and
   commits there in small, self-explanatory commits.
3. **Verify.** Before opening the PR, Claude builds and tests in the
   devcontainer image (the project's target environment), at minimum
   `make build-life`, `make build-tests && make run-tests`, and
   `make analyze` where available, plus whatever the issue needs.
4. **Pull request.** Claude pushes the branch and opens a PR against
   `main` with `Closes #<n>`, a summary of the change, and what was
   run to verify it (including anything that couldn't be run).
5. **Review.** The owner reviews on GitHub, then tells Claude in a
   Claude Code session that the PR has feedback. Claude reads the
   review and comments, replies in each thread, and pushes follow-up
   commits. Claude doesn't resolve review threads; the reviewer does.
6. **Merge.** The owner merges (merge style is the owner's choice).
   The branch is deleted after the merge.

### Dependent issues

If issue B can't be done until A's PR merges, Claude waits when it can.
When it can't, it branches B from A's branch, opens B's PR as a draft
based on A's branch, and says so in the description. When A merges and
its branch is deleted, GitHub retargets B's PR to `main`.

## Running commands as the bot

`scripts/as-claude` runs one `git` or `gh` command as the bot:

```sh
scripts/as-claude git commit -m "Fix EOF detection on ARM"
scripts/as-claude git push -u origin issue-12-eof-on-arm
scripts/as-claude gh issue create --title "..." --body "..."
scripts/as-claude gh pr create --base main --title "..." --body "..."
```

It sets the bot as commit author and committer, points `gh` at the
bot's token (`GH_TOKEN`), and replaces git's credential helpers with one
that supplies that token for the push. The token stays in the macOS
Keychain and is never written to a file or shown on a command line.
Commands run without the wrapper still use the owner's identity.

The wrapper uses the macOS Keychain, so it runs on the host, not
inside the devcontainer. Builds and tests can still run in the
container.

## Setup (one time, done by the owner)

1. **Create the bot account.** Sign up a separate GitHub account, e.g.
   `mkovacina-claude`. GitHub allows one person to run a machine
   account in addition to their own. Turn on two-factor
   authentication for it.
2. **Make it a collaborator.** From the owner's account: repository
   Settings → Collaborators → Add people → the bot. Accept the
   invitation from the bot's account.
3. **Create its token.** Signed in as the bot: Settings → Developer
   settings → Personal access tokens → Tokens (classic) → Generate new
   token, with the **`public_repo`** scope (the repo is public; use
   `repo` if it ever becomes private) and an expiry date. A classic
   token is used because a fine-grained token can't be scoped to a
   repository that the bot only collaborates on. The bot can only
   reach repositories it has been invited to.
4. **Store the token in the Keychain.** On the Mac, run this and paste
   the token when prompted:

   ```sh
   security add-generic-password -s sweep-claude-bot -a <bot-login> -w
   ```

5. **Tell the clone who the bot is.** In this clone, using the
   bot's no-reply address from its Settings → Emails page (it looks
   like `<id>+<bot-login>@users.noreply.github.com`):

   ```sh
   git config sweep.botLogin <bot-login>
   git config sweep.botEmail <id>+<bot-login>@users.noreply.github.com
   ```

6. **Protect `main`.** Repository Settings → Rules → Rulesets → New
   branch ruleset, targeting the default branch:
   - Require a pull request before merging, with 1 required approval
   - Block force pushes
   - Restrict deletions
   - Add *Repository admin* to the bypass list only if the owner wants
     to keep pushing straight to `main`
7. **Delete merged branches automatically.** Settings → General →
   "Automatically delete head branches".
8. **Check it works:**

   ```sh
   scripts/as-claude gh api user -q .login   # prints the bot's login
   ```

### Rotating or revoking access

- **New token:** generate it as the bot, then rerun the `security`
  command from step 4 with `-U` to replace the stored one.
- **Revoke everything:** delete the token on GitHub, remove the bot
  from the repository's collaborators, and run
  `security delete-generic-password -s sweep-claude-bot`.
