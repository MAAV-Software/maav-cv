# MAAV Computer Vision Repository

## Contents

This repository containc Python files that will implement and test our computer
vision algorithms that we intend to use on our vehicle for Mission 9 of IARC.

While our code base is written in C++ primarily, we are composing our algorithms
in Python because the OpenCV library is available for both languages, and Python
allows an easy way to test code and import the OpenCV library while avoiding the
hassles that come with including OpenCV using CMake, ROS with catkin_ws, etc.

Additionally, this gives members of our team experience working with Python.

## Getting Started

This tutorial was intended for Ubuntu / Linux, and modified for support in Mac
and Windows WSL environments. While most issues have been worked out, you may
have some difficulties still - reach out if you have trouble.

First, clone this repository. You can either use HTTPS or SSH with a key setup
through your account.
```
git clone <link>
cd maav-cv
```

Next, install required software to create a Python virtual environment for
installing dependencies and working on CV projects.
```
# WSL / Linux:
sudo apt-get update
sudo apt-get install python3.7 python3-pip
pip install virtualenv

# Mac with Homebrew: NEEDS CONFIRMATION
brew install python@3.7
pip install virtualenv
```

Setup your virtual environment using Python 3.7.x.
```
virtualenv --python=/usr/bin/python3.7 env
source env/bin/activate
```

**NOTE: You MUST source the activate file everytime you begin any testing or
development!! That lets your terminal know what package install directory to
use for the code.**

We have included `requirements.txt` to install OpenCV and any other required
Python libraries. To install these requirements,
run the following command:
```
pip install -r requirements.txt
```

## Windows and WSL Aside: X-Server

Normally OpenCV GUI code can run just fine in this environment. However, if
you attempt to run this code in Windows with WSL, you will encounter an
error similar to one of these:
```
python3 hello.py
  ...
  qt.qpa.xcb: could not connect to display
  ...

# OR

python3 hello.py
  ...
  : cannot connect to X server
```

This is because Windows and WSL do not natively support Linux GUI applications.
We will install an external program to host an X-Server, which will process
data and display a GUI that interacts with your program.

The X-Server application we will use is vcXsrv, which you can
[download here](https://sourceforge.net/projects/vcxsrv/). After installing,
start the application, and start it with default settings **EXCEPT FOR ONE:
Disable Access Control**

![vcXsrv App Image](https://techcommunity.microsoft.com/t5/image/serverpage/image-id/201596iBCB2B8DA889830E0)

Then, you need to set an environment variable in your WSL terminal so that
the X-Server will be used for displaying GUI apps.
```
export DISPLAY="`grep nameserver /etc/resolv.conf | sed 's/nameserver //'`:0"
```
**Tip:** Add this line to the end of your `~/.bashrc` so that it will run
everytime you start a new bash terminal. It's an extra step you won't have
to remember.

## Running Sample Code

At this point, you should be able to run the sample code without difficulties:
```
python3 hello.py
```
I encourage you to look at the code and modify it. For example, change the
image used, modify the image using OpenCV functions, or do anything else
creative you can think of!

## Development Etiquette

**UPDATE 9/30/20: We will use forks for developing, and committing to this
official repo will happen through the website, where a pull request can be
created. The README will be updated with info on how to fork later.**

This section is subject to change throughout the course of development.
At times, Google can be your friend with Git, but reach out to us with
any help you may need!

When in doubt, [check out "Oh Shit! Git!"](https://ohshitgit.com)

### Git Identity

Make sure your identity through Git is setup well.

```
# List current global config, which may contain identity info
git config --global -l
  user.name=Jacob Minock
  user.email=jaminock@umich.edu
  core.editor=vim
  ...

# Set or Change some things (for any repository you use)
# Omit --global to just make it for THIS repository
git config --global user.name "Jacob Minock"
git config --global user.email "jaminock@umich.edu"
```

### Git Branches

When developing a specific, new feature or set of changes, we suggest making
a branch for new files or the modifications of existing files. This will
allow your `master` branch to stay clean and organized in terms of commit
history and other qualities. Generally changes to the `master` branch only
happen with very small fixes - like updating a `README.md` or one doc.

```
# List branches
git branch

# Create a new branch
git checkout -b <branch>

# Switch to a branch
git checkout <branch>
```

### Git Repository Fork

TODO

## Appendix A: Helpful Resources and Links

OpenCV in Python Tutorials Page:

https://docs.opencv.org/master/dc/d4d/tutorial_py_table_of_contents_gui.html

Intel RealSense Python Wrapper Docs:

https://github.com/IntelRealSense/librealsense/tree/master/wrappers/python