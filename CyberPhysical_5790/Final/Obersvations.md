Calvin Passmore

A02107892

ECE 6790

# Final Project Observations

## Installing Ivy Updates

[https://kenmcmil.github.io/ivy/install.html](https://kenmcmil.github.io/ivy/install.html)

Ken McMillan provided a simple python based installation guide. The only problem is that it's not completely up to date. Specifically in the git clone line, change the GitHub location to 

    $ git clone --recurse-submodules https://github.com/kenmcmil/ivy.git

Also, I would suggest building a python virtual environment, because the python 2.7 requirement is not up to date, and a lot of people probably already have python 3.10+ installed.
