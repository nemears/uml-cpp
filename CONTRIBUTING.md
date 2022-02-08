# Contributing

Thanks for contributing to uml-cpp, all help and criticism is welcome! Following these guidelines should lead to seamless integration of your code into the repository. There are many ways to help improve uml-cpp, these may include:
* Expanding the api to encompass more of the uml 2.5 spec
* Improvements to the internal algorithms
* Improvements to the syntax/ usability of the api

Some contributions that will not be accepted to uml-cpp are:
* Modeling elements that are not in the uml 2.5 specification
* Code beyond just the communication and creation of the uml data structures, e.g. a front end showing diagrams

## Ground rules

As it is early in the development of uml-cpp there are not many "rules" at the moment, but I do encourage you to be courteous and organized about your code. As needs arise this section will become more defined. The current major rules to follow are:
* Stick to the uml 2.5 spec, it can be found [here](https://www.omg.org/spec/UML/2.5)
* Keep it simple, the api should be small not bloated
* Test your code! tests are in src/cpp/test at the moment

## How to report a bug

First of all if you find a security vulnerability email me at nickmears2@gmail.com instead of filing an issue publicly.

When making a bug please include the following information:
```
Relevant system properties: Operating system, CXX compiler, CMAKE version etc..

A detailed description of the bug: what you were doing, what you were expecting

Steps to reproduce: step by step explanation of how to reproduce the bug

```

## How to suggest an improvement

Create an issue! Just make sure to consider the scope of this repository before posting it. This repository aims to be a small and simple implementation of uml in c plus plus. Additions that are more using the api than adding to it will not be considered in the scope of this work.

## Code review process

Currently it is just me, so your code will be reveiwed and approved by me based on my availability. If you want to get in contact with me directly again my email is nickmears2@gmail.com.