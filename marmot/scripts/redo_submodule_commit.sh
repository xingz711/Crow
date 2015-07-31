#!/bin/bash

#
#assuming the top commit is a moose submodule update redo it to avoid a merge conflict
#

# 1. check if top commit is submodule only
LOGMD5=$(git log --stat -1 | tail -n 2 | md5)
if [ $LOGMD5 != '6e2edf9f24093c38b0fe6422c6fb3028' ]
then
  echo 'Top commit is not a pure moose submodule update!'
  exit 1
fi

# 2. go to repository root
MARMOT_DIR=`dirname $0`/..
pushd $MARMOT_DIR

# 3. get the commit message
MESSAGE=`git log -1 --pretty=%B`

# 4. get the submodule hash
cd moose
HASH=`git log -1 --pretty=%H`
cd ..

# 5. roll back that commit
git reset --hard HEAD^

# 6. rebase and update submodule
git pull --rebase upstream devel
git submodule update

# 7. restore hash and recommit
cd moose
git checkout $HASH
cd ..
git add moose
git commit -m "$MESSAGE"

# 8. done
popd
