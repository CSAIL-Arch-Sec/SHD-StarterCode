# MIT Secure Hardware Design
Welcome to the course starter code for MIT's Secure Hardware Design class!

This repository points to the starter code provided to students for each of our labs. This README contains instructions for how to run the labs on your own machines and is intended for use by individuals looking to self-study our materials (or instructors looking to use our materials as part of their course curriculum).

You can reach out to our team at `hw-sec-lab-dev` at `mit.edu`.

Our course website can be found [here](http://csg.csail.mit.edu/6.S983/).

This repository is a companion document to our lab handouts which can be found [here](http://csg.csail.mit.edu/6.S983/labs/).

# Want to use this in your course?
Please contact `hw-sec-lab-dev` at `mit.edu` before using any of our material in your course. We can provide you with the instructor solutions, sample gradebook, and grading scripts.

## Licensing
This code is released under the MIT license.

```
MIT License

Copyright (c) 2022 Mengjia Yan, Joseph Ravichandran, Peter Deutsch,
                   Weon Taek Na, Jack Cook, Miles Dai, Miguel Gomez-Garcia

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

Additionally:

```
Use of MIT Name
"MIT", "Massachusetts Institute of Technology", and its logos and seal
are trademarks of the Massachusetts Institute of Technology. Except for
purposes of attribution as required by our Creative Commons License, you
may not use MIT's names or logos, or any variations thereof, without
prior written consent of MIT. You may not use the MIT name in any of its
forms nor MIT seals or logos for promotional purposes, or in any way that
deliberately or inadvertently claims, suggests, or in MIT's sole judgment
gives the appearance or impression of a relationship with or endorsement
by MIT.
```

# Target Audience
We know that not every student who takes our course will pursue hardware security research as a career. In fact, it is likely many of them will never need to develop another microarchitectural attack again after this course.

For that reason, we want to ensure students learn not only how to implement cutting edge hardware attacks, but also understand how to reason about hardware security as an aspect of secure systems design and defense in depth.

These lab experiments provide the depth aspect of the course, where students implement full end to end microarchitectural attacks against real world processors. We encourage instructors who implement these labs in their course to encourage critical discussion of how to reason about these attacks in practice after allowing students to complete the exercises. We have found these labs provide a great jumping point leading to rich classroom discussions.

## Lab Descriptions

Here's a description of what to expect from each lab in this repository. Our lab content provides both a good deal of depth into what we think are the most important problems for architects to pay attention to, as well as a good deal of breadth to get students exposed to a variety of elements of secure hardware design.

# Lab 1 - Website Fingerprinting
**Learning Objectives**
* Explore a new kind of side channel from a high level language.
* Understand how to reason about the root cause of misleading microarchitectural observations.
* Develop a real-world website fingerprinting attack that works on modern browsers.

**Description**
In this lab, students implement the techniques from our group's ISCA 2022 paper `There's Always a Bigger Fish: A Case Study of a Misunderstood Timing Side Channel`. Students will begin by implementing a seemingly familiar cache-based side channel attack in Javascript, and will then be asked to reason about why this attack works. Then, students will remove a core part of the attack, but see that the code still works.

**Setup**
Students can complete this lab on their own machines. MacOS, Linux, Windows all should work. Google Chrome is required for Part 4 of this lab.

# Lab 2 - Cache Side Channels

**Learning Objectives**
* Discover L1/ L2/ L3 caches in practice and how they are shared in modern multicore systems.
* Explore eviction set construction and how to differentiate access latencies across the memory hierarchy.
* Implement the `prime+probe` technique to construct a covert channel.
* Build an adversary to reveal a secret from a victim process.

**Description**
This lab is a deep introduction to the problem of microarchitectural security. In this lab, students will learn how to reverse engineer an unfamiliar memory system, build a covert channel, and reveal a secret from a victim process.

**Setup**
For this lab, we granted each student their own SMT physical core (so, two logical cores) on a shared server. The TA maintained a list of which student owns which cores as part of the student information database, and a script was used to automatically email every student their assigned cores (as well as their login information).

Students were asked to fill in the `cpu.mk` file with their given cores (and we relied on a mix of the honor system and careful TA supervision to ensure that students did not maliciously attack other students).

We deployed this lab on a 96-core server. This lab could be done using any bare metal Intel machine (such as a Google Cloud Platform N2 instance).

We disabled the hardware prefetcher to make student's lives easier. Consult the ISA manual for your processor generation to determine which MSR should be written to for disabling hardware prefetching.

For our system (`Intel(R) Xeon(R) Gold 5220R CPU @ 2.20GHz`), we used the following configuration:

```
#!/bin/bash

sudo modprobe msr
sudo wrmsr -a 0x1a4 15
```

**Advice**
We have found this lab to be the most challenging of the five for a variety of reasons. As this is the student's first microarchitectural attack, there is a steep learning curve to learn how to reason about microarchitectural state. Additionally, the `prime+probe` technique is harder to get right than the `flush+reload` technique we allow in future labs.

While Lab 1 is challenging, we believe it is very rewarding for students to complete, and provides a great foundation for which to build on during in-class discussions and in future labs.

# Lab 3 - Spectre
**Learning Objectives**
* Apply an understanding of the cache hierarchy from lab 1 to build a new kind of side channel using shared memory (`flush+reload`).
* Understand how speculative execution works in modern out-of-order machines, and how it leaks information via the microarchitecture.
* Learn about the ISA protections provided by the `x86_64` ring model, and reason about information leakage across privilege boundaries.

**Description**
This lab builds on the foundation we developed in lab 1. Namely, we are going to extend the microarchitectural side channel techniques from lab 1 with the Spectre technique to leak kernel memory from userspace. Students will use the Spectre technique to implement their own `flush+reload` attacks against three vulnerable kernel methods, each one with more protection than the last.

**Setup** We ran this lab on four different physical Intel machines, all of them with the Linux spectre mitigations disabled (`mitigations=off`). This lab can be done using whatever desktop machines your team has lying around (could be the same machine you used for Lab 1).

Student login instructions (as well as which machine they were assigned to) were again emailed to them using a script.

This lab requires the use of a kernel module victim. The kernel module source can be found in `lab2/module-src`. You will need to compile `lab2km.c` and install it as a kernel module on every machine before allowing students to attempt lab 2.

We caution that machines with low memory and no swap space may freeze if students allocate large buffers of memory as part of their attacks. Instructors should beware of this possibility and test their machines extensively before allowing students to attempt the labs.


# Lab 4- Rowhammer

**Learning Objectives**
* Investigate the physical memory controller and learn how banked DRAM is implemented.
* Implement a double-sided Rowhammer attack to flip bits in a victim address.

**Setup** This lab requires the most effort to set up. Our environment used a job scheduling server (the same machine from lab 2) running HTCondor to schedule jobs across one of three Rowhammer victim machines. These machines were carefully profiled to determine which victim rows are succeptible to Rowhammer. Students were given one of the three machines to use (as usual, emailed to them via a script at the beginning of the lab assignment period). Students were provided with target physical addresses for their machine that course staff had pre-profiled for them (Students were asked to find their own rows as a later part of the assignment). HTCondor was used to handle job scheduling, as each student needs serialized access to the target machine for their attack.

We found that not every machine is easily vulnerable to Rowhammer. We encourage course staff to evaluate their machines extensively before distributing this lab.

**Description**
In a departure from side channel attacks, students will implement a double sided Rowhammer attack to flip victim bits at a specified physical address. Students will learn how to reverse engineer the bank mapping function, and discover their own vulnerable rows.

**Grading**
See the lab handout for our suggested grading scheme for this lab.

# Lab 5- ASLR Bypasses

**Learning Objectives**
* Understand how address-space layout randomization protects software from memory corruption attacks.
* Learn how software prefetchers pose a security risk by revealing which pages are present in the address space.
* Leverage speculative execution once again to probe the address space to learn which pages are present.
* Understand buffer overflows/ code reuse attacks and implement a return-oriented programming attack from assembly instruction gadgets.
* Combine software and hardware attacks to defeat a victim binary using a return oriented programming attack with ASLR defeated via hardware techniques.

**Setup** This lab can reuse the same setup from Lab 3.

**Description**
In this ultimate security lab, students will put all the pieces together from throughout the course to create a full end to end attack against a vulnerable piece of software. Students will learn about modern memory corruption protections, and use hardware side channels to defeat them. Then, students will build a realistic return-oriented programming (ROP) chain using the information leaked through side channels to defeat a victim process.

# Lab 6- CPU Fuzzing

**Learning Objectives**
* Learn how to use fuzzing in order to find faulty and hidden backdoor instructions in a RISC-V CPU.
* Understand the design and implementation of exception handlers in system software.
* Explore privilege separation at the hardware level and write privileged bare metal system software.
* Use CPU RTL bugs to exploit a piece of victim software using buffer overflow attacks.
* Leverage ROP techniques in order to manipulate security-critical register values.
* Practice common exploit tools and techniques, including pwntools.

# Deployment

## Course Machines
As these lab assignments exploit microarchitectural implementation-specific characteristics of modern processors, the deployment process for these labs is bespoke and varied between the labs.

The deployment setup instructions for each lab are described above. Here is a summary of what was discussed in the per-lab sections.

- Lab 1 uses student's personal machines.
- Lab 2 uses a shared server where each student is allocated one physical core (so, two logical cores that share an SMT core).
- Lab 3 uses any Intel machine with Spectre mitigations off.
- Lab 4 uses a carefully inspected Intel machine with a memory controller and DRAM that are known to be vulnerable to Rowhammer.
- Lab 5 reuses the Lab 3 setup.
- Lab 6 uses a shared server where each student is allocated a dedicated debug port.

For every lab, students require bare metal code execution (that is, no virtual machines, containers, etc. should be in between a student's code and the machine).

## Source Distribution

In our course, students submitted their code using their personal GitHub accounts. For each lab, we sent a GitHub classroom invite to the students (via Piazza), which generates a new repository based on a public template. Students submitted their code by pushing their code before the assignment deadline (late submissions were allowed, but marked late by the GitHub classroom UI). For grading purposes, it is possible to use the GitHub classroom assistant program to clone all the students’ repositories to your local machine.
