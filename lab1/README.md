# Hands-On Hardware Side Channels Lab

*6.888: Secure Hardware Design, Spring 2022*

This repository contains all the starting code you will need for the lab. The lab handout can be found on the [course website](http://csg.csail.mit.edu/6.888Yan/labs/).

## Accessing the Lab Machine

We have provided a lab machine for you to run your attacks on. The machine is located at `unicorn.csail.mit.edu`. You should SSH in with your Kerberos as the username.

```console
ssh <kerberos>@unicorn.csail.mit.edu
```

A randomly-generated password will be emailed to you when the lab is released. Please log in and change your password immediately to something secure using the `passwd` command. Please note that your account will be deleted at the end of this course.

## Starting the Lab

As stated on the lab handout, you must first change the `SENDER_CPU` and `RECEIVER_CPU` variables in the Makefile to your assigned CPUs. These will be emailed to you along with your lab machine password when the lab is released. **Double check that you have set these values correctly.**

After completing these steps, you are now ready to start the lab. Good luck!
