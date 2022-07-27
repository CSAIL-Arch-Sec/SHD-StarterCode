#!/usr/bin/env python3
# Automatically generate student repositories on GitHub.
# Install dependencies with:
# python3 -m pip install PyGithub
from github import Github, GithubException
import csv

# Put your token in token.txt
# See: https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/creating-a-personal-access-token
TOKEN="ahahaha nice try"

# The API endpoint for your GitHub server.
GITHUB_API="https://github.mit.edu/api/v3"

# The name of the organization under which your student repos will exist.
# We recommend 1 organization per semester.
ORG_NAME="6-888-sp22"

# Name of the main repo to which we push labs:
MAIN_REPO="lab_release"

# org = organization to create the repo under
# name = repo name
# their_name = name to use for student
def create_repo(org, name, their_name):
    print(f"Making repo {name} in {org.login}")
    try:
        org.create_repo(name,
                        description=f"MIT 6.888 Spring 2022 Private Repo for {their_name}",
                        private=True,
        )
    except:
        print("Error creating the repo. Maybe it already exists?")

# org = organization to collab under
# repo_name = repo name
# user_name = user to add as a collaborator
def invite_to_repo(org, repo_name, user_name, perms='push'):
    try:
        repo = org.get_repo(repo_name)
        repo.add_to_collaborators(user_name, permission=perms)
    except:
        print(f"Error adding {user_name} to {repo_name}, maybe the repo doesn't exist?")

# g = GitHub object
def add_all_students(org):
    with open("students.csv", "r") as students_f:
        reader = csv.DictReader(students_f)
        students = list(reader)

        for student in students:
            print(f"Creating repo for {student['kerb']}")
            create_repo(org, student['kerb'], student['name'])
            invite_to_repo(org, student['kerb'], student['kerb'], perms='push')
            invite_to_repo(org, MAIN_REPO, student['kerb'], perms='pull')

def main():
    # Token is secretly stored in token.txt
    with open("token.txt", "r") as token_f:
        TOKEN=token_f.read()

    g = Github(base_url=GITHUB_API, login_or_token=TOKEN)

    org = g.get_organization(ORG_NAME)
    add_all_students(org)

if __name__=="__main__":
    main()
