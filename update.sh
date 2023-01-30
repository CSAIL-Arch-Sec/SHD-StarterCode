#!/bin/bash
# Updates repository to latest starter code
#
# Adapted from Oliver Beckstein's ASU-CompMethodsPhysics-PHY494 course 2016-2020 placed into the public domain

# With GitHub template repositories one needs to use --allow-unrelated-histories
# at least once. https://help.github.com/en/github/creating-cloning-and-archiving-repositories/creating-a-repository-from-a-template

progname="$0"
REMOTE_NAME="startercode"
REMOTE_URL="https://github.com/CSAIL-Arch-Sec/SHD-StarterCode.git"

# progname, from top dir
UPDATESH="./deploy/$(basename $progname)"

CONTACT_MESSAGE="Contact the instructor and TA with a screen shot of ALL output from running $0."

function die () {
    local msg="$1" err=${2:-1}
    echo "ERROR: ${msg}."
    exit $err
}

# ensure everything relative to top dir
topdir="$(git rev-parse --show-toplevel)" || die "Failed to get rootdir"
cd "${topdir}" || die "Failed to get to the git root dir ${rootdir}"


# first time
# 1. set remote repo
# 2. merge histories between student (template) and remote skeleton

if ! git remote get-url ${REMOTE_NAME} >/dev/null 2>&1; then
    echo "Adding remote repository '${REMOTE_NAME}'."
    git remote add ${REMOTE_NAME} ${REMOTE_URL}

    echo "Merging histories for the first time..."
    set -x
    git pull --allow-unrelated-histories -s recursive -X theirs --no-edit  ${REMOTE_NAME} main || \
	{ git rev-list -1 MERGE_HEAD >/dev/null 2>&1 && git merge --abort ; \
	  git remote rm ${REMOTE_NAME}; \
	  die "Failed to merge histories. ${CONTACT_MESSAGE}" $?; }

    set +x
fi    

echo "updating repository... git pull from ${REMOTE_NAME}"
git pull --no-edit ${REMOTE_NAME} main || die "Failed to pull from ${REMOTE_NAME}. ${CONTACT_MESSAGE}"
