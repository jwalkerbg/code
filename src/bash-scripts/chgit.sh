#!/bin/bash

# change commit properties based on some criteria

git filter-branch --env-filter 'if [ "$GIT_AUTHOR_EMAIL" = "n.tsvetkovv@gmail.com" ]; then GIT_AUTHOR_EMAIL="n.tsvetkovv@gmail.com"; GIT_AUTHOR_NAME="Ivan Cenov"; GIT_COMMITTER_EMAIL="Ivan.Cenov@resideo.com"; GIT_COMMITTER_NAME="Ivan Cenov"; fi' -- --all
